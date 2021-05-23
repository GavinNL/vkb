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
    (void)e;

    {
        vkb::PipelineLayoutCreateInfo2 v;// = J.get< vkb::PipelineLayoutCreateInfo2 >();

        // set 0
        v.newDescriptorSet().addDescriptor(0, vk::DescriptorType::eCombinedImageSampler, 4, vk::ShaderStageFlagBits::eFragment);

        // set 1
        v.newDescriptorSet().addDescriptor(0, vk::DescriptorType::eStorageBuffer, 1, vk::ShaderStageFlagBits::eVertex);

        // push constants
        v.addPushConstantRange( vk::ShaderStageFlagBits::eFragment, 0, 64);

        REQUIRE( v.pushConstantRanges.size() == 1);
        REQUIRE( v.setLayoutsDescriptions.size() == 2);
        REQUIRE( v.setLayouts.size() == 0);

        REQUIRE( v.setLayoutsDescriptions[0].bindings.size() == 1);
        REQUIRE( v.setLayoutsDescriptions[0].bindings[0].binding         == 0);
        REQUIRE( v.setLayoutsDescriptions[0].bindings[0].descriptorType  == vk::DescriptorType::eCombinedImageSampler);
        REQUIRE( v.setLayoutsDescriptions[0].bindings[0].descriptorCount == 4);
        REQUIRE( v.setLayoutsDescriptions[0].bindings[0].stageFlags      == vk::ShaderStageFlagBits::eFragment);

        REQUIRE( v.setLayoutsDescriptions[1].bindings.size() == 1);
        REQUIRE( v.setLayoutsDescriptions[1].bindings[0].binding         == 0);
        REQUIRE( v.setLayoutsDescriptions[1].bindings[0].descriptorType  == vk::DescriptorType::eStorageBuffer);
        REQUIRE( v.setLayoutsDescriptions[1].bindings[0].descriptorCount == 1);
        REQUIRE( v.setLayoutsDescriptions[1].bindings[0].stageFlags      == vk::ShaderStageFlagBits::eVertex);
        vkb::Storage S;

        // Create the pool
        auto d  = v.create( S, window->getDevice() );
        auto d2 = v.create( S, window->getDevice() );

        REQUIRE( d == d2);

        REQUIRE( d != vk::PipelineLayout() );

        auto & Ci = S.getCreateInfo<vkb::PipelineLayoutCreateInfo2>(d);

        REQUIRE( Ci.setLayouts.size() == v.setLayoutsDescriptions.size() );
        S.destroy(d, window->getDevice());

        S.destroyAll(window->getDevice());
    }


    delete window;
    SDL_Quit();

}

