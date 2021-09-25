#include "catch.hpp"
#include <fstream>

#include "unit_helpers.h"


SCENARIO( " Scenario 1: Create a DescriptorSetLayout" )
{
    // create a default window and initialize all vulkan
    // objects.
    auto window = createWindow(1024,768);

    // resize the framegraph to the size of the
    // swapchain. This will allocate any internal
    // images which depend on the size of the swapchain (eg: gBuffers)

    auto device = vk::Device(window->getDevice());
    auto physicalDevice = vk::PhysicalDevice(window->getPhysicalDevice());

    vkb::Storage S;


    vkb::BufferCreateInfo2 ci;
    ci.usage = vk::BufferUsageFlagBits::eVertexBuffer;
    ci.size  = 2048;

    auto buffer = ci.create(S, device);

    REQUIRE( buffer != vk::Buffer() ) ;
    {
        auto & obi = S.getCreateInfo<vkb::BufferCreateInfo2>(buffer);
        REQUIRE( obi.usage == ci.usage);
        REQUIRE( obi.size  == ci.size);
    }

    vkb::MemoryAllocInfo2 ma;
    ma.bufferOrImage = buffer;
    ma.flags = vk::MemoryPropertyFlagBits::eHostVisible;

    auto mem = ma.create(S, device, physicalDevice);
    REQUIRE( mem != vk::DeviceMemory());
    {
        auto v  = S.mapMemory(mem, device);
        auto v2 = S.mapMemory(mem, device);
        REQUIRE( v  != nullptr);
        REQUIRE( v2 != nullptr);
        REQUIRE( v == v2);
    }
    {
        auto & oci = S.getCreateInfo<vkb::MemoryAllocInfo2>(mem);
        REQUIRE( std::get<vk::Buffer>(oci.bufferOrImage) == buffer );
        REQUIRE( oci.flags == ma.flags);
    }

    device.bindBufferMemory(buffer, mem, 0);
    S.destroy(buffer, device);
    S.destroy(mem, device);

    S.destroyAll( window->getDevice());

    window->destroy();
    window.reset();

    SDL_Quit();

}


