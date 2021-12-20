#ifndef VKB_BINDLESS_TEXTURE_MANAGER_H
#define VKB_BINDLESS_TEXTURE_MANAGER_H

#include <cstdint>
#include <vector>
#include <vulkan/vulkan.h>
#include <exception>
#include <stdexcept>
#include <cmath>
#include <cassert>
#include <iostream>
#include <variant>
#include <cstring>
#include <algorithm>
#include <unordered_set>
#include <vk_mem_alloc.h>

#include "BindlessTextureManagerTypes.h"

namespace vkb
{

#if defined(__ANDROID__)
#define VK_CHECK_RESULT(f)																				\
{																										\
    VkResult res = (f);																					\
    if (res != VK_SUCCESS)																				\
    {																									\
        LOGE("Fatal : VkResult is \" %d \" in %s at line %d", res, __FILE__, __LINE__);					\
        assert(res == VK_SUCCESS);																		\
    }																									\
}
#else
#define VK_CHECK_RESULT(f)																				\
{																										\
    VkResult res = (f);																					\
    if (res != VK_SUCCESS)																				\
    {																									\
        std::cout << "Fatal : VkResult is \"" << res << "\" in " << __FILE__ << " at line " << __LINE__ << std::endl; \
        assert(res == VK_SUCCESS);																		\
    }																									\
}
#endif

template<typename Img_t>
struct ImgQuery;


struct ImageInfo
{
    VkImage           image     = VK_NULL_HANDLE;
    VkImageView       imageView = VK_NULL_HANDLE;
    VkImageCreateInfo info  = {};
    VmaAllocation     allocation = nullptr;
    VmaAllocationInfo allocInfo = {};
    VkImageViewType   viewType;
    std::vector<VkImageView> mipMapViews; // one image view per mipmap
    struct
    {
        VkSampler linear  = VK_NULL_HANDLE;
        VkSampler nearest = VK_NULL_HANDLE;

        VkSampler current = VK_NULL_HANDLE;
    } sampler;
};

/**
 * @brief The BindlessTextureManager class
 *
 * The BindlessTextureManager manages an array-of-textures that can be
 * used within your rendering engine.
 *
 * How it works:
 *
 * You first create your manager and set the basic parameters. such as
 * the total textures it can manage.
 *
 * When you allocate a texture from the manager of a particular size,
 * it will generate the vulkan image/memory and immediately bind it
 * to the internal descriptor set, it will return an INDEX into
 * the array-of-textures that you can use in your shader (pass it to the shader as a pushconst)
 *
 * The manager also provides functions to copy image data to the GPU.
 *
 * When you free the texture index. it will flag that texture as "freed" but does
 * not actually free the memory. The next time you allocate a new texture
 * with the same dimensions, it will return that same index to you so you
 * can reuse it.
 *
 * If the manager cannot find another free texture of the same size, it will allocate
 * a new image and memory.
 *
 *
 * You should use the textures in your shader like this:
 *
 * //========================================================================
 * // Bindless Texture
 * //========================================================================
 * layout (set = 0, binding = 0) uniform sampler2D    u_TextureArray[1024];
 * layout (set = 0, binding = 1) uniform samplerCube  u_TextureCubeArray[1024];
 *
 *
 * vec4 bindlessTexture(int index, vec2 fragTexCoord)
 * {
 *     return texture( u_TextureArray[ clamp(index, 0, 1023) ], fragTexCoord);
 * }
 * vec4 bindlessTextureCube(int index, vec3 fragTexCoord)
 * {
 *     return texture( u_TextureCubeArray[ clamp(index, 0, 1023) ], fragTexCoord);
 * }
 *
 * //========================================================================
 *
 */
class BindlessTextureManager
{
public:
    struct CreateInfo
    {
        VkInstance       instance       = VK_NULL_HANDLE;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice         device         = VK_NULL_HANDLE;
        VkQueue          graphicsQueue  = VK_NULL_HANDLE;// the queue used for submitting commands to
        VmaAllocator     allocator      = nullptr;       // can be left null, will create

        // which stages are the textures available to
        VkShaderStageFlags stageFlags   = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        //
        uint32_t totalTexture2D   = 16;
        uint32_t totalTextureCube = 16;
    };

    using _buffer = std::pair<VkBuffer, VmaAllocation>;

    /**
     * @brief create
     * @param C
     * @return
     *
     * Create all the internal data for the bindless texture manager.
     */
    bool create(CreateInfo const & C)
    {
        static_assert (sizeof(idTexture_t<0>)==sizeof(uint32_t),"");
        static_assert (sizeof(idTexture_t<1>)==sizeof(uint32_t),"");
        static_assert (sizeof(idTexture_t<2>)==sizeof(uint32_t),"");
        m_createInfo = C;
        if( m_createInfo.allocator == nullptr)
        {
            VmaAllocatorCreateInfo aci = {};
            aci.physicalDevice   = m_createInfo.physicalDevice;
            aci.device           = m_createInfo.device;
            aci.instance         = m_createInfo.instance;
            aci.vulkanApiVersion = 0;
            auto result = vmaCreateAllocator(&aci, &m_createInfo.allocator);
            if( result != VK_SUCCESS)
            {
                throw std::runtime_error("Error creator allocator");
            }
            m_selfManagedAllocator = true;
        }

        assert( m_createInfo.allocator != nullptr);
        assert( m_createInfo.device != VK_NULL_HANDLE);
        assert( m_createInfo.physicalDevice != VK_NULL_HANDLE);
        assert( m_createInfo.instance != VK_NULL_HANDLE);
        assert( m_createInfo.graphicsQueue != VK_NULL_HANDLE);
        m_commandPool = _createCommandPool();
        init(m_createInfo.totalTexture2D, m_createInfo.totalTextureCube);
        return true;
    }

