#ifndef VKJSON_RENDERPASSCREATEINFO2_H
#define VKJSON_RENDERPASSCREATEINFO2_H

#include <vulkan/vulkan.hpp>
#include <functional>
#include "HashFunctions.h"

namespace vkb
{

struct SubpassDescription2
{
    vk::PipelineBindPoint                  pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    std::vector<vk::AttachmentReference>   inputAttachments;
    std::vector<vk::AttachmentReference>   colorAttachments;
    std::optional<vk::AttachmentReference> depthStencilAttachment;
    std::optional<vk::AttachmentReference> resolveAttachment;

    std::vector<uint32_t> preserveAttachments;

    template<typename Callable_t>
    void create_t(Callable_t && CC) const
    {
        vk::SubpassDescription D({}, vk::PipelineBindPoint::eGraphics,
                                 inputAttachments.size(), inputAttachments.data(),
                                 colorAttachments.size(), colorAttachments.data(),
                                 resolveAttachment.has_value() ? &*resolveAttachment : nullptr,
                                 depthStencilAttachment.has_value() ? &*depthStencilAttachment : nullptr,
                                 preserveAttachments.size(),
                                 preserveAttachments.data());
        CC(D);
    }

    size_t hash() const
    {
        std::hash<uint32_t> Hu;

        size_t seed = 0x9e3779b9;
        hash_c(seed, hash_e(pipelineBindPoint));
        for(auto & a : inputAttachments)
        {
            hash_c(seed, hash_e(a.layout)       );// = vk::Format::eR8G8B8A8Unorm;
            hash_c(seed, Hu(a.attachment)       );// = vk::SampleCountFlagBits::e1;//VK_SAMPLE_COUNT_1_BIT;									// We don't use multi sampling in this example
        }
        for(auto & a : colorAttachments)
        {
            hash_c(seed, hash_e(a.layout)       );// = vk::Format::eR8G8B8A8Unorm;
            hash_c(seed, Hu(a.attachment)       );// = vk::SampleCountFlagBits::e1;//VK_SAMPLE_COUNT_1_BIT;									// We don't use multi sampling in this example
        }
        if( depthStencilAttachment.has_value())
        {
            auto & a = *depthStencilAttachment;
            hash_c(seed, hash_e(a.layout)       );// = vk::Format::eR8G8B8A8Unorm;
            hash_c(seed, Hu(a.attachment)       );// = vk::SampleCountFlagBits::e1;//VK_SAMPLE_COUNT_1_BIT;									// We don't use multi sampling in this example
        }
        if( resolveAttachment.has_value())
        {
            auto & a = *resolveAttachment;
            hash_c(seed, hash_e(a.layout)       );// = vk::Format::eR8G8B8A8Unorm;
            hash_c(seed, Hu(a.attachment)       );// = vk::SampleCountFlagBits::e1;//VK_SAMPLE_COUNT_1_BIT;									// We don't use multi sampling in this example
        }
        for(auto & a : preserveAttachments)
        {
            hash_c(seed, Hu(a)       );// = vk::SampleCountFlagBits::e1;//VK_SAMPLE_COUNT_1_BIT;									// We don't use multi sampling in this example
        }


        return seed;
    }

};


struct RenderPassCreateInfo2
{
    using object_type           = vk::RenderPass;
    using base_create_info_type = vk::RenderPassCreateInfo;

    std::vector<vk::AttachmentDescription> attachments;
    std::vector<vk::SubpassDependency>     dependencies;
    std::vector<vkb::SubpassDescription2>  subpasses;


    template<typename Callable_t>
    object_type create_t(Callable_t && CC) const
    {
        base_create_info_type C;


        std::vector<vk::SubpassDescription>   _subpasses;
        for(auto & d : subpasses)
        {
            d.create_t( [&](auto & _D)
            {
                _subpasses.emplace_back(_D);
            });

        }

        C.pDependencies   = dependencies.data();
        C.dependencyCount = dependencies.size();
        C.pAttachments    = attachments.data();
        C.attachmentCount = attachments.size();
        C.subpassCount = _subpasses.size();
        C.pSubpasses   = _subpasses.data();

        return CC(C);
    }

    object_type create(vk::Device d) const
    {
        return
        create_t( [d](base_create_info_type & C)
        {
            return d.createRenderPass(C);
        });

    }

    object_type create(Storage & S, vk::Device device) const
    {
        auto h = hash();
        auto & _map = S.renderPasses;

        auto f = _map.find(h);
        if( f == _map.end())
        {
            auto l = create(device);
            _map[h] = l;
            return l;
        }
        else
        {
            return f->second;
        }
    }

