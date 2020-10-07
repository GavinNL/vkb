#ifndef VKJSON_DESCRIPTORSETLAYOUTCREATEINFO2_H
#define VKJSON_DESCRIPTORSETLAYOUTCREATEINFO2_H

#include <vulkan/vulkan.hpp>
#include <functional>
#include "HashFunctions.h"


namespace vkb
{

struct DescriptorSetLayoutCreateInfo2
{
    vk::DescriptorSetLayoutCreateFlags          flags;
    std::vector<vk::DescriptorSetLayoutBinding> bindings;

    template<typename Callable_t>
    vk::DescriptorSetLayout create_t(Callable_t && CC) const
    {
        vk::DescriptorSetLayoutCreateInfo D;
        D.bindingCount = bindings.size();
        D.pBindings    = bindings.data();
        D.flags        = flags;

        return CC(D);
    }



    /**
     * @brief create
     * @param d
     * @return
     *
     * Create the descriptor set layout
     */
    vk::DescriptorSetLayout create(vk::Device d) const
    {
        return create_t( [d](vk::DescriptorSetLayoutCreateInfo & C)
        {
            return d.createDescriptorSetLayout(C);
        });
    }

    /**
     * @brief create
     * @param S
     * @return
     *
     * Create the descriptor set layout, but only if a similar layout
     * doesnt' already exist in storage.
     */
    vk::DescriptorSetLayout create(Storage & S, vk::Device device) const
    {
        auto h = hash();
        auto f = S.descriptorSetLayouts.find(h);
        if( f == S.descriptorSetLayouts.end())
        {
            auto l = create(device);
            S.descriptorSetLayouts[h] = l;
            return l;
        }
        else
        {
            return f->second;
        }
    }

    size_t hash() const
    {
        std::hash<size_t> H;
        size_t seed = hash_f( flags );
        for(auto & b : bindings)
        {
            hash_c(seed, H(b.binding) );
            hash_c(seed, vkb::hash_f(b.stageFlags)  );
            hash_c(seed, vkb::hash_e(b.descriptorType ) );
            hash_c(seed, H(b.descriptorCount) );
        }
        return seed;
    }

    //============================================================
    // Helper functions
    //============================================================
    DescriptorSetLayoutCreateInfo2& addDescriptor( uint32_t binding,vk::DescriptorType type, uint32_t count, vk::ShaderStageFlags stageFlags)
    {
        bindings.emplace_back( vk::DescriptorSetLayoutBinding(binding, type, count, stageFlags, nullptr));
        return *this;
    }


};

}

#endif