    /**
     * @brief destroy
     *
     * Destroy the texture manager and all
     * textures currently being managed
     */
    void destroy()
    {
        while(m_image2D.images.size())
        {
            destroyImage(m_image2D.images.back());
            m_image2D.images.pop_back();
        }
        while(m_imageCube.images.size())
        {
            destroyImage(m_imageCube.images.back());
            m_imageCube.images.pop_back();
        }

        vkDestroyCommandPool(getDevice(), m_commandPool, nullptr);

        // destroy the descriptor pool
        vkDestroyDescriptorPool(getDevice(), m_descriptorPool,nullptr);

        // destroy the layout
        vkDestroyDescriptorSetLayout(getDevice(), m_layout, nullptr);
        m_layout = VK_NULL_HANDLE;

        if( m_createInfo.allocator && m_selfManagedAllocator)
        {
            vmaDestroyAllocator(m_createInfo.allocator);
            m_createInfo.allocator = nullptr;
            m_selfManagedAllocator = false;
        }
    }

    //=========================================================================================================================
    // Simple Texture Management
    //=========================================================================================================================
    // How it works:
    //
    //   The BindlessTextureManager manges a multiple array-of-textures,
    //   one for each type of texture: texture2D, textureCube, etc
    //
    //   When you call allocateTexture( ), it will create a texture
    //   and place it into one of the available slots in the array-of-textures
    //   you can then use that texture right away in your shader my indexing into
    //   the array of textures.
    //
    //   When you free the texture, it does not destroy the data. It will mark the
    //   image index as "free". You can still continue to index into that array
    //   index.
    //
    //   When you call allocateTexture() again, it will seach for a "free" image with
    //   the same dimensions and format to use and return that index instead.
    //   You can then use that index to copy new image data over.
    //
    //   If it cannot find a free image, it will allocate a new image if there is
    //   available indices left int he array-of-textures. If there are no more indices
    //   it will destroy any free images currently available to make room.
    //
    //   When you call allocateTexture, it returns an idTextureXXXXX. This is
    //   simply a wrapper around an index into the array-of-textures
    //   You can copy this index into your user usring PUSH CONSTANTS so that you
    //   know how to index into the array-of-textures
    //=========================================================================================================================

    /**
     * @brief allocateTexture
     * @param extent
     * @param format
     * @return
     *
     * Allocate a texture in the manager.
     */
    template<typename idType>
    idType _findAvailable(VkFormat format, VkExtent3D extent, uint32_t arrayLayers, uint32_t mipMaps)
    {
        auto &images = bindingInfo<idType>().images;
        auto &free   = bindingInfo<idType>().free;

        for(auto  i : free)
        {
            auto & _info = images[i].info;
            if( _info.extent.width  == extent.width &&
                _info.extent.height == extent.height &&
                _info.extent.depth  == extent.depth  &&
                _info.arrayLayers   == arrayLayers &&
                _info.mipLevels     == mipMaps &&
                _info.format        == format)
            {
                free.erase(i);
                return { static_cast<int32_t>(i) };
            }
        }
        return {-1};
    }

    idTexture2D allocateTexture(VkExtent2D extent,
                                VkFormat format = VK_FORMAT_R8G8B8A8_UNORM,
                                uint32_t mipMaps = 99999,
                                VkImageUsageFlags additionalUsage={})
    {
        auto ext3d        = VkExtent3D{extent.width, extent.height, 1};
        mipMaps           = std::clamp<uint32_t>(mipMaps, 1, calculateMipMaps(extent));

        return _allocateTexture<idTexture2D>(ext3d, format, 1, mipMaps, VK_IMAGE_VIEW_TYPE_2D, additionalUsage);
    }

    idTextureCube allocateTextureCube(uint32_t cubeLength,
                                      VkFormat format = VK_FORMAT_R8G8B8A8_UNORM,
                                      uint32_t mipMaps = 99999,
                                      VkImageUsageFlags additionalUsage={})
    {
        auto ext3d        = VkExtent3D{cubeLength, cubeLength, 1};
        mipMaps           = std::clamp<uint32_t>(mipMaps, 1, calculateMipMaps(cubeLength));

        return _allocateTexture<idTextureCube>(ext3d, format, 6, mipMaps, VK_IMAGE_VIEW_TYPE_CUBE, additionalUsage);
    }

    //=========================================================================================================================
    /**
     * @brief freeTexture
     * @param id
     *
     * Frees a texture, this does not automatically "free memory",
     * it keeps the texture in a memory pool. the next time you
     * allocate a texturte, it will reuse the same one if the dimensions
     * are the same.
     */
    template<typename idType>
    void freeTexture( idType id)
    {
        assert( static_cast<size_t>(id.index) < bindingInfo<idType>().images.size() );
        auto i = static_cast<size_t>(id.index);
        bindingInfo<idType>().free.insert(i);
    }
    //=========================================================================================================================


