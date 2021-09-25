#include "catch.hpp"
#include <fstream>

#include "unit_helpers.h"


SCENARIO( " Scenario 1: Create a DescriptorSetLayout" )
{
    // create a default window and initialize all vulkan
    // objects.
    auto window = createWindow(1024,768);

    // resize the framegraph to the size of the
    // swapchain. This will allocate any internal
    // images which depend on the size of the swapchain (eg: gBuffers)
    auto e = window->getSwapchainExtent();
    (void)e;

    vkb::Storage S;


    vkb::GraphicsPipelineCreateInfo2 PCI;

    // Viewports
    PCI.viewportState.viewports.emplace_back( vk::Viewport(0,0,1024,768,0,1.0f));
    PCI.viewportState.scissors.emplace_back( vk::Rect2D( {0,0}, {1024,768}));

    // vertex inputs
    uint32_t stride=0+12+24;
    PCI.setVertexInputAttribute(0,0,vk::Format::eR32G32B32Sfloat,0 );
    PCI.setVertexInputAttribute(1,1,vk::Format::eR32G32B32Sfloat,12);
    PCI.setVertexInputAttribute(2,2,vk::Format::eR8G8B8A8Unorm  ,24);

    PCI.setVertexInputBinding(0,stride, vk::VertexInputRate::eVertex);
    PCI.setVertexInputBinding(1,stride, vk::VertexInputRate::eVertex);
    PCI.setVertexInputBinding(2,stride, vk::VertexInputRate::eVertex);

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

    auto [pipeline, layout, renderpass] = PCI.create( S, window->getDevice() );

    REQUIRE( pipeline != vk::Pipeline());

    // 2 shaders were created
    REQUIRE( S.shaderModules.size() == 2);

    // this wont be true because PCI.renderPass is a RenderPassCreateInfo2
    // and  the getCreateInfo< ... >().renderPass is a vk::RenderPass object
    //REQUIRE(S.getCreateInfo<vkb::GraphicsPipelineCreateInfo2>(pipeline).hash() == PCI.hash());


    S.destroy(pipeline, window->getDevice());

    S.destroyAll(window->getDevice());

    window->destroy();
    window.reset();

    SDL_Quit();

}

