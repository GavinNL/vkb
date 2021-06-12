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
#include <vkb/utils/DynamicPipeline.h>
#include <vkb/utils/DescriptorSetAllocator.h>

using namespace vkw;

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

    vkb::Storage S;


    vkb::DescriptorSetAllocator dSetAllocator;

    vkb::DescriptorPoolCreateInfo2 basicPool;
    basicPool.setMaxSets(10)
         .setPoolSize(vk::DescriptorType::eCombinedImageSampler, 10)
         .setPoolSize(vk::DescriptorType::eUniformBuffer, 10)
         .setFlags(   vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet
                    | vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind);

    dSetAllocator.init(&S, window->getDevice(), basicPool );


    // Allocate 7 combined image samplers
    auto set = dSetAllocator.allocateDescriptorSet( {{ 0, vk::DescriptorType::eCombinedImageSampler, 7, vk::ShaderStageFlagBits::eFragment, {}}});

    auto parentPool = dSetAllocator.getParentPool(set);
    {
        auto & poolInfo = dSetAllocator.getPoolInfo(parentPool);

        // there are 9 sets remaining
        REQUIRE( poolInfo.remainingSets == 9);

        // and 3 combined image samplers
        REQUIRE( poolInfo.remaining.at(vk::DescriptorType::eCombinedImageSampler) == 3);
    }

    // free the set
    dSetAllocator.freeDescriptorSet(set);

    {

        auto & poolInfo = dSetAllocator.getPoolInfo(parentPool);

        // we are now back to 10 sets remaining
        REQUIRE( poolInfo.remainingSets == 10);

        // and 10 combined image samplers
        REQUIRE( poolInfo.remaining.at(vk::DescriptorType::eCombinedImageSampler) == 10);
    }


    // allocate two sets
    {
        auto set1 = dSetAllocator.allocateDescriptorSet( {
                                                             { 0, vk::DescriptorType::eCombinedImageSampler, 4, vk::ShaderStageFlagBits::eFragment, {}},
                                                             { 1, vk::DescriptorType::eCombinedImageSampler, 3, vk::ShaderStageFlagBits::eVertex  , {}}
                                                         });

        auto set2 = dSetAllocator.allocateDescriptorSet( {{ 0, vk::DescriptorType::eCombinedImageSampler, 7, vk::ShaderStageFlagBits::eFragment, {}}});

        // they come from different pools because
        // each pool only has 10 combined image samplers
        REQUIRE( dSetAllocator.getParentPool(set1) != dSetAllocator.getParentPool(set2) );
    }

    // destroy the pools
    dSetAllocator.destroy();


    S.destroyAll(window->getDevice());

    delete window;
    SDL_Quit();

}

