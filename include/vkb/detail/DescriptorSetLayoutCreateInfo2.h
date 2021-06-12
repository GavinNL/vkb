#ifndef VKB_DESCRIPTORSETLAYOUTCREATEINFO2_H
#define VKB_DESCRIPTORSETLAYOUTCREATEINFO2_H

#include <vulkan/vulkan.hpp>
#include <functional>
#include <any>
#include "HashFunctions.h"


namespace vkb
{

struct DescriptorSetLayoutBinding2
{
    uint32_t                 binding;
    vk::DescriptorType       descriptorType;
    uint32_t                 descriptorCount;
    vk::ShaderStageFlags     stageFlags;
    std::vector<vk::Sampler> immutableSampler;


    DescriptorSetLayoutBinding2(uint32_t binding_ = {},
                                VULKAN_HPP_NAMESPACE::DescriptorType descriptorType_ = VULKAN_HPP_NAMESPACE::DescriptorType::eSampler,
                                uint32_t descriptorCount_ = {},
                                VULKAN_HPP_NAMESPACE::ShaderStageFlags stageFlags_ = {},
                                const std::vector<vk::Sampler> pImmutableSamplers_ = {}) VULKAN_HPP_NOEXCEPT
    : binding( binding_ ),
      descriptorType( descriptorType_ ),
      descriptorCount( descriptorCount_ ),
      stageFlags( stageFlags_ ),
      immutableSampler( pImmutableSamplers_ )
    {}
};

struct DescriptorSetLayoutCreateInfo2
{
    using object_type           = vk::DescriptorSetLayout;
    using base_create_info_type = vk::DescriptorSetLayoutCreateInfo;


    vk::DescriptorSetLayoutCreateFlags          flags;
    std::vector<DescriptorSetLayoutBinding2> bindings;

    template<typename Callable_t>
    object_type create_t(Callable_t && CC) const
    {
        vk::DescriptorSetLayoutCreateInfo D;

        std::vector<vk::DescriptorSetLayoutBinding> _bindings;
        for(auto & b: bindings)
        {
            auto & N = _bindings.emplace_back();
            N.binding = b.binding;
            N.stageFlags = b.stageFlags;
            N.descriptorCount = b.descriptorCount;
            N.descriptorType = b.descriptorType;
            N.pImmutableSamplers = b.immutableSampler.data();
        }

        D.bindingCount = static_cast<uint32_t>(bindings.size());
        D.pBindings    = _bindings.data();
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
        bindings.emplace_back( DescriptorSetLayoutBinding2(binding, type, count, stageFlags, {}));
        return *this;
    }
    DescriptorSetLayoutCreateInfo2& addDescriptor( uint32_t binding,vk::DescriptorType type, uint32_t count, vk::ShaderStageFlags stageFlags, std::vector<vk::Sampler> immutableSamplers)
    {
        auto & a = m_tempStorage.emplace_back() = immutableSamplers;
        auto & v = *std::any_cast< std::vector<vk::Sampler> >(&a);
        bindings.emplace_back( DescriptorSetLayoutBinding2(binding, type, count, stageFlags, v));
        return *this;
    }

    std::vector< std::any> m_tempStorage;
};

}

#endif