    size_t hash() const
    {
        std::hash<uint32_t> Hu;

        size_t seed = 0x9e3779b9;

        for(auto & a : attachments)
        {
            hash_c(seed, hash_e(a.format )       );// = vk::Format::eR8G8B8A8Unorm;
            hash_c(seed, hash_e(a.samples)       );// = vk::SampleCountFlagBits::e1;//VK_SAMPLE_COUNT_1_BIT;									// We don't use multi sampling in this example
            hash_c(seed, hash_e(a.loadOp        ));// = vk::AttachmentLoadOp::eClear;//VK_ATTACHMENT_LOAD_OP_CLEAR;							// Clear this attachment at the start of the render pass
            hash_c(seed, hash_e(a.storeOp       ));// = vk::AttachmentStoreOp::eStore;//VK_ATTACHMENT_STORE_OP_STORE;							// Keep it's contents after the render pass is finished (for displaying it)
            hash_c(seed, hash_e(a.stencilLoadOp ));// = vk::AttachmentLoadOp::eDontCare;//VK_ATTACHMENT_LOAD_OP_DONT_CARE;					// We don't use stencil, so don't care for load
            hash_c(seed, hash_e(a.stencilStoreOp));// = vk::AttachmentStoreOp::eDontCare;// VK_ATTACHMENT_STORE_OP_DONT_CARE;				// Same for store
            hash_c(seed, hash_e(a.initialLayout ));// = vk::ImageLayout::eUndefined;//VK_IMAGE_LAYOUT_UNDEFINED;						// Layout at render pass start. Initial doesn't matter, so we use undefined
            hash_c(seed, hash_e(a.finalLayout   ));// = vk::ImageLayout::eShaderReadOnlyOptimal;//VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        }
        for(auto & a : dependencies)
        {
            hash_c( seed, Hu(a.srcSubpass     ) );//= VK_SUBPASS_EXTERNAL;								// Producer of the dependency
            hash_c( seed, Hu(a.dstSubpass     ) );//= 0;													// Consumer is our single subpass that will wait for the execution depdendency
            hash_c( seed, hash_f(a.srcStageMask   ) );//= vk::PipelineStageFlagBits::eBottomOfPipe;// VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            hash_c( seed, hash_f(a.dstStageMask   ) );//= vk::PipelineStageFlagBits::eColorAttachmentOutput;// VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            hash_c( seed, hash_f(a.srcAccessMask  ) );//= vk::AccessFlagBits::eMemoryRead;// VK_ACCESS_MEMORY_READ_BIT;
            hash_c( seed, hash_f(a.dstAccessMask  ) );//= vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;// VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            hash_c( seed, hash_f(a.dependencyFlags) );//= vk::DependencyFlagBits::eByRegion;// VK_DEPENDENCY_BY_REGION_BIT;
        }

        for(auto & b : subpasses)
        {
            hash_c(seed, b.hash() );
        }
        return seed;
    }


    // create a simple renderPassCreateInfo2 struct that can be used
    // for a simple swapchian with depth testing
    //
    // Always places the depthstencil image last in the attachments.
    static RenderPassCreateInfo2 defaultSwapchainRenderPass(  std::vector< std::pair<vk::Format,vk::ImageLayout> > colors, std::pair<vk::Format,vk::ImageLayout> depthFormat = {vk::Format::eUndefined, vk::ImageLayout::eUndefined})
    {
        vkb::RenderPassCreateInfo2 R;

        for(auto & c : colors)
            R.attachments.emplace_back(vk::AttachmentDescription( {},
                                       c.first,
                                       vk::SampleCountFlagBits::e1,
                                       vk::AttachmentLoadOp::eClear,
                                       vk::AttachmentStoreOp::eStore,
              /*stencil load*/         vk::AttachmentLoadOp::eDontCare,
              /*stencil load*/         vk::AttachmentStoreOp::eDontCare,
              /*initial layout*/       vk::ImageLayout::eUndefined,
              /*final   layout*/       c.second));

        if( depthFormat.first != vk::Format::eUndefined)
            R.attachments.emplace_back(vk::AttachmentDescription( {},
                                       depthFormat.first,
                                       vk::SampleCountFlagBits::e1,
                                       vk::AttachmentLoadOp::eClear,
                                       vk::AttachmentStoreOp::eStore,
              /*stencil load*/         vk::AttachmentLoadOp::eDontCare,
              /*stencil load*/         vk::AttachmentStoreOp::eDontCare,
              /*initial layout*/       vk::ImageLayout::eUndefined,
              /*final   layout*/       depthFormat.second));

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

        uint32_t i=0;
        for(auto & c : colors)
        {
            (void)c;
            s.colorAttachments.emplace_back( vk::AttachmentReference(i++, vk::ImageLayout::eColorAttachmentOptimal));
        }

        if( depthFormat.first != vk::Format::eUndefined)
            s.depthStencilAttachment       = vk::AttachmentReference(i, vk::ImageLayout::eDepthStencilAttachmentOptimal);

        return R;
    }

};



}

#endif
