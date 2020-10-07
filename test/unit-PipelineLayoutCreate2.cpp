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


    {
    auto jstr = R"foo(
    {
        "pushConstantRanges":
        [
            {
                "size" : 64,
                "offset" : 0,
                "stageFlags" : ["Fragment"]
            }
        ],
        "setLayouts":
            [
                {
                    "bindings" : [
                        {
                            "binding" : 0,
                            "descriptorType" : "CombinedImageSampler",
                            "descriptorCount" : 4,
                            "stageFlags" : ["Fragment"]
                        }
                    ]
                },
                {
                    "bindings" : [
                        {
                            "binding" : 0,
                            "descriptorType" : "StorageBuffer",
                            "descriptorCount" : 1,
                            "stageFlags" : ["Vertex"]
                        }
                    ]
                }
            ]

    }
    )foo";

        auto J = nlohmann::json::parse(jstr);

        vkb::PipelineLayoutCreateInfo2 v = J.get< vkb::PipelineLayoutCreateInfo2 >();

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


        S.destroy(d, window->getDevice());

        S.destroyAll(window->getDevice());
    }


    delete window;
    SDL_Quit();

}