    //=========================================================================================================================
    /**
     * @brief destroyTexture
     * @param id
     *
     * Destroys the texture. This will free the memory and
     * destroy the vulkan handle. The texture in the
     * array-of-textures will replaced by the
     * null texture.
     */
    template<typename idType>
    void destroyTexture(idType id)
    {
        if( id.index == 0)
        destroyImage( bindingInfo<idType>().images[static_cast<uint32_t>(id.index)] );
        setDirty(id);
    }
    //=========================================================================================================================
    /**
     * @brief allocateTexture
     * @param img
     * @param mips
     * @return
     *
     * Allocate a texture directrly from a host-image. To be able to
     * use this function properly, you must define an ImgQuery<T> struct
     * similar to how std::hash<T> works. Here is an example:
     *
     * namespace vkb{
     * template<>
     * struct ImgQuery<SimpleImage>
     * {
     *     SimpleImage const & I;
     *     ImgQuery(SimpleImage const & i) : I(i)
     *     {
     *     }
     *     void const * data() const;
     *     uint32_t width() const;
     *     uint32_t height() const;
     *     uint32_t depth() const;
     *     uint32_t bytesPerPixel() const;
     *     VkFormat format() const;
     * };
     * }
     */
    template<typename Img_t>
    idTexture2D allocateTexture( Img_t const & img, uint32_t mips = 0 )
    {
        ImgQuery<Img_t> Q(img);
        VkExtent2D  srcExtent = { Q.width(), Q.height() };

        // allocate a texture
        auto id = allocateTexture(srcExtent, Q.format(), mips);

        // upload the data to layer 0 mip 0
        // this will convert layer 0 mip 0 into SHADER_READ_ONLY_OPTIMAL
        uploadImageData(id, img, {{0,0}, {0,0}}, 0, 0);

        if( getMipmapCount(id) > 1)
        {
            generateMipMaps(id,0,1,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }
        return id;
    }

    /**
     * @brief setImageSampler
     * @param img
     * @param filter
     * @return
     *
     * This is temporary
     */
    template<typename imageId_type>
    bool setImageSampler( imageId_type id, VkFilter filter)
    {
        if(filter == VK_FILTER_NEAREST)
        {
            auto & imgInfo = info(id);
            if(imgInfo.sampler.current != imgInfo.sampler.nearest)
            {
                imgInfo.sampler.current = imgInfo.sampler.nearest;
                setDirty(id);
                return true;
            }
        }
        else
        {
            auto & imgInfo = info(id);
            if(imgInfo.sampler.current != imgInfo.sampler.linear)
            {
                imgInfo.sampler.current = imgInfo.sampler.linear;
                setDirty(id);
            }
            return true;
        }
        return false;
    }

    //=========================================================================================================================

    std::string generateShaderFunctions(uint32_t setNumber) const
    {
        std::string out;
        out += "layout (set = " + std::to_string(setNumber) + ", binding = 0) uniform sampler2D   u_TextureArray[    " + std::to_string(m_createInfo.totalTexture2D) + "];  \n";
        out += "layout (set = " + std::to_string(setNumber) + ", binding = 1) uniform samplerCube u_TextureCubeArray[" + std::to_string(m_createInfo.totalTextureCube) + "];  ";
        return out;
    }



    template<typename imageId_type, typename Img_t>
    void uploadImageData( imageId_type dstId,
                          Img_t const & img,
                          VkRect2D dstRect = { {0,0}, {0,0}},
                          uint32_t dstLayer = 0,
                          uint32_t dstMip = 0)
    {
        ImgQuery<Img_t> Q(img);
        VkExtent2D  srcExtent = { Q.width(), Q.height() };
        void const * srcData = Q.data();
        auto bbp = Q.bytesPerPixel();
        if( dstRect.extent.width == 0)
        {
            dstRect.extent.width = srcExtent.width;
        }
        if( dstRect.extent.height == 0)
        {
            dstRect.extent.height = srcExtent.height;
        }
        auto byteSize = srcExtent.width*srcExtent.height*bbp;
        uploadImageData(dstId, srcData, byteSize, srcExtent, dstRect, dstLayer, dstMip);
    }

    /**
     * @brief uploadImageData
     * @param dstId
     * @param srcData
     * @param srcByteSize
     * @param srcExtent
     * @param dstRect
     * @param dstLayer
     * @param dstMip
     *
     * Uploads data from the host to the GPU image.
     * srcData/srcByteSize and srcExtent define the imagedata on the host,
     * dstRect,dstLayer and dstMip define the location where the image will be
     * copied into.
     *
     */
    template<typename textureIDType>
    void uploadImageData( textureIDType dstId,
                          void const * srcData,
                          uint32_t     srcByteSize,
                          VkExtent2D   srcExtent,

                          VkRect2D     dstRect,
                          uint32_t     dstLayer,
                          uint32_t     dstMip)
    {
        auto stg = _allocateStagingBuffer(srcData, srcByteSize);

        auto image = getImage(dstId);
        auto extent = getExtent(dstId);

        _beginCommandBuffer([=](auto cmd)
        {
            _transitionLayout(cmd, image,
                              VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                              dstLayer, 1,
                              dstMip  , 1);

            VkExtent3D _extent;
            uint32_t dstRectOffsetx = static_cast<uint32_t>(dstRect.offset.x);
            uint32_t dstRectOffsety = static_cast<uint32_t>(dstRect.offset.y);
            _extent.depth  = 1;
            _extent.width  = std::min(extent.width  - dstRectOffsetx, dstRect.extent.width) ;
            _extent.height = std::min(extent.height - dstRectOffsety, dstRect.extent.height);

            VkOffset3D _offset{ dstRect.offset.x, dstRect.offset.y, 0};

            _copyBufferToImage(cmd, stg, srcExtent.width,srcExtent.height, dstId, _offset, _extent, dstLayer, dstMip);

            _transitionLayout(cmd, image,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                              dstLayer, 1,
                              dstMip  , 1);
        });

        _destroyBuffer(stg);
    }

    /**
     * @brief generateMipMaps
     * @param id
     * @param mip0InitialLayout
     *
     * Generate mipmaps for an image. This will generate the mipmaps by
     * bliting Mip:0 to all the smaller mips
     */
    template<typename textureId_type>
    void generateMipMaps(textureId_type id,
                         uint32_t baseArrayLayer, uint32_t layerCount,
                         VkImageLayout mip0InitialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        VkFilter filter = VK_FILTER_LINEAR;

        auto maxLayers = getLayerCount(id);
        layerCount = std::min(maxLayers-baseArrayLayer, layerCount);
        VkImageBlit region;
        region.srcSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        region.srcSubresource.baseArrayLayer = baseArrayLayer;
        region.srcSubresource.layerCount     = layerCount;
        region.srcSubresource.mipLevel       = 0;

        region.dstSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        region.dstSubresource.baseArrayLayer = baseArrayLayer;
        region.dstSubresource.layerCount     = layerCount;
        region.dstSubresource.mipLevel       = 0;

        region.srcOffsets[0] = {0,0,0};
        region.dstOffsets[0] = {0,0,0};

        auto ext = getExtent(id);

        region.srcOffsets[1] = {static_cast<int32_t>(ext.width),static_cast<int32_t>(ext.height),1};
        region.dstOffsets[1] = {static_cast<int32_t>(ext.width),static_cast<int32_t>(ext.height),1};;

        uint32_t maxMip = getMipmapCount(id);
        auto     image  = getImage(id);

        _beginCommandBuffer([=](auto cmd)
        {
            // transition the first mip level into
            // source
            _image_TransitionLayout(cmd,
                                    image,
                                    mip0InitialLayout,
                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                    baseArrayLayer, layerCount,
                                    0, 1);

            for(uint32_t i=1;i<maxMip;i++)
            {
                auto reg = region;

                reg.srcSubresource.mipLevel = i-1;
                reg.srcOffsets[1].x >>= (i-1);
                reg.srcOffsets[1].y >>= (i-1);
               // reg.srcOffsets[1].z >>= (i-1);

                reg.dstSubresource.mipLevel = i;
                reg.dstOffsets[1].x >>= i;
                reg.dstOffsets[1].y >>= i;
               // reg.dstOffsets[1].z >>= i;

                _image_TransitionLayout(cmd,
                                        image,
                                        VK_IMAGE_LAYOUT_UNDEFINED,
                                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                        baseArrayLayer, layerCount,
                                        i, 1);

                // convert layer i-1 into transfer src
                // convert layer i into transfer dst
                vkCmdBlitImage(cmd, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &reg,filter);

                _image_TransitionLayout(cmd,
                                        image,
                                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                        baseArrayLayer, layerCount,
                                        i, 1);
            }

            _image_TransitionLayout(cmd,
                                    image,
                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                    baseArrayLayer, layerCount,
                                    0, maxMip);
        });

    }



    template<typename idType>
    ImageInfo & info(idType id)
    {
        return bindingInfo<idType>().images.at(static_cast<size_t>(id.index) );
    }
    template<typename idType>
    ImageInfo const & info(idType id) const
    {
        return bindingInfo<idType>().images.at( static_cast<size_t>(id.index) );
    }

    template<typename idType>
    VkFormat getFormat(idType id) const
    {
        return info(id).format;
    }
    uint32_t formatSize(VkFormat f) const
    {
        (void)f;
        assert( f == VK_FORMAT_R8G8B8A8_UNORM);
        return 4;
    }
    template<typename idType>
    uint32_t getMipmapCount(idType id) const
    {
        return info(id).info.mipLevels;
    }
    template<typename idType>
    uint32_t getLayerCount(idType id) const
    {
        return info(id).info.arrayLayers;
    }
    template<typename idType>
    VkExtent3D getExtent(idType id) const
    {
        return info(id).info.extent;
    }
    template<typename idType>
    VkImage getImage(idType id) const
    {
        return info(id).image;
    }
    template<typename idType>
    VkImageView getImageView(idType id) const
    {
        return info(id).imageView;
    }
    template<typename idType>
    VkImageView getMipMapImageView(idType id, uint32_t mipmap) const
    {
        return info(id).mipMapViews.at(mipmap);
    }



    //===================================================================================================
    // Texture Manager Usage:
    //
    // To use the texture manager during your render loop:
    // at the start of the frame you should call:
    //
    //      TextureManager::nextChain();
    //      TextureManager::update();
    //
    // This updates all images that have been flaged as dirty and performs
    // a descriptorWrite on all of them.
    //
    //
    //
    // For each graphics pipeline that wants to use the textures
    // you should make sure you create your pipeline layout
    // with the descriptor set layout managed by the TextureManager
    //
    //  TextureManager::getDescriptorSetLayout()
    //
    // The texture manger's textures should all be used on a single set
    // set 0 is probably the easest one to use.
    //  VkPipelineLayoutCreateInfo ci = {};
    //  ci.pSetLayouts = {  TextureManager::getDescriptorSetLayout(),
    //                      // your other set layouts };
    //
    //
    // Then when you bind the pipeline, you should also bind the texture
    // manager:
    //
    //   TextureManager::bind(commandBuffer, 0, pipelineLayout);
    //===================================================================================================
    /**
     * @brief update
     *
     * Updates all the dirty indices in the texture array.
     *
     * This should only be called ONCE per frame
     */
    void update()
    {
        if(m_image2D.images.size() == 0)
        {
            throw std::runtime_error("Cannot update images. Need to allocate at least 1 image");
        }
        auto   chainIndex = getCurrentChain();
        auto   set        = m_dsetChain.at(chainIndex);

        // at this point, _updateAllDirty will go through all
        // the indices in chainLink.dirty, and perform
        // a descriptor write on those indices.
        // if the VkImage for that array index
        // is a null handle (ie: it has been destroyed)
        // then it will use the VkImage in index[ 0 ]
        {
            auto & chainLink = bindingInfo<idTexture2D>().arrayInfo.at(chainIndex);
            _updateAllDirty(getDevice(), set, chainLink.binding, bindingInfo<idTexture2D>().images, chainLink.dirty);
        }
        {
            auto & chainLink = bindingInfo<idTextureCube>().arrayInfo.at(chainIndex);
            _updateAllDirty(getDevice(), set, chainLink.binding, bindingInfo<idTextureCube>().images, chainLink.dirty);
        }
    }

    void nextChain()
    {
        m_currentChain = (m_currentChain+1) % m_dsetChain.size();
    }

    /**
     * @brief bind
     * @param cmd
     * @param set
     * @param layout
     *
     * Bind the entire texture descriptor set to the
     * command buffer.
     */
    void bind(VkCommandBuffer cmd, uint32_t set, VkPipelineLayout layout)
    {
        auto dset = m_dsetChain[ getCurrentChain() ];
        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, set, 1, &dset, 0, nullptr);
    }
    //===================================================================================================

