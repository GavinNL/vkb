#ifndef VKJSON_DESCRIPTORPOOLCREATEINFO2_H
#define VKJSON_DESCRIPTORPOOLCREATEINFO2_H

#include <vulkan/vulkan.hpp>
#include <functional>
#include "HashFunctions.h"


namespace vkb
{

struct DescriptorPoolCreateInfo2
{
    using object_type           = vk::DescriptorPool;
    using base_create_info_type = vk::DescriptorPoolCreateInfo;

    uint32_t                            maxSets;
    std::vector<vk::DescriptorPoolSize> sizes;
    vk::DescriptorPoolCreateFlags       flags;

    template<typename Callable_t>
    object_type create_t(Callable_t && CC) const
    {
        base_create_info_type D;
        D.pPoolSizes    = sizes.data();
        D.poolSizeCount = static_cast<uint32_t>(sizes.size());
        D.maxSets       = maxSets;
        D.flags         = flags;

        return CC(D);
    }

    /**
     * @brief create
     * @param d
     * @return
     *
     * Create the descriptor set layout
     */
    object_type create(vk::Device d) const
    {
        return create_t( [d](base_create_info_type & C)
        {
            return d.createDescriptorPool(C);
        });
    }

    size_t hash() const
    {
        std::hash<uint32_t> H;
        size_t seed = H(maxSets);

        hash_c(seed, hash_f(flags) );
        for(auto & b : sizes)
        {
            hash_c(seed, vkb::hash_e(b.type)  );
            hash_c(seed, H(b.descriptorCount)  );
        }
        return seed;
    }

    DescriptorPoolCreateInfo2& setMaxSets(uint32_t maxsets)
    {
        maxSets = maxsets;
        return *this;
    }
    DescriptorPoolCreateInfo2& setFlags(vk::DescriptorPoolCreateFlags f)
    {
        flags = f;
        return *this;
    }
    DescriptorPoolCreateInfo2& setPoolSize(vk::DescriptorType tpe, uint32_t count)
    {
        sizes.emplace_back(tpe, count);
        return *this;
    }

};

}

#endif
