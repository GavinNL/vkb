#include "catch.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <SDLVulkanWindow.h>

#include <vulkan/vulkan.hpp>

#include <vkb/vkb.h>
#include <vkb/serial/from_json.h>

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

    auto jstr = R"foo(
    {
        "code" : {
            "uri" : "test"
         },
         "name" : "main",
         "stage" : "Vertex"
    }
    )foo";

    vkb::Storage S;
    {

        auto J = nlohmann::json::parse(jstr);

        J["code"]["uri"] = CMAKE_SOURCE_DIR "/share/shaders/vert.spv";
        J["stage"] = "Vertex";

        auto ss = J.get<vkb::PipelineShaderStageCreateInfo2>();

        REQUIRE( ss.name == "main");
        REQUIRE( ss.stage == vk::ShaderStageFlagBits::eVertex);
        REQUIRE( ss.code.size() > 0);




        vkb::ShaderModuleCreateInfo2 smci;
        smci.code = ss.code;

        auto module = smci.create(S, window->getDevice());
        auto module2 = smci.create(S, window->getDevice());
        REQUIRE(module == module2);
        REQUIRE( module != vk::ShaderModule()) ;



        S.destroy(module, window->getDevice());
    }
    {

        auto J = nlohmann::json::parse(jstr);

        J["code"]["uri"] = CMAKE_SOURCE_DIR "/share/shaders/frag.spv";
        J["stage"] = "Fragment";

        auto ss = J.get<vkb::PipelineShaderStageCreateInfo2>();

        REQUIRE( ss.name == "main");
        REQUIRE( ss.stage == vk::ShaderStageFlagBits::eFragment);
        REQUIRE( ss.code.size() > 0);


        vkb::ShaderModuleCreateInfo2 smci;
        smci.code = ss.code;

        auto module = smci.create(S, window->getDevice());
        auto module2 = smci.create(S, window->getDevice());
        REQUIRE(module == module2);
        REQUIRE( module != vk::ShaderModule()) ;


        S.destroy(module, window->getDevice());

    }
    S.destroyAll(window->getDevice());

    delete window;
    SDL_Quit();

}

