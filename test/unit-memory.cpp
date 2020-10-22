#include "catch.hpp"
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <vkw/SDLVulkanWindow.h>
#include <vkw/SDLVulkanWindow_INIT.inl>
#include <vkw/SDLVulkanWindow_USAGE.inl>
using namespace vkw;

#include <vulkan/vulkan.hpp>

#include <vkb/vkb.h>


static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanReportFunc(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t obj,
    size_t location,
    int32_t code,
    const char* layerPrefix,
    const char* msg,
    void* userData)
{
    (void)obj;
    (void)flags;
    (void)objType;
    (void)location;
    (void)code;
    (void)userData;
    printf("VULKAN VALIDATION: [%s] %s\n", layerPrefix, msg);
    //throw std::runtime_error( msg );
    return VK_FALSE;
}


SCENARIO( " Scenario 1: Create a DescriptorSetLayout" )
{
    SDL_Init(SDL_INIT_EVERYTHING);
    auto window = new SDLVulkanWindow();

    // 1. create the window
    window->createWindow("Simple Deferred", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 1024,768);

    // 2. initialize the vulkan instance
    SDLVulkanWindow::InitilizationInfo info;
    info.callback = VulkanReportFunc;
    window->createVulkanInstance( info);

    // 3. Create the following objects:
    //    instance, physical device, device, graphics/present queues,
    //    swap chain, depth buffer, render pass and framebuffers
    window->initSurface(SDLVulkanWindow::SurfaceInitilizationInfo());


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

    delete window;
    SDL_Quit();

}