    size_t getCurrentChain() const
    {
        return m_currentChain;
    }


    template<typename textureId_type>
    void setDirty(textureId_type id)
    {
        for(auto & x : bindingInfo<textureId_type>().arrayInfo)
        {
            x.dirty.push_back( static_cast<size_t>(id.index) );
        }
    }


    void copyImageData(void const * pixelData, size_t byteSize,
                       uint32_t srcPixelWidth,
                       uint32_t srcPixelsHeight,
                       VkImage dstImage,
                       VkOffset3D dstOffset,
                       VkExtent3D dstExtent,
                       uint32_t arrayBaseLayer,
                       uint32_t mipBaseLevel)
    {
        auto staging = _allocateBuffer(byteSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
        void * mapped = nullptr;
        vmaMapMemory(getAllocator(), staging.second, &mapped);

        std::memcpy(mapped, pixelData, byteSize);
        vmaFlushAllocation(getAllocator(), staging.second, 0, byteSize);

        { // do buffer copy
            VkBufferImageCopy region = {};

            region.bufferOffset                    = 0;
            region.bufferRowLength                 = srcPixelWidth;
            region.bufferImageHeight               = srcPixelsHeight;
            region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.baseArrayLayer = arrayBaseLayer;
            region.imageSubresource.layerCount     = 1;
            region.imageSubresource.mipLevel       = mipBaseLevel;
            region.imageSubresource.layerCount     = 1;
            region.imageOffset                     = dstOffset;
            region.imageExtent                     = dstExtent;

            auto cmdBuffer = _allocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

            vkCmdCopyBufferToImage(
                cmdBuffer,
                staging.first,
                dstImage,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                static_cast<uint32_t>(1),
                &region
            );

            vkEndCommandBuffer(cmdBuffer);
            _submitCommandBuffer(cmdBuffer, true);
            _freeCommandBuffer(cmdBuffer);
        }

        vmaUnmapMemory(getAllocator(), staging.second);
        _destroyBuffer(staging);
    }

    VkDescriptorSetLayout getDescriptorSetLayout() const
    {
        return m_layout;
    }
    VmaAllocator getAllocator()
    {
        return m_createInfo.allocator;
    }
protected:

    void init(uint32_t total2DTextures, uint32_t totalTextureCube)
    {
        VkDescriptorSetLayoutCreateInfo ci = {};

        std::vector<VkDescriptorSetLayoutBinding> bindings;

        {
            auto & L1 = bindings.emplace_back();
            L1.binding         = 0;
            L1.descriptorCount = total2DTextures;
            L1.stageFlags      = m_createInfo.stageFlags;
            L1.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        }
        if(totalTextureCube > 0)
        {
            auto & L1 = bindings.emplace_back();
            L1.binding         = 1;
            L1.descriptorCount = totalTextureCube;
            L1.stageFlags      = m_createInfo.stageFlags;
            L1.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        }

        ci.sType           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        ci.bindingCount    = static_cast<uint32_t>(bindings.size());
        ci.pBindings       = bindings.data();
        ci.flags           = {};

        VK_CHECK_RESULT( vkCreateDescriptorSetLayout(getDevice(), &ci, nullptr, & m_layout) );


        //=================
        uint32_t maxSets = 10;

        std::vector<VkDescriptorPoolSize> sizes;

        sizes.push_back( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, (totalTextureCube + total2DTextures) * maxSets });

        VkDescriptorPoolCreateInfo poolCI = {};
        poolCI.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolCI.maxSets       = maxSets;
        poolCI.pPoolSizes    = sizes.data();
        poolCI.poolSizeCount = static_cast<uint32_t>(sizes.size());

        VK_CHECK_RESULT( vkCreateDescriptorPool(getDevice(), &poolCI, nullptr, &m_descriptorPool) );

        size_t maxDescriptorChain = 5;

        //==================
        for(size_t i=0;i<maxDescriptorChain;i++)
        {
            VkDescriptorSetAllocateInfo allocInfo = {};
            allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool     = m_descriptorPool;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts        = &m_layout;

            VkDescriptorSet dSet;
            VK_CHECK_RESULT(vkAllocateDescriptorSets(getDevice(), &allocInfo, &dSet));

            {
                auto & dArrayInfo = bindingInfo<idTexture2D>().arrayInfo.emplace_back();
                dArrayInfo.binding = idTexture2D::binding;
            }
            {
                auto & dArrayInfo = bindingInfo<idTextureCube>().arrayInfo.emplace_back();
                dArrayInfo.binding = idTextureCube::binding;
            }
            m_dsetChain.push_back(dSet);
        }


        // always allocate a "null" texture
        allocateTexture({8,8}, VK_FORMAT_R8G8B8A8_UNORM, 1);
        allocateTextureCube(8, VK_FORMAT_R8G8B8A8_UNORM, 1);

        for(size_t i=0;i<total2DTextures;i++)
        {
            setDirty(idTexture2D{static_cast<int32_t>(i)});
        }
        for(size_t i=0;i<totalTextureCube;i++)
        {
            setDirty(idTextureCube{static_cast<int32_t>(i)});
        }
    }

