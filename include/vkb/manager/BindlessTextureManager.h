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
#include <vk_mem_alloc.h>

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

struct idTexture2D
{
    int32_t index = -1;
};
struct idTexture2DArray
{
    int32_t index = -1;
};
struct idTextureCube
{
    int32_t index = -1;
};

template<typename Img_t>
struct ImgQuery;


struct ImageInfo
{
    VkImage           image;
    VkImageView       imageView;
    VkImageCreateInfo info;
    VmaAllocation     allocation;
    VmaAllocationInfo allocInfo;
    VkImageViewType   viewType;
    struct
    {
        VkSampler linear;
        VkSampler nearest;
    } sampler;
};

struct DescriptorChain
{
    VkDescriptorSet     m_descriptorSet;
    std::vector<size_t> dirty;

    void update(VkDevice device, std::vector<ImageInfo> const & images)
    {
        std::vector<VkWriteDescriptorSet> writes;
        std::vector<VkDescriptorImageInfo> imageInfos;
        writes.reserve(dirty.size());
        imageInfos.reserve(dirty.size());

        std::sort(dirty.begin(), dirty.end());
        dirty.erase( std::unique( dirty.begin(), dirty.end()), dirty.end());


        for(auto j : dirty)
        {
            auto i = std::min( images.size()-1, j);

            auto & b = writes.emplace_back();
            b.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            b.dstArrayElement = static_cast<uint32_t>(i);
            b.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            b.dstBinding      = 0;
            b.dstSet          = m_descriptorSet;

            auto &img         = imageInfos.emplace_back();
            img.imageLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            img.imageView     = images[i].imageView;
            img.sampler       = images[i].sampler.linear;

            b.pImageInfo      = &img;
            b.descriptorCount = 1;
        }
        std::cerr << "Updating: " << dirty.size() << " descriptors" << std::endl;

        vkUpdateDescriptorSets(device, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
        dirty.clear();
    }
};



class BindlessTextureManager
{
public:

    struct CreateInfo
    {
        VkInstance       instance;
        VkPhysicalDevice physicalDevice;
        VkDevice         device;
        VkQueue          graphicsQueue; // the queue used for submitting commands to
        VmaAllocator     allocator = nullptr; // can be left null, will create

