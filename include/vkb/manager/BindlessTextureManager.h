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

    bool createAllocator(VkInstance instance,
                         VkPhysicalDevice physicalDevice,
                         VkDevice device)
    {
        m_allocatorInfo.physicalDevice   = physicalDevice;
        m_allocatorInfo.device           = device;
        m_allocatorInfo.instance         = instance;
        m_allocatorInfo.vulkanApiVersion = 0;

        auto result = vmaCreateAllocator(&m_allocatorInfo, &m_allocator);

        if( result != VK_SUCCESS)
        {
            throw std::runtime_error("Error creator allocator");
        }
        m_selfManagedAllocator = true;
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

        // destroy the descriptor pool
        vkDestroyDescriptorPool(getDevice(), m_descriptorPool,nullptr);

        // destroy the layout
        vkDestroyDescriptorSetLayout(getDevice(), m_layout, nullptr);
        m_layout = VK_NULL_HANDLE;

        if( m_allocator && m_selfManagedAllocator)
        {
            vmaDestroyAllocator(m_allocator);
            m_allocator = nullptr;
            m_selfManagedAllocator = false;
        }
    }

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


    idTexture2D allocateTexture(VkExtent2D extent, VkFormat format = VK_FORMAT_R8G8B8A8_UNORM)
    {
        int32_t j=0;
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
        auto img = image_Create(extent.width,extent.height,1, format, VK_IMAGE_VIEW_TYPE_2D, 1, 0, {});
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
protected:
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

        if( imageInfo.mipLevels == 0)
        {
            auto w = width;
            auto h = std::max(w,height);
            auto d = std::max(h,depth);
            imageInfo.mipLevels = 1 + static_cast<uint32_t>(std::floor(std::log2(d)));
            miplevels = imageInfo.mipLevels;
        }

        imageInfo.samples       = VK_SAMPLE_COUNT_1_BIT;// vk::SampleCountFlagBits::e1;
        imageInfo.tiling        = VK_IMAGE_TILING_OPTIMAL;// vk::ImageTiling::eOptimal;
        imageInfo.usage         = additionalUsageFlags | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;// vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst;
        imageInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;// vk::SharingMode::eExclusive;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;// vk::ImageLayout::eUndefined;

        if( arrayLayers == 6)
            imageInfo.flags |=  VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;// vk::ImageCreateFlagBits::eCubeCompatible;

        VmaAllocationCreateInfo allocCInfo = {};
        allocCInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

        VkImage           image;
        VmaAllocation     allocation;
        VmaAllocationInfo allocInfo;

        VkImageCreateInfo & imageInfo_c = imageInfo;
        vmaCreateImage(m_allocator,  &imageInfo_c,  &allocCInfo, &image, &allocation, &allocInfo);

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


    void destroyImage(ImageInfo & I)
    {
        vkDestroyImageView(getDevice(), I.imageView, nullptr);

        vkDestroySampler(getDevice(), I.sampler.linear, nullptr);
        vkDestroySampler(getDevice(), I.sampler.nearest, nullptr);

        I.sampler.linear = I.sampler.nearest = VK_NULL_HANDLE;
        vmaDestroyImage(m_allocator, I.image, I.allocation);
        I.allocation = nullptr;
        I.image      = VK_NULL_HANDLE;
        I.imageView  = VK_NULL_HANDLE;
    }

    VkDevice getDevice() const
    {
        // destroy the layout
        return m_allocatorInfo.device;
    }
    VkDescriptorSetLayout m_layout;
    VkDescriptorPool      m_descriptorPool;


    std::vector<ImageInfo>       m_images;
    std::vector<size_t>          m_freeImages;
    std::vector<DescriptorChain> m_dChain;
    VmaAllocatorCreateInfo       m_allocatorInfo;
    VmaAllocator                 m_allocator = nullptr;
    bool                         m_selfManagedAllocator=false;
    size_t m_currentChain = 0;
};

}
#endif
