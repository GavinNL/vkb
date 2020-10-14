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
std::vector<uint32_t> readSPV(std::string path)
{
    std::ifstream stream(path, std::ios::in | std::ios::binary);
    std::vector<char> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

    assert( contents.size() % sizeof(uint32_t) == 0);
    std::vector<uint32_t> code;
    code.resize( contents.size() / sizeof(uint32_t));
    std::memcpy( code.data(), contents.data(), contents.size());
    return code;
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


    // Resize the framegraph to the size of the
    // swapchain. This will allocate any internal
    // images which depend on the size of the swapchain (eg: gBuffers)
    auto e = window->getSwapchainExtent();


    vkb::Storage S;
    {

        vkb::ShaderModuleCreateInfo2 smci;
        smci.code = readSPV( CMAKE_SOURCE_DIR "/share/shaders/vert.spv" );

        auto module  = smci.create(S, window->getDevice());
        auto module2 = smci.create(S, window->getDevice());
        REQUIRE(module == module2);
        REQUIRE( module != vk::ShaderModule()) ;

        S.destroy(module, window->getDevice());
    }

    S.destroyAll(window->getDevice());

    delete window;
    SDL_Quit();

}

