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


    vkb::Storage S;


    vkb::GraphicsPipelineCreateInfo2 PCI;

    // Viewports
    PCI.viewportState.viewports.emplace_back( vk::Viewport(0,0,1024,768,0,1.0f));
    PCI.viewportState.scissors.emplace_back( vk::Rect2D( {0,0}, {1024,768}));

    // vertex inputs
    uint32_t stride=4+12+12;
    PCI.setVertexInputAttribute(0,0,vk::Format::eR8G8B8A8Unorm  ,0);
    PCI.setVertexInputAttribute(1,0,vk::Format::eR32G32B32Sfloat,4 );
    PCI.setVertexInputAttribute(2,0,vk::Format::eR32G32B32Sfloat,16);

    PCI.setVertexInputBinding(0,stride, vk::VertexInputRate::eVertex);
    PCI.setVertexInputBinding(1,stride, vk::VertexInputRate::eVertex);
    PCI.setVertexInputBinding(2,stride, vk::VertexInputRate::eVertex);

    PCI.inputAssemblyState.setTopology( vk::PrimitiveTopology::eTriangleList);

    // shader stages
    PCI.addStage( vk::ShaderStageFlagBits::eVertex, "main", CMAKE_SOURCE_DIR "/share/shaders/vert.spv");
    PCI.addStage( vk::ShaderStageFlagBits::eFragment, "main", CMAKE_SOURCE_DIR "/share/shaders/frag.spv");

    // Render Pass - we will initialize the pipeline by providing the RenderPassCreateInfo2 struct
    //               and have it auto generate the renderpass for us.
    PCI.renderPass = vkb::RenderPassCreateInfo2::createSimpleRenderPass({{ vk::Format(window->getSwapchainFormat()), vk::ImageLayout::ePresentSrcKHR}});

    // Descriptor sets/Push constants
    PCI.addPushConstantRange(vk::ShaderStageFlagBits::eVertex, 0, 128);

    // single attachment (Swapchain), use default values
    // setColorBlendOp and setBlendEnabled are just
    /// being added for show, these are teh default values
    PCI.addBlendStateAttachment().setColorBlendOp(vk::BlendOp::eAdd)
                                 .setBlendEnable(true);



    {
        auto pci = PCI;

        auto H = vkb::hash<vkb::GraphicsPipelineCreateInfo2>();

        pci.inputAssemblyState.setTopology(vk::PrimitiveTopology::eTriangleList);

        auto h1 = H(pci);

        pci.inputAssemblyState.setTopology(vk::PrimitiveTopology::eLineList);

        auto h2 = H(pci);

        pci.inputAssemblyState.setTopology(vk::PrimitiveTopology::eTriangleList);

        auto h3 = H(pci);

        REQUIRE( h1 != h2);
        REQUIRE( h1 == h3);
    }
    vkb::DynamicPipeline dP;

    // Initialize the dynamic pipeline with a particular state
    dP.init(&S, PCI, window->getDevice());

    dP.setTopology( vk::PrimitiveTopology::eTriangleList );

    REQUIRE( dP.pipelineCount() == 0);

    // get the current pipeline
    auto x = dP.get();

    REQUIRE( dP.pipelineCount() == 1);

    dP.setTopology( vk::PrimitiveTopology::eLineList );

    // get the new pipeline
    auto y = dP.get();

    REQUIRE( dP.pipelineCount() == 2);
    REQUIRE( std::get<0>(x) != std::get<0>(y) );

    // set the topology back
    dP.setTopology( vk::PrimitiveTopology::eTriangleList);

    auto z = dP.get();

    REQUIRE( dP.pipelineCount() == 2);

    REQUIRE( std::get<0>(x) == std::get<0>(z) );
    REQUIRE( dP.pipelineCount() == 2);


    {
        dP.disableVertexAttribute(2);

        auto q1 = dP.get();

        (void)q1;
        REQUIRE( dP.pipelineCount() == 3);

        dP.enableVertexAttribute(2, 0, vk::Format::eR32G32B32Sfloat, 16);

        auto q2 = dP.get();

        REQUIRE( dP.pipelineCount() == 3);
        REQUIRE( std::get<0>(q2) == std::get<0>(x) );
    }
    dP.destroy();

    REQUIRE( dP.pipelineCount() == 0);

    S.destroyAll(window->getDevice());

    delete window;
    SDL_Quit();

}

