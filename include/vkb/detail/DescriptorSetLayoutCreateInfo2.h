#ifndef VKB_DESCRIPTORSETLAYOUTCREATEINFO2_H
#define VKB_DESCRIPTORSETLAYOUTCREATEINFO2_H

#include <vulkan/vulkan.hpp>
#include <functional>
#include <any>
#include "HashFunctions.h"


namespace vkb
{

struct DescriptorSetLayoutCreateInfo2
{
    using object_type           = vk::DescriptorSetLayout;
    using base_create_info_type = vk::DescriptorSetLayoutCreateInfo;


    vk::DescriptorSetLayoutCreateFlags          flags;
    std::vector<vk::DescriptorSetLayoutBinding> bindings;

    template<typename Callable_t>
    object_type create_t(Callable_t && CC) const
    {
        vk::DescriptorSetLayoutCreateInfo D;
        D.bindingCount = static_cast<uint32_t>(bindings.size());
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
    object_type create(vk::Device d) const
    {
        return create_t( [d](base_create_info_type & C)
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
    object_type create(Storage & S, vk::Device device) const
    {
        auto h = hash();
        auto f = S.descriptorSetLayouts.find(h);
        if( f == S.descriptorSetLayouts.end())
        {
            auto l = create(device);
            if( l )
                S.storeCreateInfo(l, *this);
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


    /**
     * @brief allocateFromPool
     * @param S
     * @param pool
     * @param device
     * @return
     *
     * Allocate a descriptor set using this layout infromation. The layout will be
     * created in the storage are and reused if needed.
     */
    vk::DescriptorSet allocateFromPool(Storage & S, vk::DescriptorPool pool, vk::Device device)
    {
        auto layout = create(S, device);
        vk::DescriptorSetAllocateInfo i;
        i.setDescriptorSetCount(1)
         .setPSetLayouts(&layout)
         .setDescriptorPool(pool);

        auto sets = device.allocateDescriptorSets(i);
        return sets.front();

    }
    //============================================================
    // Helper functions
    //============================================================
    DescriptorSetLayoutCreateInfo2& addDescriptor( uint32_t binding,vk::DescriptorType type, uint32_t count, vk::ShaderStageFlags stageFlags)
    {
        bindings.emplace_back( vk::DescriptorSetLayoutBinding(binding, type, count, stageFlags, nullptr));
        return *this;
    }
    DescriptorSetLayoutCreateInfo2& addDescriptor( uint32_t binding,vk::DescriptorType type, uint32_t count, vk::ShaderStageFlags stageFlags, std::vector<vk::Sampler> immutableSamplers)
    {
        auto & a = m_tempStorage.emplace_back() = immutableSamplers;
        auto & v = *std::any_cast< std::vector<vk::Sampler> >(&a);
        bindings.emplace_back( vk::DescriptorSetLayoutBinding(binding, type, count, stageFlags, v.data()));
        return *this;
    }

    std::vector< std::any> m_tempStorage;
};

}

#endif
