#ifndef VKB_BUFFERCREATEINFO2_H
#define VKB_BUFFERCREATEINFO2_H

#include <vulkan/vulkan.hpp>
#include <functional>
#include <variant>

#include "HashFunctions.h"
#include "Storage.h"
#include "MemoryAlloc.h"

namespace vkb
{

struct BufferCreateInfo2
{
    using object_type           = vk::Buffer;
    using base_create_info_type = vk::BufferCreateInfo;

    vk::BufferUsageFlags  usage       = {};
    vk::DeviceSize        size        = 0;
    vk::SharingMode       sharingMode = vk::SharingMode::eExclusive;
    std::vector<uint32_t> queueFamilyIndices;

    template<typename Callable_t>
    object_type create_t( Callable_t && C) const
    {
        vk::BufferCreateInfo bc;

        bc.usage       = usage;
        bc.size        = size;
        bc.sharingMode = sharingMode;
        if( queueFamilyIndices.size())
        {
            bc.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
            bc.pQueueFamilyIndices = queueFamilyIndices.data();
        }

        return C(bc);
    }

    object_type create(vk::Device device) const
    {
        return create_t([device](vk::BufferCreateInfo & x)
        {
            return device.createBuffer(x);
        });
    }

    object_type create(Storage & S, vk::Device device) const
    {
        auto mem = create(device);

        S.storeCreateInfo(mem, *this);

        return mem;
    }
};


}

#endif
