#include "catch.hpp"
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <SDLVulkanWindow.h>

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
    auto e = window->getSwapchainExtent();


    {
        vkb::DescriptorSetLayoutCreateInfo2 v;

        v.addDescriptor(0, vk::DescriptorType::eCombinedImageSampler, 4, vk::ShaderStageFlagBits::eFragment);
        REQUIRE( v.bindings.size() == 1);
        REQUIRE( v.bindings[0].binding         == 0);
        REQUIRE( v.bindings[0].descriptorType  == vk::DescriptorType::eCombinedImageSampler);
        REQUIRE( v.bindings[0].descriptorCount == 4);
        REQUIRE( v.bindings[0].stageFlags      == vk::ShaderStageFlagBits::eFragment);

        v.flags = {};


        vkb::Storage S;

        // Create the pool
        auto d = v.create( S, window->getDevice() );
        auto d2 = v.create( S, window->getDevice() );

        REQUIRE( d == d2);

        REQUIRE( d != vk::DescriptorSetLayout() );


        S.destroy(d, window->getDevice());

        auto & Ci = S.getCreateInfo<vkb::DescriptorSetLayoutCreateInfo2>(d);

        REQUIRE( Ci.bindings.size()             == v.bindings.size()            );
        REQUIRE( Ci.bindings[0].binding         == v.bindings[0].binding        );
        REQUIRE( Ci.bindings[0].descriptorType  == v.bindings[0].descriptorType );
        REQUIRE( Ci.bindings[0].descriptorCount == v.bindings[0].descriptorCount);
        REQUIRE( Ci.bindings[0].stageFlags      == v.bindings[0].stageFlags     );

        REQUIRE( Ci.hash() == v.hash() );

    }


    delete window;
    SDL_Quit();

}

