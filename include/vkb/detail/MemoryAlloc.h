#ifndef VKJSON_MEMORYALLOC_H
#define VKJSON_MEMORYALLOC_H

#include <vulkan/vulkan.hpp>
#include <functional>
#include <variant>

#include "HashFunctions.h"
#include "Storage.h"

namespace vkb
{

struct MemoryAllocInfo2
{
    using object_type           = vk::DeviceMemory;
    using base_create_info_type = vk::MemoryAllocateInfo;

    std::variant<vk::Buffer,vk::Image>  bufferOrImage;
    vk::MemoryPropertyFlags             flags;
    vk::DeviceSize                      size=0;// if 0, will be same size as buffer/image requirement
                                               // can be used to allocate additional memory

    // do not use this;
    void* _mapped = nullptr;

    template<typename Callable_t>
    object_type create_t(vk::Device         device,
                         vk::PhysicalDevice physicalDevice,
                         Callable_t      && C) const
    {

        auto bufferMemoryRequirements =
                std::holds_alternative<vk::Buffer>(bufferOrImage)
                    ? device.getBufferMemoryRequirements( std::get<vk::Buffer>(bufferOrImage) )
                    : device.getImageMemoryRequirements(  std::get<vk::Image>(bufferOrImage) );

        vk::MemoryAllocateInfo info;

        info.allocationSize  = std::max(bufferMemoryRequirements.size, size);
        info.memoryTypeIndex = findMemoryType(physicalDevice,
                                              bufferMemoryRequirements.memoryTypeBits,
                                              flags);

        return C(info);
    }

    object_type create(vk::Device device, vk::PhysicalDevice physicalDevice) const
    {
        return create_t(device, physicalDevice,
                        [device](vk::MemoryAllocateInfo & x)
        {
            return device.allocateMemory(x);
        });
    }

    object_type create(Storage & S, vk::Device device, vk::PhysicalDevice physicalDevice) const
    {
        auto cpy = *this;
        cpy._mapped = nullptr;
        auto mem = create_t(device, physicalDevice,
                        [device, &cpy](vk::MemoryAllocateInfo & x)
        {
            cpy.size = x.allocationSize;
            return device.allocateMemory(x);
        });

        S.storeCreateInfo(mem, cpy);

        return mem;
    }


protected:
    static uint32_t findMemoryType(vk::PhysicalDevice physicalDevice,
                                   uint32_t typeFilter,
                                   vk::MemoryPropertyFlags properties)
    {
        auto memProperites        = physicalDevice.getMemoryProperties();

        for (uint32_t i = 0; i < memProperites.memoryTypeCount ; i++)
        {
            const vk::MemoryPropertyFlags MemPropFlags = static_cast<vk::MemoryPropertyFlags>(memProperites.memoryTypes[i].propertyFlags);
            #define GET_BIT(a, j) (a & (1u << j))

            if ( GET_BIT(typeFilter,i) && ( MemPropFlags & properties) == properties)
            {
                return i;
            }
        }
        throw std::runtime_error("failed to find suitable memory type!");
    }
};

inline void *Storage::mapMemory(vk::DeviceMemory m, vk::Device device)
{
    auto & Mc = _getCreateInfo<vkb::MemoryAllocInfo2>(m);

    if( Mc._mapped) return Mc._mapped;

    auto v = device.mapMemory(m, 0, Mc.size);
    if( v!=nullptr)
        Mc._mapped = v;

    return Mc._mapped;
}

inline void Storage::unmapMemory(vk::DeviceMemory m, vk::Device device)
{
    auto & Mc = _getCreateInfo<vkb::MemoryAllocInfo2>(m);

    if( Mc._mapped == nullptr) return;

    device.unmapMemory(m);

    Mc._mapped = nullptr;
}

}

#endif
