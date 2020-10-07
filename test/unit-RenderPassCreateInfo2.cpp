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
        vkb::RenderPassCreateInfo2 R;

        vk::AttachmentDescription a;
        a.format         = vk::Format::eR8G8B8A8Unorm;
        a.samples        = vk::SampleCountFlagBits::e1;//VK_SAMPLE_COUNT_1_BIT;									// We don't use multi sampling in this example
        a.loadOp         = vk::AttachmentLoadOp::eClear;//VK_ATTACHMENT_LOAD_OP_CLEAR;							// Clear this attachment at the start of the render pass
        a.storeOp        = vk::AttachmentStoreOp::eStore;//VK_ATTACHMENT_STORE_OP_STORE;							// Keep it's contents after the render pass is finished (for displaying it)
        a.stencilLoadOp  = vk::AttachmentLoadOp::eDontCare;//VK_ATTACHMENT_LOAD_OP_DONT_CARE;					// We don't use stencil, so don't care for load
        a.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;// VK_ATTACHMENT_STORE_OP_DONT_CARE;				// Same for store
        a.initialLayout  = vk::ImageLayout::eUndefined;//VK_IMAGE_LAYOUT_UNDEFINED;						// Layout at render pass start. Initial doesn't matter, so we use undefined
        a.finalLayout    = vk::ImageLayout::eShaderReadOnlyOptimal;//VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;					// Layout to which the attachment is transitioned when the render pass is finished

        R.attachments.emplace_back(vk::AttachmentDescription( {},
                                   vk::Format::eR8G8B8A8Unorm,
                                   vk::SampleCountFlagBits::e1,
                                   vk::AttachmentLoadOp::eClear,
                                   vk::AttachmentStoreOp::eStore,
          /*stencil load*/         vk::AttachmentLoadOp::eDontCare,
          /*stencil load*/         vk::AttachmentStoreOp::eDontCare,
          /*initial layout*/       vk::ImageLayout::eUndefined,
          /*final   layout*/       vk::ImageLayout::eShaderReadOnlyOptimal));

        R.attachments.emplace_back(vk::AttachmentDescription( {},
                                   vk::Format::eD32SfloatS8Uint,
                                   vk::SampleCountFlagBits::e1,
                                   vk::AttachmentLoadOp::eClear,
                                   vk::AttachmentStoreOp::eStore,
          /*stencil load*/         vk::AttachmentLoadOp::eDontCare,
          /*stencil load*/         vk::AttachmentStoreOp::eDontCare,
          /*initial layout*/       vk::ImageLayout::eUndefined,
          /*final   layout*/       vk::ImageLayout::eShaderReadOnlyOptimal));

        R.dependencies.resize(2);
        R.dependencies[0].srcSubpass    = VK_SUBPASS_EXTERNAL;								// Producer of the dependency
        R.dependencies[0].dstSubpass    = 0;													// Consumer is our single subpass that will wait for the execution depdendency
        R.dependencies[0].srcStageMask  = vk::PipelineStageFlagBits::eBottomOfPipe;// VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        R.dependencies[0].dstStageMask  = vk::PipelineStageFlagBits::eColorAttachmentOutput;// VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        R.dependencies[0].srcAccessMask = vk::AccessFlagBits::eMemoryRead;// VK_ACCESS_MEMORY_READ_BIT;
        R.dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;// VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        R.dependencies[0].dependencyFlags = vk::DependencyFlagBits::eByRegion;// VK_DEPENDENCY_BY_REGION_BIT;

        // Second dependency at the end the renderpass
        // Does the transition from the initial to the final layout
        R.dependencies[1].srcSubpass      = 0;													// Producer of the dependency is our single subpass
        R.dependencies[1].dstSubpass      = VK_SUBPASS_EXTERNAL;								// Consumer are all commands outside of the renderpass
        R.dependencies[1].srcStageMask    = vk::PipelineStageFlagBits::eColorAttachmentOutput;//VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        R.dependencies[1].dstStageMask    = vk::PipelineStageFlagBits::eBottomOfPipe;//VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        R.dependencies[1].srcAccessMask   = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
        R.dependencies[1].dstAccessMask   = vk::AccessFlagBits::eMemoryRead;
        R.dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;


        auto & s = R.subpasses.emplace_back();

        s.pipelineBindPoint      = vk::PipelineBindPoint::eGraphics;
        s.colorAttachments.emplace_back( vk::AttachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal));
        s.depthStencilAttachment       = vk::AttachmentReference(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

        vkb::Storage S;

        auto rp =  R.create(S,window->getDevice());
        auto rp2 = R.create(S,window->getDevice());

        REQUIRE( rp != vk::RenderPass());
        REQUIRE( rp == rp2);

        auto rc2 =
        R.defaultSwapchainRenderPass( {
                                          {vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eShaderReadOnlyOptimal},
                                          {vk::Format::eR32G32B32A32Sfloat, vk::ImageLayout::eShaderReadOnlyOptimal},
                                          {vk::Format::eR32G32B32A32Sfloat, vk::ImageLayout::eShaderReadOnlyOptimal}
                                      },
                                      {vk::Format::eD32Sfloat, vk::ImageLayout::eShaderReadOnlyOptimal}
                                      );

        auto rp3 = rc2.create(S,window->getDevice());
        auto rp4 = rc2.create(S,window->getDevice());
        REQUIRE( rp3 == rp4);
        REQUIRE( rp3 != vk::RenderPass());

        S.destroy(rp2, window->getDevice());

        S.destroyAll(window->getDevice());
        //return;

    }

    delete window;
    SDL_Quit();

}

