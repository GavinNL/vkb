#ifndef GVK_IMAGE_SAMPLER_MANAGER_H
#define GVK_IMAGE_SAMPLER_MANAGER_H

#include <memory>
#include <iostream>
#include <cassert>
#include <vector>
#include <vulkan/vulkan.h>
#include <map>
#include <unordered_map>

namespace gvk
{

struct SamplerInfo
{
    VkSamplerCreateFlags flags                   = {};
    VkFilter             magFilter               = VK_FILTER_LINEAR;
    VkFilter             minFilter               = VK_FILTER_LINEAR;
    VkSamplerMipmapMode  mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    VkSamplerAddressMode addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkSamplerAddressMode addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkSamplerAddressMode addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    float                mipLodBias              = 0.0f;
    VkBool32             anisotropyEnable        = VK_FALSE;
    float                maxAnisotropy           = 1;
    VkBool32             compareEnable           = VK_FALSE;
    VkCompareOp          compareOp               = VK_COMPARE_OP_ALWAYS;
    float                minLod                  = 0;
    float                maxLod                  = VK_LOD_CLAMP_NONE;
    VkBorderColor        borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    VkBool32             unnormalizedCoordinates = VK_FALSE;

    size_t hash() const
    {
        size_t h = 156485465u;

        auto hash_combine = [](std::size_t& seed, const auto& v)
        {
            using type = std::decay_t<decltype(v)>;
            if constexpr (std::is_enum_v<type>)
            {
                auto s = static_cast<size_t>(v);
                std::hash<size_t> hasher;
                seed ^= hasher(s) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            }
            else
            {
                std::hash<type> hasher;
                seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            }
        };

        hash_combine(h, flags);
        hash_combine(h, magFilter);
        hash_combine(h, minFilter);
        hash_combine(h, mipmapMode);
        hash_combine(h, addressModeU);
        hash_combine(h, addressModeV);
        hash_combine(h, addressModeW);
        hash_combine(h, mipLodBias);
        hash_combine(h, anisotropyEnable);
        hash_combine(h, maxAnisotropy);
        hash_combine(h, compareEnable);
        hash_combine(h, compareOp);
        hash_combine(h, minLod);
        hash_combine(h, maxLod);
        hash_combine(h, borderColor);
        hash_combine(h, unnormalizedCoordinates);

        return h;
    }

    bool operator==(SamplerInfo const & B) const
    {
        return
        flags                       == B.flags
        && magFilter                == B.magFilter
        && minFilter                == B.minFilter
        && mipmapMode               == B.mipmapMode
        && addressModeU             == B.addressModeU
        && addressModeV             == B.addressModeV
        && addressModeW             == B.addressModeW
        && mipLodBias               == B.mipLodBias
        && anisotropyEnable         == B.anisotropyEnable
        && maxAnisotropy            == B.maxAnisotropy
        && compareEnable            == B.compareEnable
        && compareOp                == B.compareOp
        && minLod                   == B.minLod
        && maxLod                   == B.maxLod
        && borderColor              == B.borderColor
        && unnormalizedCoordinates  == B.unnormalizedCoordinates;
    }

};


class ImageSamplerCache
{
    public:
        void init(VkDevice newDevice)
        {
            m_device = newDevice;
        }

        void destroy()
        {
            for(auto & l : m_samplerCache)
            {
                vkDestroySampler(m_device, l.second, nullptr);
            }
            m_samplerCache.clear();
        }

        /**
         * @brief createDescriptorSetLayout
         * @param info
         * @return
         *
         * Create a descriptor set from a VkDescriptorSetLayoutCreateInfo
         */
        VkSampler createSampler(VkSamplerCreateInfo const & info)
        {
            SamplerInfo DLI;
            assert(info.pNext == nullptr);
            DLI.flags                    = info.flags;
            DLI.magFilter                = info.magFilter;
            DLI.minFilter                = info.minFilter;
            DLI.mipmapMode               = info.mipmapMode;
            DLI.addressModeU             = info.addressModeU;
            DLI.addressModeV             = info.addressModeV;
            DLI.addressModeW             = info.addressModeW;
            DLI.mipLodBias               = info.mipLodBias;
            DLI.anisotropyEnable         = info.anisotropyEnable;
            DLI.maxAnisotropy            = info.maxAnisotropy;
            DLI.compareEnable            = info.compareEnable;
            DLI.compareOp                = info.compareOp;
            DLI.minLod                   = info.minLod;
            DLI.maxLod                   = info.maxLod;
            DLI.borderColor              = info.borderColor;
            DLI.unnormalizedCoordinates  = info.unnormalizedCoordinates;

            return _create(info, DLI);
        }

        /**
         * @brief createDescriptorSetLayout
         * @param info
         * @return
         *
         * Create a descriptorSetLayout from the custom DescriptorLayoutInfo struct
         */
        VkSampler createSampler(SamplerInfo const & info)
        {
            VkSamplerCreateInfo ci = {};
            ci.sType                    = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            ci.pNext                    = nullptr;
            ci.flags                    = info.flags;
            ci.magFilter                = info.magFilter;
            ci.minFilter                = info.minFilter;
            ci.mipmapMode               = info.mipmapMode;
            ci.addressModeU             = info.addressModeU;
            ci.addressModeV             = info.addressModeV;
            ci.addressModeW             = info.addressModeW;
            ci.mipLodBias               = info.mipLodBias;
            ci.anisotropyEnable         = info.anisotropyEnable;
            ci.maxAnisotropy            = info.maxAnisotropy;
            ci.compareEnable            = info.compareEnable;
            ci.compareOp                = info.compareOp;
            ci.minLod                   = info.minLod;
            ci.maxLod                   = info.maxLod;
            ci.borderColor              = info.borderColor;
            ci.unnormalizedCoordinates  = info.unnormalizedCoordinates;

            return _create(ci,info);
        }

        SamplerInfo const & getSamplerInfo(VkSampler l)
        {
            for(auto & x : m_samplerCache)
            {
                if(x.second == l)
                {
                    return x.first;
                }
            }
            throw std::runtime_error("This descriptorSetLayout was not created in this cache");
        }

    private:
        VkSampler _create(VkSamplerCreateInfo const & ci, SamplerInfo const & info)
        {
            auto it = m_samplerCache.find(info);
            if( it != m_samplerCache.end())
            {
                return it->second;
            }

            VkSampler samp;
            auto result = vkCreateSampler(m_device, &ci, nullptr, &samp);

            if(result == VK_SUCCESS)
            {
                m_samplerCache[info] = samp;
                return samp;
            }
            throw std::runtime_error("Could not create Sampler");
        }

        struct SamplerInfoHash
        {
            std::size_t operator()(const SamplerInfo& k) const{
                return k.hash();
            }
        };

        std::unordered_map<SamplerInfo, VkSampler, SamplerInfoHash> m_samplerCache;
        VkDevice m_device;
};

}


#endif