    static uint32_t calculateMipMaps(VkExtent3D extent)
    {
        return calculateMipMaps( std::min( std::min( extent.width, extent.height), extent.depth ) );
    }
    static uint32_t calculateMipMaps(VkExtent2D extent)
    {
        return calculateMipMaps( std::min( extent.width, extent.height) );
    }
    static uint32_t calculateMipMaps(uint32_t w)
    {
        return  1 + static_cast<uint32_t>(std::floor(std::log2(w)));
    }

    ImageInfo  image_Create(  VkExtent3D extent
                             ,VkFormat format
                             ,VkImageViewType viewType
                             ,uint32_t arrayLayers
                             ,uint32_t miplevels // maximum mip levels
                             ,VkImageUsageFlags additionalUsageFlags
                                    )
    {
        VkImageCreateInfo imageInfo{};

        imageInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType     = extent.depth==1 ? VK_IMAGE_TYPE_2D : VK_IMAGE_TYPE_3D;
        imageInfo.format        = format;
        imageInfo.extent        = extent;
        imageInfo.mipLevels     = miplevels;
        imageInfo.arrayLayers   = arrayLayers;

        imageInfo.samples       = VK_SAMPLE_COUNT_1_BIT;// vk::SampleCountFlagBits::e1;
        imageInfo.tiling        = VK_IMAGE_TILING_OPTIMAL;// vk::ImageTiling::eOptimal;
        imageInfo.usage         = additionalUsageFlags | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;// vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst;
        imageInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;// vk::SharingMode::eExclusive;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;// vk::ImageLayout::eUndefined;

        //if( imageInfo.mipLevels == 0)
        //{
        //    imageInfo.mipLevels = calculateMipMaps( VkExtent2D{extent.width, extent.height});
        //    miplevels = imageInfo.mipLevels;
        //}

        if( arrayLayers == 6)
            imageInfo.flags |=  VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;// vk::ImageCreateFlagBits::eCubeCompatible;

        VmaAllocationCreateInfo allocCInfo = {};
        allocCInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

        VkImage           image;
        VmaAllocation     allocation;
        VmaAllocationInfo allocInfo;

        VkImageCreateInfo & imageInfo_c = imageInfo;
        VK_CHECK_RESULT( vmaCreateImage(getAllocator(),  &imageInfo_c,  &allocCInfo, &image, &allocation, &allocInfo) );

        ImageInfo I;
        I.image      = image;
        I.info       = imageInfo;
        I.allocInfo  = allocInfo;
        I.allocation = allocation;
        I.viewType   = viewType;

        // create the image view
        {
            {
                VkImageViewCreateInfo ci{};
                ci.sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                ci.image      = I.image;
                ci.viewType   = viewType;
                ci.format     = format;
                ci.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};

                ci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

                ci.subresourceRange.baseMipLevel = 0;
                ci.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;

                ci.subresourceRange.baseArrayLayer = 0;
                ci.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

                switch(ci.format)
                {
                    case VK_FORMAT_D16_UNORM:
                    case VK_FORMAT_D32_SFLOAT:
                    case VK_FORMAT_D16_UNORM_S8_UINT:
                    case VK_FORMAT_D24_UNORM_S8_UINT:
                    case VK_FORMAT_D32_SFLOAT_S8_UINT:
                        ci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;// vk::ImageAspectFlagBits::eDepth;
                        break;
                    default:
                        ci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; //vk::ImageAspectFlagBits::eColor;
                        break;
                }

                VK_CHECK_RESULT( vkCreateImageView(getDevice(), &ci, nullptr, &I.imageView) );

                // Create one image view per mipmap level
                for(uint32_t i=0;i<miplevels;i++)
                {
                    ci.subresourceRange.baseMipLevel = i;
                    ci.subresourceRange.levelCount = 1;

                    VkImageView vv;
                    VK_CHECK_RESULT( vkCreateImageView(getDevice(), &ci, nullptr, &vv) );
                    I.mipMapViews.push_back(vv);
                }
            }


        }


