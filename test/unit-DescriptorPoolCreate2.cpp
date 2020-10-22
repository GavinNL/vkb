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
    auto e = window->getSwapchainExtent();

    vkb::Storage S;

    vkb::DescriptorPoolCreateInfo2 pC;
    pC.setPoolSize( vk::DescriptorType::eCombinedImageSampler, 10);
    pC.setPoolSize( vk::DescriptorType::eUniformBuffer, 10);
    pC.setMaxSets(10);

    auto dPool = pC.create(S, window->getDevice());

    REQUIRE( dPool != vk::DescriptorPool() );

    {
        auto & ci = S.getCreateInfo<vkb::DescriptorPoolCreateInfo2>(dPool);

        REQUIRE( ci.hash() == pC.hash() );
    }

    {
        vkb::DescriptorSetUpdater bb;
        bb.updateTexelBufferDescriptor( {},{},{}, vk::DescriptorType::eStorageTexelBuffer, { vk::BufferView(), vk::BufferView(), vk::BufferView()});
    }
    {
        vkb::DescriptorSetLayoutCreateInfo2 v;

        v.addDescriptor(0, vk::DescriptorType::eCombinedImageSampler, 4, vk::ShaderStageFlagBits::eFragment);
        REQUIRE( v.bindings.size() == 1);
        REQUIRE( v.bindings[0].binding         == 0);
        REQUIRE( v.bindings[0].descriptorType  == vk::DescriptorType::eCombinedImageSampler);
        REQUIRE( v.bindings[0].descriptorCount == 4);
        REQUIRE( v.bindings[0].stageFlags      == vk::ShaderStageFlagBits::eFragment);

        v.flags = {};




        auto dSet = v.allocateFromPool(S, dPool, window->getDevice());

        REQUIRE( dSet != vk::DescriptorSet()) ;

    }
    S.destroyAll( window->getDevice());


    vk::Device(window->getDevice()).destroyDescriptorPool( dPool );
    delete window;
    SDL_Quit();

}