        //
        uint32_t totalTexture2D = 16;

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
        m_commandPool = _createCommandPool();
        init(m_createInfo.totalTexture2D);
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
        while(m_images.size())
        {
            destroyImage(m_images.back());
            m_images.pop_back();
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
    /**
     * @brief allocateTexture
     * @param extent
     * @param format
     * @return
     *
     * Allocate a texture in the manager. Once a texture is allocated
     * it is not destroyed unless you:
     */
    idTexture2D allocateTexture(VkExtent2D extent, VkFormat format = VK_FORMAT_R8G8B8A8_UNORM, uint32_t mipMaps = 0)
    {
        int32_t j=0;

        // Loop through all the free images.
        // These are images which were no longer needed, but are still
        // allocated. They can be reused.
        // So find one that fits the dimensions/format/etc
        // and return that one. none exist, then
        // we will have to create a new texture
        for(auto & i : m_freeImages)
        {
            auto & info = m_images[i].info;
            if( info.extent.width == extent.width &&
                info.extent.height == extent.height &&
                info.extent.depth == 1 &&
                info.arrayLayers == 1 &&
                info.format == format)
            {
                return {j};
            }
            ++j;
        }
        auto img = image_Create(extent.width,extent.height,1, format, VK_IMAGE_VIEW_TYPE_2D, 1, mipMaps, {});
        m_images.push_back(img);
        setDirty(m_images.size()-1);
        return { static_cast<int32_t>(m_images.size()-1) };
    }

    /**
     * @brief freeTexture
     * @param id
     *
     * Frees a texture, this does not automatically "free memory",
     * it keeps the texture in a memory pool. the next time you
     * allocate a texturte, it will reuse the same one if the dimensions
     * are the same.
     */
    void freeTexture(idTexture2D id)
    {
        auto i = static_cast<size_t>(id.index);
        m_freeImages.push_back(i);
    }

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
            generateMipMaps(id,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }
        return id;
    }
    //=========================================================================================================================
    template<typename Img_t>
    void uploadImageData( idTexture2D dstId,
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
    void uploadImageData( idTexture2D dstId,
                          void const * srcData,
                          uint32_t     srcByteSize,
                          VkExtent2D   srcExtent,

                          VkRect2D     dstRect,
                          uint32_t     dstLayer,
                          uint32_t     dstMip)
    {
        auto stg = _allocateStagingBuffer(srcData, srcByteSize);

        _beginCommandBuffer([=](auto cmd)
        {
            _transitionLayout(cmd, m_images[dstId.index].image,
                              VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                              dstLayer, 1,
                              dstMip  , 1);

            VkExtent3D _extent;
            _extent.depth  = 1;
            _extent.width  = std::min(m_images[dstId.index].info.extent.width  - dstRect.offset.x, dstRect.extent.width);
            _extent.height = std::min(m_images[dstId.index].info.extent.height - dstRect.offset.y, dstRect.extent.height);

            VkOffset3D _offset{ dstRect.offset.x, dstRect.offset.y, 0};

            _copyBufferToImage(cmd, stg, srcExtent.width,srcExtent.height, dstId, _offset, _extent, dstLayer, dstMip);

            _transitionLayout(cmd, m_images[dstId.index].image,
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
    void generateMipMaps(idTexture2D id, VkImageLayout mip0InitialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        VkFilter filter = VK_FILTER_LINEAR;

        VkImageBlit region;
        region.srcSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        region.srcSubresource.baseArrayLayer = 0;
        region.srcSubresource.layerCount     = 1;
        region.srcSubresource.mipLevel       = 0;

        region.dstSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        region.dstSubresource.baseArrayLayer = 0;
        region.dstSubresource.layerCount     = 1;
        region.dstSubresource.mipLevel       = 0;

        region.srcOffsets[0] = {0,0,0};
        region.dstOffsets[0] = {0,0,0};

        auto ext = m_images[id.index].info.extent;

        region.srcOffsets[1] = {static_cast<int32_t>(ext.width),static_cast<int32_t>(ext.height),1};
        region.dstOffsets[1] = {static_cast<int32_t>(ext.width),static_cast<int32_t>(ext.height),1};;

        uint32_t maxMip = getMipmapCount(id);

        auto image = m_images[id.index].image;

        _beginCommandBuffer([=](auto cmd)
        {
            // transition the first mip level into
            // source
            _image_TransitionLayout(cmd,
                                    image,
                                    mip0InitialLayout,
                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                    0, 1,
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
                                        0, 1,
                                        i, 1);

                // convert layer i-1 into transfer src
                // convert layer i into transfer dst
                vkCmdBlitImage(cmd, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &reg,filter);

                _image_TransitionLayout(cmd,
                                        image,
                                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                        0, 1,
                                        i, 1);
            }

            _image_TransitionLayout(cmd,
                                    image,
                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                    0, 1,
                                    0, maxMip);
        });

    }


    ImageInfo const& info(idTexture2D id) const
    {
        return m_images.at(id.index);
    }
    VkFormat getFormat(idTexture2D id) const
    {
        return m_images.at(id.index).info.format;
    }
    uint32_t formatSize(VkFormat f) const
    {
        assert( f == VK_FORMAT_R8G8B8A8_UNORM);
        return 4;
    }
    uint32_t getMipmapCount(idTexture2D id) const
    {
        return m_images.at(id.index).info.mipLevels;
    }
    uint32_t getLayerCount(idTexture2D id) const
    {
        return m_images.at(id.index).info.arrayLayers;
    }
    VkExtent3D getExtent(idTexture2D id) const
    {
        return m_images.at(id.index).info.extent;
    }




    /**
     * @brief update
     *
     * Updates all the dirty indices in the texture array.
     *
     * This should only be called ONCE per frame
     */
    void update()
    {
        if(m_images.size() == 0)
        {
            throw std::runtime_error("Cannot update images. Need to allocate at least 1 image");
        }
        m_dChain[ getCurrentChain() ].update(getDevice(), m_images);
        m_currentChain = (m_currentChain+1) % m_dChain.size();
    }
    void setDirty(size_t index)
    {
        for(auto & x : m_dChain)
        {
            x.dirty.push_back(index);
        }
    }
    size_t getCurrentChain() const
    {
        return m_currentChain;
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
protected:

    void init(uint32_t total2DTextures)
    {
        VkDescriptorSetLayoutCreateInfo ci = {};

        std::vector<VkDescriptorSetLayoutBinding> bindings;
        auto & L1 = bindings.emplace_back();

        L1.binding         = 0;
        L1.descriptorCount = total2DTextures;
        L1.stageFlags      = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        L1.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

        ci.sType           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        ci.bindingCount    = 1;
        ci.pBindings       = bindings.data();
        ci.flags           = {};

        VK_CHECK_RESULT( vkCreateDescriptorSetLayout(getDevice(), &ci, nullptr, & m_layout) );


        //=================
        uint32_t maxSets = 10;

        std::vector<VkDescriptorPoolSize> sizes;

        sizes.push_back( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, total2DTextures * maxSets * 1000});

        VkDescriptorPoolCreateInfo poolCI = {};
        poolCI.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolCI.maxSets       = maxSets;
        poolCI.pPoolSizes    = sizes.data();
        poolCI.poolSizeCount = static_cast<uint32_t>(sizes.size());

        VK_CHECK_RESULT( vkCreateDescriptorPool(getDevice(), &poolCI, nullptr, &m_descriptorPool) );

        //==================
        for(size_t i=0;i<2;i++)
        {
            VkDescriptorSetAllocateInfo allocInfo = {};
            allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool     = m_descriptorPool;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts        = &m_layout;

            VkDescriptorSet dSet;
            VK_CHECK_RESULT(vkAllocateDescriptorSets(getDevice(), &allocInfo, &dSet));

            m_dChain.emplace_back().m_descriptorSet = dSet;
        }

        for(size_t i=0;i<total2DTextures;i++)
        {
            setDirty(i);
        }
    }


    ImageInfo  image_Create( uint32_t width, uint32_t height, uint32_t depth
                             ,VkFormat format
                             ,VkImageViewType viewType
                             ,uint32_t arrayLayers
                             ,uint32_t miplevels // maximum mip levels
                             ,VkImageUsageFlags additionalUsageFlags
                                    )
    {
        VkImageCreateInfo imageInfo{};

        imageInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType     = depth==1 ? VK_IMAGE_TYPE_2D : VK_IMAGE_TYPE_3D;
        imageInfo.format        = format;
        imageInfo.extent        = VkExtent3D{width, height, depth};
        imageInfo.mipLevels     = miplevels;
        imageInfo.arrayLayers   = arrayLayers;

        imageInfo.samples       = VK_SAMPLE_COUNT_1_BIT;// vk::SampleCountFlagBits::e1;
        imageInfo.tiling        = VK_IMAGE_TILING_OPTIMAL;// vk::ImageTiling::eOptimal;
        imageInfo.usage         = additionalUsageFlags | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;// vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst;
        imageInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;// vk::SharingMode::eExclusive;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;// vk::ImageLayout::eUndefined;

        if( imageInfo.mipLevels == 0)
        {
            auto w = width;
            auto h = std::max(w,height);
            auto d = std::max(h,depth);
            imageInfo.mipLevels = 1 + static_cast<uint32_t>(std::floor(std::log2(d)));
            miplevels = imageInfo.mipLevels;
        }

        if( arrayLayers == 6)
            imageInfo.flags |=  VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;// vk::ImageCreateFlagBits::eCubeCompatible;

        VmaAllocationCreateInfo allocCInfo = {};
        allocCInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

        VkImage           image;
        VmaAllocation     allocation;
        VmaAllocationInfo allocInfo;

        VkImageCreateInfo & imageInfo_c = imageInfo;
        vmaCreateImage(getAllocator(),  &imageInfo_c,  &allocCInfo, &image, &allocation, &allocInfo);

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
            ci.maxLod                  =  static_cast<float>(miplevels) ;
            ci.borderColor             =  VK_BORDER_COLOR_INT_OPAQUE_BLACK;// vk::BorderColor::eIntOpaqueBlack ;
            ci.unnormalizedCoordinates =  VK_FALSE ;

            ci.magFilter               =  VK_FILTER_LINEAR;//vk::Filter::eLinear;
            ci.minFilter               =  VK_FILTER_LINEAR;//vk::Filter::eLinear;

            vkCreateSampler(getDevice(), &ci, nullptr, &I.sampler.linear);

            ci.magFilter               =  VK_FILTER_NEAREST;//vk::Filter::eNearest;
            ci.minFilter               =  VK_FILTER_NEAREST;//vk::Filter::eNearest;

            vkCreateSampler(getDevice(), &ci, nullptr, &I.sampler.nearest);
        }

        return I;
    }

    void _copyBufferToImage( VkCommandBuffer cmd,
                            _buffer srcBuffer,
                            uint32_t srcImageWidth,
                            uint32_t srcImageHeight,
                            idTexture2D dstId,
                            VkOffset3D dstOffset,
                            VkExtent3D dstExtent,
                            uint32_t   dstArrayBaseLayer,
                            uint32_t   dstMipBaseLevel
                          )
    {
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
            m_images[dstId.index].image,
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

    _buffer _allocateBuffer(uint32_t size, VkBufferUsageFlags usage, VmaMemoryUsage memUsage)
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
        vkDestroyImageView(getDevice(), I.imageView, nullptr);

        vkDestroySampler(getDevice(), I.sampler.linear, nullptr);
        vkDestroySampler(getDevice(), I.sampler.nearest, nullptr);

        I.sampler.linear = I.sampler.nearest = VK_NULL_HANDLE;
        vmaDestroyImage(getAllocator(), I.image, I.allocation);
        I.allocation = nullptr;
        I.image      = VK_NULL_HANDLE;
        I.imageView  = VK_NULL_HANDLE;
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
    VmaAllocator getAllocator()
    {
        return m_createInfo.allocator;
    }
    VkDescriptorSetLayout m_layout;
    VkDescriptorPool      m_descriptorPool;
    VkCommandPool         m_commandPool;

    std::vector<ImageInfo>       m_images;
    std::vector<size_t>          m_freeImages;
    std::vector<DescriptorChain> m_dChain;
    CreateInfo                   m_createInfo;
    bool                         m_selfManagedAllocator = false;
    size_t                       m_currentChain         = 0;
};

}
#endif
