#ifndef VKB_DESCRIPTOR_SET_ALLOCATOR_H
#define VKB_DESCRIPTOR_SET_ALLOCATOR_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <map>
#include <vkb/vkb.h>
#include <vkb/detail/Storage.h>

namespace vkb
{

struct DescriptorSetAllocator
{
    struct DescriptorPoolInfo
    {
        std::map<vk::DescriptorType, uint32_t> remaining;
        uint32_t remainingSets = 0;
    };
    struct DescriptorSetInfo
    {
        vk::DescriptorPool      parent;
        vk::DescriptorSetLayout layout;
    };

    struct DescriptorSetBinding
    {
        uint32_t           binding;
        vk::DescriptorType   type;
        uint32_t           count;
        vk::ShaderStageFlags stages;
        std::vector<vk::Sampler> immutableSamplers;
    };

    /**
     * @brief init
     * @param storage
     * @param device
     * @param defaultCI
     *
     * Initialize the descriptorSetAllocator. The defaultCI is the default create
     * info that will be used for all pools.
     */
    void init( vkb::Storage * storage, vk::Device device, vkb::DescriptorPoolCreateInfo2 defaultCI = defaultPoolCreateInfo())
    {
        m_storage = storage;
        m_Device  = device;
        m_defaultPoolCI = defaultCI;
    }

    /**
     * @brief allocateDescriptorSet
     * @param B
     * @return
     *
     * Allocate a descriptor set based on what bindings you need
     *
     * dSetAllocator.allocateDescriptorSet( {
     *                                        { 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4, VK_SHADER_STAGE_FRAGMENT_BIT},
     *                                        { 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 3, VK_SHADER_STAGE_VERTEX_BIT}
     *                                      });
     */
    vk::DescriptorSet allocateDescriptorSet( vk::ArrayProxy<DescriptorSetBinding const> B)
    {
        vk::DescriptorPool currentPool;

        {
            bool poolFound=false;

            // Loop through all the pools we currently have
            // created.
            for(auto & PP : m_PoolInfo)
            {
                auto & dInfo = PP.second;

                // check if this set has enough descriptors for each type
                if( dInfo.remainingSets > 0
                    && std::all_of(B.begin(), B.end(),[&](auto & K)
                    {
                        return dInfo.remaining.at(K.type) >= K.count;
                    })
                  )
                {
                    poolFound=true;
                    currentPool = PP.first;
                    break;
                }
            }

            // we haven't been able to find a pool
            // so we need to create a new one
            if( poolFound == false)
            {
                currentPool = createNewPool();
            }
        }


        auto & poolInfo = m_PoolInfo.at(currentPool);

        poolInfo.remainingSets--;
        vkb::DescriptorSetLayoutCreateInfo2 dL;
        for(auto & b : B)
        {
            dL.addDescriptor(b.binding, vk::DescriptorType(b.type) ,b.count, vk::ShaderStageFlags(b.stages), b.immutableSamplers);
            poolInfo.remaining[b.type] -= b.count;
        }

        auto layout = dL.create(*m_storage, m_Device);

        vk::DescriptorSetAllocateInfo i;
        i.setDescriptorSetCount(1)
         .setPSetLayouts(&layout)
         .setDescriptorPool(currentPool);

        auto outSet = m_Device.allocateDescriptorSets(i).front();

        {
            auto & setInfo = m_SetInfo[outSet];
            setInfo.layout = layout;
            setInfo.parent = currentPool;
        }

        return outSet;
    }

    /**
     * @brief freeDescriptorSet
     * @param set
     *
     * Free the descriptor set by returning it to the pool
     */
    void freeDescriptorSet(vk::DescriptorSet set)
    {
        auto & info = m_SetInfo.at(set);

        // free the set
        m_Device.freeDescriptorSets(info.parent, set);
        auto & pool = m_PoolInfo.at(info.parent);

        auto & CI = m_storage->getCreateInfo<vkb::DescriptorSetLayoutCreateInfo2>(info.layout);
        for(auto & x : CI.bindings)
        {
            pool.remaining[x.descriptorType] += x.descriptorCount;
        }
        pool.remainingSets++;
        m_SetInfo.erase(set);
    }


    /**
     * @brief destroy
     *
     * Destroy all descriptor pools
     */
    void destroy()
    {
        for(auto & xx : m_PoolInfo)
        {
            m_Device.destroyDescriptorPool(xx.first);
        }
        m_PoolInfo.clear();
        m_SetInfo.clear();
    }

    vk::DescriptorPool getParentPool(vk::DescriptorSet s) const
    {
        return m_SetInfo.at( s ).parent;
    }
    vk::DescriptorSetLayout getLayout(vk::DescriptorSet s) const
    {
        return m_SetInfo.at( s ).layout;
    }
    DescriptorPoolInfo const& getPoolInfo(vk::DescriptorPool p) const
    {
        return m_PoolInfo.at( p );
    }

    static vkb::DescriptorPoolCreateInfo2 defaultPoolCreateInfo()
    {
        vkb::DescriptorPoolCreateInfo2 dPool;

        dPool.setMaxSets(10)
             .setPoolSize(vk::DescriptorType::eCombinedImageSampler, 100)
             .setPoolSize(vk::DescriptorType::eUniformBuffer       , 100)
             .setPoolSize(vk::DescriptorType::eStorageBuffer       , 100)
             .setPoolSize(vk::DescriptorType::eStorageBufferDynamic, 100)
             .setPoolSize(vk::DescriptorType::eUniformBufferDynamic, 100)
             .setFlags(   vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet
                        | vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind);

        return dPool;
    }
protected:
    std::map< vk::DescriptorPool, DescriptorPoolInfo> m_PoolInfo;
    std::map< vk::DescriptorSet , DescriptorSetInfo>  m_SetInfo;
    vkb::Storage * m_storage = nullptr;
    vk::Device     m_Device;

    vkb::DescriptorPoolCreateInfo2 m_defaultPoolCI;

    vk::DescriptorPool createNewPool()
    {
        vkb::DescriptorPoolCreateInfo2 & dPool = m_defaultPoolCI;

        dPool.setMaxSets(10)
             .setPoolSize(vk::DescriptorType::eCombinedImageSampler, 10)
             .setPoolSize(vk::DescriptorType::eUniformBuffer, 10)
             .setFlags(   vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet
                        | vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind);

        auto outPool   = dPool.create(*m_storage, m_Device);

        auto & setInfo = m_PoolInfo[outPool];
        setInfo.remainingSets = dPool.maxSets;
        for(auto & x : dPool.sizes)
        {
            setInfo.remaining[ x.type ] = x.descriptorCount;
        }

        return outPool;
    }


};

}

#endif