        // create a sampler
        {
            // Temporary 1-mipmap sampler
            VkSamplerCreateInfo ci;
            ci.sType                   =  VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            ci.magFilter               =  VK_FILTER_LINEAR;//  vk::Filter::eLinear;
            ci.minFilter               =  VK_FILTER_LINEAR;//  vk::Filter::eLinear;
            ci.mipmapMode              =  VK_SAMPLER_MIPMAP_MODE_LINEAR;// vk::SamplerMipmapMode::eLinear ;
            ci.addressModeU            =  VK_SAMPLER_ADDRESS_MODE_REPEAT;//vk::SamplerAddressMode::eRepeat ;
            ci.addressModeV            =  VK_SAMPLER_ADDRESS_MODE_REPEAT;//vk::SamplerAddressMode::eRepeat ;
            ci.addressModeW            =  VK_SAMPLER_ADDRESS_MODE_REPEAT;// vk::SamplerAddressMode::eRepeat ;
            ci.mipLodBias              =  0.0f  ;
            ci.anisotropyEnable        =  VK_FALSE;
            ci.maxAnisotropy           =  1 ;
            ci.compareEnable           =  VK_FALSE ;
            ci.compareOp               =  VK_COMPARE_OP_ALWAYS;// vk::CompareOp::eAlways ;
            ci.minLod                  =  0 ;
            ci.maxLod                  =  VK_LOD_CLAMP_NONE;//static_cast<float>(miplevels);
            ci.borderColor             =  VK_BORDER_COLOR_INT_OPAQUE_BLACK;// vk::BorderColor::eIntOpaqueBlack ;
            ci.unnormalizedCoordinates =  VK_FALSE ;

            ci.magFilter               =  VK_FILTER_LINEAR;//vk::Filter::eLinear;
            ci.minFilter               =  VK_FILTER_LINEAR;//vk::Filter::eLinear;

            vkCreateSampler(getDevice(), &ci, nullptr, &I.sampler.linear);

            ci.magFilter               =  VK_FILTER_NEAREST;//vk::Filter::eNearest;
            ci.minFilter               =  VK_FILTER_NEAREST;//vk::Filter::eNearest;

            vkCreateSampler(getDevice(), &ci, nullptr, &I.sampler.nearest);

            I.sampler.current = I.sampler.linear;
        }

        return I;
    }

    template<typename imageId_type>
    void _copyBufferToImage( VkCommandBuffer cmd,
                            _buffer srcBuffer,
                            uint32_t srcImageWidth,
                            uint32_t srcImageHeight,
                            imageId_type dstId,
                            VkOffset3D dstOffset,
                            VkExtent3D dstExtent,
                            uint32_t   dstArrayBaseLayer,
                            uint32_t   dstMipBaseLevel
                          )
    {
        //auto & m_images = m_image2D.images;
        // do buffer copy
        VkBufferImageCopy region = {};

        region.bufferOffset                    = 0;
        region.bufferRowLength                 = srcImageWidth;
        region.bufferImageHeight               = srcImageHeight;
        region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.baseArrayLayer = dstArrayBaseLayer;
        region.imageSubresource.layerCount     = 1;
        region.imageSubresource.mipLevel       = dstMipBaseLevel;
        region.imageSubresource.layerCount     = 1;
        region.imageOffset                     = dstOffset;
        region.imageExtent                     = dstExtent;

        vkCmdCopyBufferToImage(
            cmd,
            srcBuffer.first,
            getImage(dstId),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            static_cast<uint32_t>(1),
            &region
        );
    }

    void _destroyBuffer(_buffer buffer)
    {
        vmaDestroyBuffer(getAllocator(), buffer.first, buffer.second);
    }

    _buffer _allocateStagingBuffer(void const * data, uint32_t byteSize)
    {
        auto staging = _allocateBuffer(byteSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
        void * mapped = nullptr;
        vmaMapMemory(getAllocator(), staging.second, &mapped);

        std::memcpy(mapped, data, byteSize);
        vmaFlushAllocation(getAllocator(), staging.second, 0, byteSize);

        return staging;
    }

    _buffer _allocateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memUsage)
    {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size  = size;
        bufferInfo.usage = usage;

        VmaAllocationCreateInfo allocCInfo = {};
        allocCInfo.usage = memUsage;
        allocCInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT; // can always set this bit,
                                                             // vma will not allow device local
                                                             // memory to be mapped

        VkBuffer          buffer;
        VmaAllocation     allocation = nullptr;
        VmaAllocationInfo allocInfo;

        auto result = vmaCreateBuffer(getAllocator(), &bufferInfo, &allocCInfo, &buffer, &allocation, &allocInfo);

        VK_CHECK_RESULT(result);

        return {buffer,allocation};
    }

