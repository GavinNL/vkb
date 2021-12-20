#ifndef GVK_DESCRIPTOR_SET_MANAGER_H
#define GVK_DESCRIPTOR_SET_MANAGER_H

#include <memory>
#include <iostream>
#include <cassert>
#include <vector>
#include <vulkan/vulkan.h>
#include <map>
#include <unordered_map>

#include <spdlog/spdlog.h>

namespace gvk
{

struct DescriptorLayoutInfo
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    VkDescriptorSetLayoutCreateFlags          flags;

    size_t hash() const
    {
        std::hash<size_t> Hs;
        auto h = Hs(bindings.size());

        auto hash_combine = [](std::size_t& seed, const auto& v)
        {
            std::hash<std::decay_t<decltype(v)> > hasher;
            seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        };

        for(auto & b : bindings)
        {
            hash_combine(h, b.descriptorCount);
            hash_combine(h, static_cast<uint32_t>(b.descriptorType));
            hash_combine(h, static_cast<uint32_t>(b.stageFlags));
        }
        hash_combine(h, static_cast<uint32_t>(flags));
        return h;
    }

    bool operator==(DescriptorLayoutInfo const & B) const
    {
        if(bindings.size() == B.bindings.size())
        {
            return std::equal(bindings.begin(),bindings.end(),B.bindings.begin(),
                       [](VkDescriptorSetLayoutBinding const & a, VkDescriptorSetLayoutBinding const & b)
            {
                return std::tie(a.binding,a.descriptorCount,a.descriptorType,a.stageFlags,a.pImmutableSamplers) ==
                        std::tie(b.binding,b.descriptorCount,b.descriptorType,b.stageFlags,b.pImmutableSamplers);
            });
        }
        return true;
    }

};


class DescriptorLayoutCache
{
    public:
        void init(VkDevice newDevice)
        {
            m_device = newDevice;
        }

        void destroy()
        {
            for(auto & l : m_layoutCache)
            {
                vkDestroyDescriptorSetLayout(m_device, l.second, nullptr);
            }
            m_layoutCache.clear();
        }

        /**
         * @brief createDescriptorSetLayout
         * @param info
         * @return
         *
         * Create a descriptor set from a VkDescriptorSetLayoutCreateInfo
         */
        VkDescriptorSetLayout createDescriptorSetLayout(VkDescriptorSetLayoutCreateInfo const & info)
        {
            DescriptorLayoutInfo DLI;
            DLI.flags = info.flags;
            for(uint32_t i=0;i<info.bindingCount;i++)
            {
                DLI.bindings.push_back(info.pBindings[i]);
            }
            //std::sort(DLI.bindings.begin(), DLI.bindings.end(),[](auto const & a,auto const & b)
            //{
            //    return a.binding < b.binding;
            //});
            return _createDescriptorSetLayout(info, DLI);
        }

        /**
         * @brief createDescriptorSetLayout
         * @param info
         * @return
         *
         * Create a descriptorSetLayout from the custom DescriptorLayoutInfo struct
         */
        VkDescriptorSetLayout createDescriptorSetLayout(DescriptorLayoutInfo const & info)
        {
            VkDescriptorSetLayoutCreateInfo ci = {};
            ci.sType                           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            ci.pBindings                       = info.bindings.data();
            ci.bindingCount                    = static_cast<uint32_t>(info.bindings.size());
            ci.flags                           = info.flags;

            return _createDescriptorSetLayout(ci,info);
        }

        DescriptorLayoutInfo const & getLayoutInfo(VkDescriptorSetLayout l)
        {
            for(auto & x : m_layoutCache)
            {
                if(x.second == l)
                {
                    return x.first;
                }
            }
            throw std::runtime_error("This descriptorSetLayout was not created in this cache");
        }

    private:
        VkDescriptorSetLayout _createDescriptorSetLayout(VkDescriptorSetLayoutCreateInfo const & ci, DescriptorLayoutInfo const & info)
        {
            auto it = m_layoutCache.find(info);
            if( it != m_layoutCache.end())
            {
                return it->second;
            }

            VkDescriptorSetLayout layout;
            auto result = vkCreateDescriptorSetLayout(m_device, &ci, nullptr, &layout);

            if(result == VK_SUCCESS)
            {
                m_layoutCache[info] = layout;
                return layout;
            }
            throw std::runtime_error("Could not create descriptor set layout");
        }

        struct DescriptorLayoutHash
        {
            std::size_t operator()(const DescriptorLayoutInfo& k) const{
                return k.hash();
            }
        };

        std::unordered_map<DescriptorLayoutInfo, VkDescriptorSetLayout, DescriptorLayoutHash> m_layoutCache;
        VkDevice m_device;
};


/**
 * @brief The DescriptorPoolQueue class
 *
 * A Descriptor Pool queue a queue of descriptor pools of limited sizes.
 * The pools can only be used to allocate a descriptor set of a specified layout,
 * which is set at initialization time.
 *
 */
class DescriptorPoolQueue
{
public:

