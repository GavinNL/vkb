#ifndef VKJSON_FROM_JSON_H
#define VKJSON_FROM_JSON_H

#include <nlohmann/json.hpp>
#include "../vkb.h"
#include "from_json_base.h"

namespace vk {

}

namespace vkb
{



inline void from_json(const nlohmann::json & j, PipelineShaderStageCreateInfo2& p)
{
    j.at("name").get_to(p.name);
    p.stage = j.at("stage").get<vk::ShaderStageFlagBits>();

    // "code" : {
    //     "uri" : "",
    //     "type" : "spirv"
    // }

    if( j.count("code") )
    {
        if( j.at("code").is_object() )
        {
            auto uri = j.at("code").at("uri").get<std::string>();
            std::ifstream i;


            //std::vector<uint8_t> readResourceBIN(const vka::uri & _uri) const
            {
                auto path = uri;

                std::ifstream stream(path, std::ios::in | std::ios::binary);
                std::vector<char> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

                assert( contents.size() % sizeof(uint32_t) == 0);
                p.code.resize( contents.size() / sizeof(uint32_t));
                std::memcpy( p.code.data(), contents.data(), contents.size());
            }
        }

    }

}

inline void from_json(const nlohmann::json & j, PipelineViewportStateCreateInfo2& p)
{
    p.viewports = j.at("viewports").get<  std::vector< vk::Viewport > > ();
    p.scissors  = j.at("scissors").get<   std::vector< vk::Rect2D > > ();
}

inline void from_json(const nlohmann::json & j, PipelineColorBlendStateCreateInfo2& p)
{
    j.at("logicOpEnable").get_to(p.logicOpEnable);

    j.at("blendConstants").at(0).get_to(p.blendConstants[0]);
    j.at("blendConstants").at(1).get_to(p.blendConstants[1]);
    j.at("blendConstants").at(2).get_to(p.blendConstants[2]);
    j.at("blendConstants").at(3).get_to(p.blendConstants[3]);

    p.attachments = j.at("attachments").get<   std::vector< vk::PipelineColorBlendAttachmentState > > ();
}


inline void from_json(const nlohmann::json & j, PipelineVertexInputStateCreateInfo2& p)
{
    p.vertexBindingDescriptions   = j.at("vertexBindingDescriptions").get<   std::vector< vk::VertexInputBindingDescription > > ();
    p.vertexAttributeDescriptions = j.at("vertexAttributeDescriptions").get< std::vector< vk::VertexInputAttributeDescription > > ();
}


inline void from_json(const nlohmann::json & j, DescriptorSetLayoutCreateInfo2& p)
{
    p.bindings   = j.at("bindings").get<  std::vector< vk::DescriptorSetLayoutBinding > > ();

    if( j.count("flags"))
    {
        for(auto & f : j.at("flags") )
        {
            auto s= f.get<std::string>();
            if( s == "UpdateAfterBindPool")    p.flags |= vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool;
            if( s == "PushDescriptorKHR")      p.flags |= vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR;
            if( s == "UpdateAfterBindPoolEXT") p.flags |= vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPoolEXT;
        }
    }
}


inline void from_json(const nlohmann::json & j, PipelineLayoutCreateInfo2& p)
{
    vkb::DescriptorSetLayoutCreateInfo2 d;
    std::vector<int> x;

    p.setLayoutsDescriptions   = j.at("setLayouts").get<  std::vector< vkb::DescriptorSetLayoutCreateInfo2 > > ();
    p.pushConstantRanges       = j.at("pushConstantRanges").get<  std::vector< vk::PushConstantRange > > ();
}



inline void from_json(const nlohmann::json & j, GraphicsPipelineCreateInfo2& p)
{
    p.vertexInputState   = j.at("vertexInputState").get<  vkb::PipelineVertexInputStateCreateInfo2  > ();
    p.rasterizationState = j.at("rasterizationState").get<  vk::PipelineRasterizationStateCreateInfo  > ();
    p.multisampleState   = j.at("multisampleState").get<  vk::PipelineMultisampleStateCreateInfo  > ();
    p.depthStencilState         = j.at("depthState").get<  vk::PipelineDepthStencilStateCreateInfo  > ();
    p.blendState         = j.at("blendState").get<  vkb::PipelineColorBlendStateCreateInfo2  > ();
    p.inputAssemblyState      = j.at("inputAssembly").get<  vk::PipelineInputAssemblyStateCreateInfo  > ();
    p.viewportState      = j.at("viewportState").get<  vkb::PipelineViewportStateCreateInfo2  > ();
    p.tessellation       = j.at("tessellation").get<  vk::PipelineTessellationStateCreateInfo  > ();
    p.dynamicStates = j.at("dynamicStates").get< std::vector<vk::DynamicState> >();

    std::vector<std::string> dynamicStates = j.at("dynamicStates").get< std::vector<std::string> >();

    vk::PipelineShaderStageCreateInfo d;

    // uint32_t stageCount_ = {},
    // const VULKAN_HPP_NAMESPACE::PipelineShaderStageCreateInfo* pStages_ = {},
    //
    // VULKAN_HPP_NAMESPACE::PipelineLayout layout_ = {},
    // VULKAN_HPP_NAMESPACE::RenderPass renderPass_ = {},
    // uint32_t subpass_ = {},
    // VULKAN_HPP_NAMESPACE::Pipeline basePipelineHandle_ = {},
    // int32_t basePipelineIndex_ = {} ) VULKAN_HPP_NOEXCEPT

    //(done) const VULKAN_HPP_NAMESPACE::PipelineTessellationStateCreateInfo* pTessellationState_ = {},
    //(done) const VULKAN_HPP_NAMESPACE::PipelineViewportStateCreateInfo* pViewportState_ = {},
    //(done) const VULKAN_HPP_NAMESPACE::PipelineDynamicStateCreateInfo* pDynamicState_ = {},
    //(done) const VULKAN_HPP_NAMESPACE::PipelineInputAssemblyStateCreateInfo* pInputAssemblyState_ = {},
    //(done) const VULKAN_HPP_NAMESPACE::PipelineColorBlendStateCreateInfo* pColorBlendState_ = {},
    //(done) const VULKAN_HPP_NAMESPACE::PipelineDepthStencilStateCreateInfo* pDepthStencilState_ = {},
    //(done) const VULKAN_HPP_NAMESPACE::PipelineMultisampleStateCreateInfo* pMultisampleState_ = {},
    //(done) const VULKAN_HPP_NAMESPACE::PipelineRasterizationStateCreateInfo* pRasterizationState_ = {},
    //(done) const VULKAN_HPP_NAMESPACE::PipelineVertexInputStateCreateInfo* pVertexInputState_ = {},
}


inline void from_json(const nlohmann::json & j, vkb::SubpassDescription2& p)
{
    if( j.count("colorAttachments"))
        p.colorAttachments = j.at("colorAttachments").get< decltype (p.colorAttachments) > ();

    if( j.count("inputAttachments"))
        p.inputAttachments = j.at("inputAttachments").get< decltype (p.inputAttachments) > ();

    p.pipelineBindPoint = j.at("pipelineBindPoint").get< decltype (p.pipelineBindPoint) > ();

    if( j.count("depthStencilAttachment"))
    {
        p.depthStencilAttachment = j.at("depthStencilAttachment").get< decltype (p.depthStencilAttachment)::value_type > ();
    }
    if( j.count("resolveAttachment"))
    {
        p.resolveAttachment = j.at("resolveAttachment").get< decltype (p.resolveAttachment)::value_type > ();
    }
    if( j.count("preserveAttachments"))
    {
        p.preserveAttachments = j.at("preserveAttachments").get< decltype (p.preserveAttachments) > ();
    }

    //p.inputAttachments = j.at("inputAttachments").get< decltype (p.inputAttachments) >();
    //p.colorAttachments = j.at("colorAttachments").get< decltype (p.colorAttachments) >();
    //p.inputAttachments = j.at("inputAttachments").get< decltype (p.inputAttachments) >();
    //p.depthStencilAttachment = j.at("depthStencilAttachment").get< decltype (p.depthStencilAttachment) >();
    //p.resolveAttachment = j.at("resolveAttachment").get< decltype (p.resolveAttachment) >();

    //eGraphics = VK_PIPELINE_BIND_POINT_GRAPHICS,
    //eCompute = VK_PIPELINE_BIND_POINT_COMPUTE,
    //eRayTracingNV = VK_PIPELINE_BIND_POINT_RAY_TRACING_NV

}

inline void from_json(const nlohmann::json & j, RenderPassCreateInfo2& p)
{
//    p.attachments    = j.at("attachments").get< decltype(p.attachments) > ();
//    p.dependencies   = j.at("dependencies").get< std::vector< vk::SubpassDependency > > ();

}


}

#endif
