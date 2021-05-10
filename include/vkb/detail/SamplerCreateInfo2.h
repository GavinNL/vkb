#ifndef VKB_SAMPLERCREATEINFO2_H
#define VKB_SAMPLERCREATEINFO2_H

#include <vulkan/vulkan.hpp>
#include <functional>
#include "HashFunctions.h"
#include "Storage.h"

namespace vkb
{

struct SamplerCreateInfo2 : public vk::SamplerCreateInfo
{
    using object_type           = vk::Sampler;
    using base_create_info_type = vk::SamplerCreateInfo;

    template<typename Callable_t>
    object_type create_t(Callable_t && CC) const
    {
        base_create_info_type const & D =  *this;
        return CC( D);
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
        return create_t( [d](base_create_info_type const & C)
        {
            return d.createSampler(C);
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
        auto f = S.samplers.find(h);
        if( f == S.samplers.end())
        {
            auto l = create(device);
            if( l )
                S.storeCreateInfo(l, *this);
            S.samplers[h] = l;
            return l;
        }
        else
        {
            return f->second;
        }
    }

    size_t hash() const
    {
        size_t seed = hash_pod( *this);
        return seed;
    }


};

}

#endif