    /**
     * @brief init
     * @param device
     * @param layout - the layout that this pool will use
     * @param cache
     *
     * Initialize the descriptor pool queue.
     */
    void init(VkDevice device,
              DescriptorLayoutCache & cache,
              VkDescriptorSetLayout layout,
              uint32_t maxSetsPerPool = 10
              )
    {
        m_device                = device;
        m_layout                = layout;

        m_createInfo.maxSets = maxSetsPerPool;

        auto & layoutInfo = cache.getLayoutInfo(layout);
        std::map<VkDescriptorType, uint32_t> sizeMap;
        for(auto x : layoutInfo.bindings)
        {
            sizeMap[x.descriptorType] += x.descriptorCount;
        }
        m_poolSizes.clear();
        for(auto & x : sizeMap)
        {
            auto &s           = m_poolSizes.emplace_back();
            s.descriptorCount = x.second * maxSetsPerPool;
            s.type            = x.first;
        }

        createNewPool();
    }

    /**
     * @brief destroy
     *
     * Destroy the DescriptorPool queue.
     */
    void destroy()
    {
        freePools();
        for(auto p : m_pools)
        {
            vkDestroyDescriptorPool(m_device, p, nullptr);
        }
        m_pools.clear();
    }

    /**
     * @brief releaseToPool
     * @param set
     *
     * Release a descriptor set back to the queue
     */
    void releaseToPool(VkDescriptorSet set)
    {
        auto p = m_setToPool.at(set);
        ++m_returnedSets[p];
        if( m_returnedSets[p] == m_createInfo.maxSets )
        {
            resetPool(p);
            spdlog::debug("Total Descriptors Returned: {}, DescriptorPool reset", m_createInfo.maxSets);
        }
    }

    VkDescriptorSet allocateDescriptorSet()
    {
        VkDescriptorSet set = VK_NULL_HANDLE;

        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

        for(auto p : m_pools)
        {
            allocInfo.descriptorPool     = p;
            allocInfo.pSetLayouts        = &m_layout;
            allocInfo.descriptorSetCount = 1;

            auto result = vkAllocateDescriptorSets(m_device, &allocInfo, &set);

            switch (result)
            {
                case VK_SUCCESS:
                    //all good, return
                    m_setToPool[set] = p;
                    return set;
                case VK_ERROR_FRAGMENTED_POOL:
                case VK_ERROR_OUT_OF_POOL_MEMORY:
                    //reallocate pool
                default:
                    //unrecoverable error
                    throw std::runtime_error("Unrecoverable error");
            }
        }

        createNewPool();
        return allocateDescriptorSet();
    }

protected:
    void freePools()
    {
        for(auto p : m_pools)
        {
            resetPool(p);
        }
    }

    void resetPool(VkDescriptorPool p)
    {
        vkResetDescriptorPool(m_device, p, {});
        m_returnedSets.erase(p);
    }

    VkDescriptorPool createNewPool()
    {
        m_createInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        m_createInfo.pPoolSizes    = m_poolSizes.data();
        m_createInfo.poolSizeCount = static_cast<uint32_t>(m_poolSizes.size());

        VkDescriptorPool pool;
        auto result = vkCreateDescriptorPool(m_device, &m_createInfo, nullptr, &pool);
        if( result != VK_SUCCESS)
        {
            throw std::runtime_error("Error creating Descriptor Pool");
        }
        m_returnedSets[pool] = 0;
        m_pools.push_back(pool);
        return pool;
    }

    VkDevice                          m_device;
    VkDescriptorSetLayout             m_layout;
    std::vector<VkDescriptorPool>     m_pools;
    std::vector<VkDescriptorPoolSize> m_poolSizes;
    std::unordered_map<VkDescriptorSet, VkDescriptorPool> m_setToPool;
    std::unordered_map<VkDescriptorPool, uint32_t> m_returnedSets;
    VkDescriptorPoolCreateInfo        m_createInfo = {};
};

class DescriptorSetAllocator
{
public:
    void init(VkDevice device)
    {
        m_device = device;
    }
    void destroy()
    {
        for(auto & x : m_poolQueue)
        {
            x.second.destroy();
        }
    }

    VkDescriptorSet allocate(VkDescriptorSetLayout layout, DescriptorLayoutCache & cache)
    {
        auto it = m_poolQueue.find(layout);
        if( it != m_poolQueue.end())
        {
            auto s = it->second.allocateDescriptorSet();
            m_setLayouts[s] = layout;
            return s;
        }

        auto & newQueue = m_poolQueue[layout];

        newQueue.init(m_device, cache, layout, 10);

        auto s = newQueue.allocateDescriptorSet();
        m_setLayouts[s] = layout;
        return s;
    }

    void releaseDescriptorSet(VkDescriptorSet s)
    {
        auto l = m_setLayouts.at(s);
        m_poolQueue.at(l).releaseToPool(s);
    }

    std::unordered_map<VkDescriptorSetLayout, DescriptorPoolQueue> m_poolQueue;
    std::unordered_map<VkDescriptorSet, VkDescriptorSetLayout> m_setLayouts;
    VkDevice m_device;

};

}


#endif