    void destroyImage(ImageInfo & I)
    {
        vkDestroySampler(getDevice(), I.sampler.linear,  nullptr);
        vkDestroySampler(getDevice(), I.sampler.nearest, nullptr);
        vkDestroyImageView(getDevice(), I.imageView, nullptr);
        for(auto & v : I.mipMapViews)
        {
            vkDestroyImageView(getDevice(), v, nullptr);
        }
        vmaDestroyImage(getAllocator(), I.image, I.allocation);

        I.allocation = nullptr;
        I.image      = VK_NULL_HANDLE;
        I.imageView  = VK_NULL_HANDLE;
        I.sampler.linear = I.sampler.nearest = VK_NULL_HANDLE;
        I.sampler.current = VK_NULL_HANDLE;
        I.mipMapViews.clear();
    }

    template<typename callable_t>
    void _beginCommandBuffer(callable_t && C)
    {
        auto b = _allocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

        C(b);

        vkEndCommandBuffer(b);
        _submitCommandBuffer(b, true);
    }

    VkCommandBuffer _allocateCommandBuffer(VkCommandBufferLevel level, bool begin)
    {
        VkCommandBufferAllocateInfo cmdBufAllocateInfo{};
        cmdBufAllocateInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmdBufAllocateInfo.commandPool        = getCommandPool();
        cmdBufAllocateInfo.level              = level;
        cmdBufAllocateInfo.commandBufferCount = 1;

        VkCommandBuffer cmdBuffer;
        VK_CHECK_RESULT(vkAllocateCommandBuffers(getDevice(), &cmdBufAllocateInfo, &cmdBuffer));

        // If requested, also start recording for the new command buffer
        if (begin)
        {
            VkCommandBufferBeginInfo commandBufferBI{};
            commandBufferBI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            VK_CHECK_RESULT(vkBeginCommandBuffer(cmdBuffer, &commandBufferBI));
        }

        return cmdBuffer;
    }

    VkCommandPool getCommandPool()
    {
        return m_commandPool;
    }

    void _freeCommandBuffer(VkCommandBuffer cmd)
    {
        vkFreeCommandBuffers(getDevice(), getCommandPool(), 1, &cmd);
    }

    VkFence _submitCommandBuffer(VkCommandBuffer cmd, bool waitForFence=true)
    {
        VkSubmitInfo submitInfo{};
        submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers    = &cmd;

        {
            // Create fence to ensure that the command buffer has finished executing
            VkFenceCreateInfo fenceInfo{};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            VkFence fence;
            VK_CHECK_RESULT(vkCreateFence(getDevice(), &fenceInfo, nullptr, &fence));

            VK_CHECK_RESULT(vkQueueSubmit(getGraphicsQueue(), 1, &submitInfo, fence));

            if( waitForFence )
            {
                vkWaitForFences( getDevice(), 1, &fence, VK_TRUE,100000000000);
                vkDestroyFence(getDevice(), fence, nullptr);
                return VK_NULL_HANDLE;
            }
            return fence;
        }
    }

    void _image_TransitionLayout(VkCommandBuffer cmd,
                                VkImage image,
                                VkImageLayout oldLayout,
                                VkImageLayout newLayout,
                                uint32_t firstLayer, uint32_t layerCount,
                                uint32_t firstMip  , uint32_t mipCount)
    {
        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask              = VK_IMAGE_ASPECT_COLOR_BIT;
        subresourceRange.baseMipLevel            = firstMip;
        subresourceRange.levelCount              = mipCount;
        subresourceRange.baseArrayLayer          = firstLayer;
        subresourceRange.layerCount              = layerCount;

        // Image barrier for optimal image (target)
        // Optimal image will be used as destination for the copy
        {
            VkImageMemoryBarrier imageMemoryBarrier{};
            imageMemoryBarrier.sType            = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageMemoryBarrier.oldLayout        = oldLayout;
            imageMemoryBarrier.newLayout        = newLayout;
            imageMemoryBarrier.srcAccessMask    = 0;
            imageMemoryBarrier.dstAccessMask    = VK_ACCESS_TRANSFER_WRITE_BIT;
            imageMemoryBarrier.image            = image;
            imageMemoryBarrier.subresourceRange = subresourceRange;

            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
        }
    }

    VkCommandPool _createCommandPool( VkQueueFlags queueFlagBits = VK_QUEUE_TRANSFER_BIT | VK_QUEUE_GRAPHICS_BIT,
                                      VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
    {
        VkCommandPoolCreateInfo cmdPoolInfo = {};
        cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

        cmdPoolInfo.queueFamilyIndex = _getQueueFamilyIndex(queueFlagBits, getPhysicalDevice());

        cmdPoolInfo.flags = createFlags;
        VkCommandPool cmdPool;
        VK_CHECK_RESULT(vkCreateCommandPool(getDevice(), &cmdPoolInfo, nullptr, &cmdPool));
        return cmdPool;
    }

    static uint32_t _getQueueFamilyIndex(VkQueueFlags queueFlags, VkPhysicalDevice physicalDevice)
    {
        std::vector<VkQueueFamilyProperties> queueFamilyProperties;
        uint32_t queueFamilyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        assert(queueFamilyCount > 0);
        queueFamilyProperties.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

        // Dedicated queue for compute
        // Try to find a queue family index that supports compute but not graphics
        if (queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++) {
                if ((queueFamilyProperties[i].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)) {
                    return i;
                    break;
                }
            }
        }

        // For other queue types or if no separate compute queue is present, return the first one to support the requested flags
        for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++) {
            if (queueFamilyProperties[i].queueFlags & queueFlags) {
                return i;
                break;
            }
        }

