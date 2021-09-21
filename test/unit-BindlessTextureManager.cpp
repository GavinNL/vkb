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
#include <vkb/manager/BindlessTextureManager.h>

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

    vkb::BindlessTextureManager::CreateInfo Ci;
    Ci.instance       = window->getInstance();
    Ci.physicalDevice = window->getPhysicalDevice();
    Ci.device         = window->getDevice();
    Ci.allocator      = nullptr;// create it for us
    Ci.graphicsQueue  = window->getGraphicsQueue();
    Ci.totalTexture2D = 32;
    vkb::BindlessTextureManager tManager;

    tManager.create(Ci);

    auto id = tManager.allocateTexture({256,256});
    uint8_t rawData[256*256*4];

    tManager.copyDataToImage(id, rawData,256*256*4, {256,256}, {{0,0},{256,256}},0,0);


    tManager.update();
    tManager.update();
    tManager.update();
    tManager.update();
    tManager.update();

    tManager.destroy();


    S.destroyAll(window->getDevice());

    delete window;
    SDL_Quit();

}

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