        throw std::runtime_error("Could not find a matching queue family index");
    }

    void _transitionLayout(VkCommandBuffer cmd,
                           VkImage image,
                           VkImageLayout oldLayout,
                           VkImageLayout newLayout,
                           uint32_t firstLayer, uint32_t layerCount,
                           uint32_t firstMip  , uint32_t mipCount)
    {
        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

        subresourceRange.baseMipLevel = firstMip;
        subresourceRange.levelCount   = mipCount;

        subresourceRange.baseArrayLayer = firstLayer;
        subresourceRange.layerCount     = layerCount;

        // Image barrier for optimal image (target)
        // Optimal image will be used as destination for the copy
        {
            VkImageMemoryBarrier imageMemoryBarrier{};
            imageMemoryBarrier.sType         = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageMemoryBarrier.oldLayout     = oldLayout;
            imageMemoryBarrier.newLayout     = newLayout;
            imageMemoryBarrier.srcAccessMask = 0;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            imageMemoryBarrier.image         = image;
            imageMemoryBarrier.subresourceRange = subresourceRange;

            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
        }
    }

    VkQueue getGraphicsQueue() const
    {
        return m_createInfo.graphicsQueue;
    }
    VkPhysicalDevice getPhysicalDevice() const
    {
        return m_createInfo.physicalDevice;
    }
    VkDevice getDevice() const
    {
        // destroy the layout
        return m_createInfo.device;
    }

    VkDescriptorSetLayout m_layout;
    VkDescriptorPool      m_descriptorPool;
    VkCommandPool         m_commandPool;

    struct DescriptorArrayInfo
    {
        uint32_t            binding;
        std::vector<size_t> dirty;
    };

    struct bindingInfo
    {
        std::vector<ImageInfo>           images;
        std::unordered_set<size_t>       free;
        std::vector<DescriptorArrayInfo> arrayInfo; // one for each chain
    };

    //===================================================
    size_t                       m_currentChain = 0;
    std::vector<VkDescriptorSet> m_dsetChain;
    bindingInfo                  m_image2D;
    bindingInfo                  m_imageCube;
    //===================================================

    template<typename idType>
    bindingInfo & bindingInfo()
    {
        if constexpr ( std::is_same_v<idType, idTexture2D >)
        {
            return m_image2D;
        }
        if constexpr ( std::is_same_v<idType, idTextureCube >)
        {
            return m_imageCube;
        }
    }
    template<typename idType>
    struct bindingInfo const & bindingInfo() const
    {
        if constexpr ( std::is_same_v<idType, idTexture2D >)
        {
            return m_image2D;
        }
        if constexpr ( std::is_same_v<idType, idTextureCube >)
        {
            return m_imageCube;
        }
    }

    void _updateAllDirty(VkDevice device, VkDescriptorSet descriptorSet, uint32_t binding, std::vector<ImageInfo> const & images, std::vector<size_t> & m_dirty)
    {
        std::vector<VkWriteDescriptorSet> writes;
        std::vector<VkDescriptorImageInfo> imageInfos;
        writes.reserve(m_dirty.size());
        imageInfos.reserve(m_dirty.size());

        std::sort(m_dirty.begin(), m_dirty.end());
        m_dirty.erase( std::unique( m_dirty.begin(), m_dirty.end()), m_dirty.end());

        for(auto j : m_dirty)
        {
            auto & b = writes.emplace_back();
            b.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            b.dstArrayElement = static_cast<uint32_t>(j);
            b.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            b.dstBinding      = binding;
            b.dstSet          = descriptorSet;

            auto &img         = imageInfos.emplace_back();
            {
                auto i = std::min( images.size()-1, j);

                // if the image doesn't exist, (ie: it has been destroyed)
                // then use the first image in the array instead.
                // this is usually the "null image"
                if( images[i].image == VK_NULL_HANDLE)
                {
                    auto it = std::find_if( images.begin(),
                                            images.end(),
                                            [](auto & _img)
                                            {
                                                return _img.image != VK_NULL_HANDLE;
                                            });
                    if( it == images.end())
                    {
                        throw std::runtime_error("Image array has no valid images");
                    }
                    i = static_cast<size_t>(std::distance(images.begin(), it));
                }

                assert( images[i].image != VK_NULL_HANDLE );

                img.imageLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                img.imageView     = images[i].imageView;
                img.sampler       = images[i].sampler.current;
            }

            b.pImageInfo      = &img;
            b.descriptorCount = 1;
        }
        if( m_dirty.size() != 0)
        {
            std::cerr << "Updating: " << descriptorSet << ": " << m_dirty.size() << " descriptors" << std::endl;
        }

        vkUpdateDescriptorSets(device, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
        m_dirty.clear();
    }

    template<typename idType>
    size_t maxTextures() const
    {
        if constexpr ( std::is_same_v<idType, idTexture2D >)
        {
            return m_createInfo.totalTexture2D;
        }
        if constexpr ( std::is_same_v<idType, idTextureCube >)
        {
            return m_createInfo.totalTextureCube;
        }
    }

    template<typename idType>
    idType _allocateTexture(VkExtent3D ext3d,
                                VkFormat format,
                                uint32_t layers,
                                uint32_t mipMaps,
                                VkImageViewType viewType,
                                VkImageUsageFlags additionalUsageFlags)
    {
        auto &images = bindingInfo<idType>().images;

        // Loop through all the free images.
        // These are images which were no longer needed, but are still
        // allocated. They can be reused.
        // So find one that fits the dimensions/format/etc
        // and return that one. none exist, then
        // we will have to create a new texture
        {
            auto id = _findAvailable<idType>(format, ext3d, 1, mipMaps);

            if( id.index != -1)
            {
                // a free image was found
                return id;
            }
        }


        // find an index in the image array
        // that is currently not being used and returns that one
        // if none can be found, appends an index to the end if
        // it is not already at its max capacity.
        auto _findAvailableIndex = [&]() -> idType
        {
            for(auto & _im : images)
            {
                if(_im.image == VK_NULL_HANDLE)
                {
                    return idType{ static_cast<int32_t>(std::distance(&images.front(), &_im)) };
                }
            }
            if( images.size() < maxTextures<idType>() )
            {
                images.emplace_back();
                return idType{ static_cast<int32_t>(images.size()-1) };
            }
            return idType{-1};
        };



        {
            auto id = _findAvailableIndex();
            if( id.index == -1)
            {
                throw std::runtime_error("Unable to allocate a new texture");
            }


            auto img = image_Create( ext3d, format, viewType, layers, mipMaps, additionalUsageFlags);
            images[static_cast<size_t>(id.index)] = img;

            _beginCommandBuffer([=](auto cmd)
            {
                _transitionLayout(cmd, getImage(id),
                                  VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                  0, getLayerCount(id),
                                  0, getMipmapCount(id));
            });

            setDirty(id);
            return id;
        }

    }



    CreateInfo                   m_createInfo;
    bool                         m_selfManagedAllocator = false;
};

}
#endif
