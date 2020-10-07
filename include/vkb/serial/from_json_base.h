#ifndef VKJSON_VERTEXINPUTATTRIBUTEDESCRIPTION_H
#define VKJSON_VERTEXINPUTATTRIBUTEDESCRIPTION_H

#include <nlohmann/json.hpp>
#include <vulkan/vulkan.hpp>
#include <fstream>
#include "from_json_enums.h"
#include "from_json_flags.h"

namespace vk
{

void from_json(const nlohmann::json & j, vk::VertexInputBindingDescription& p);
void from_json(const nlohmann::json & j, vk::VertexInputAttributeDescription& p);
void from_json(const nlohmann::json & j, vk::PipelineColorBlendAttachmentState& p);
void from_json(const nlohmann::json & j, vk::Viewport& p);
void from_json(const nlohmann::json & j, vk::Rect2D& p);
void from_json(const nlohmann::json & j, vk::Offset2D& p);
void from_json(const nlohmann::json & j, vk::Extent2D& p);

}


namespace vk
{

inline void from_json(const nlohmann::json & j, vk::Extent2D& p)
{
    if( j.is_object())
    {
        j.at("width").get_to(p.width);
        j.at("height").get_to(p.height);
    }
    else if( j.is_array() && j.size() >= 2)
    {
        j.at(0).get_to(p.width);
        j.at(1).get_to(p.height);
    }
}

inline void from_json(const nlohmann::json & j, vk::Offset2D& p)
{
    if( j.is_object())
    {
        j.at("x").get_to(p.x);
        j.at("y").get_to(p.y);
    }
    else if( j.is_array() && j.size() == 2)
    {
        j.at(0).get_to(p.x);
        j.at(1).get_to(p.y);
    }
}

inline void from_json(const nlohmann::json & j, vk::Rect2D& p)
{
    p.extent = j.at("extent").get<vk::Extent2D>();
    p.offset = j.at("offset").get<vk::Offset2D>();
}

inline void from_json(const nlohmann::json & j, vk::Viewport& p)
{
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
    j.at("width").get_to(p.width);
    j.at("height").get_to(p.height);
    j.at("maxDepth").get_to(p.maxDepth);
    j.at("minDepth").get_to(p.minDepth);
}


inline void from_json(const nlohmann::json & j, vk::PushConstantRange& p)
{
    j.at("size").get_to(p.size);
    j.at("offset").get_to(p.offset);
    j.at("stageFlags").get_to(p.stageFlags);
    auto & S = j.at("stageFlags");

    p.stageFlags = {};
    vk::from_json(S, p.stageFlags);// S.get<vk::ShaderStageFlags>();

}

inline void from_json(const nlohmann::json & j, vk::DescriptorSetLayoutBinding& p)
{
    j.at("binding").get_to(p.binding);
    j.at("descriptorCount").get_to(p.descriptorCount);
    j.at("descriptorType").get_to(p.descriptorType);
    j.at("stageFlags").get_to(p.stageFlags);
}




inline void from_json(const nlohmann::json & j, vk::AttachmentReference& p)
{
    j.at("attachment").get_to(p.attachment);
    p.layout = j.at("layout").get<vk::ImageLayout>();
}


inline void from_json(const nlohmann::json & j, vk::PipelineTessellationStateCreateInfo& p)
{
    j.at("patchControlPoints").get_to(p.patchControlPoints);
}


inline void from_json(const nlohmann::json & j, vk::PipelineInputAssemblyStateCreateInfo& p)
{
    j.at("primitiveRestartEnable").get_to(p.primitiveRestartEnable);
    j.at("topology").get_to(p.topology);
}

inline void from_json(const nlohmann::json & j, vk::PipelineColorBlendAttachmentState& p)
{
    j.at("blendEnable").get_to(p.blendEnable);
    j.at("srcAlphaBlendFactor").get_to(p.srcAlphaBlendFactor);
    j.at("dstAlphaBlendFactor").get_to(p.dstAlphaBlendFactor);
    j.at("srcColorBlendFactor").get_to(p.srcColorBlendFactor);
    j.at("dstColorBlendFactor").get_to(p.dstColorBlendFactor);
    j.at("alphaBlendOp").get_to(p.alphaBlendOp);
    j.at("colorBlendOp").get_to(p.colorBlendOp);
}

inline void from_json(const nlohmann::json & j, vk::PipelineDepthStencilStateCreateInfo& p)
{

    j.at("depthBoundsTestEnable").get_to(p.depthBoundsTestEnable);
    j.at("maxDepthBounds").get_to(p.maxDepthBounds);
    j.at("minDepthBounds").get_to(p.minDepthBounds);
    j.at("depthTestEnable").get_to(p.depthTestEnable);

    j.at("depthWriteEnable").get_to(p.depthWriteEnable);
    j.at("stencilTestEnable").get_to(p.stencilTestEnable);
    j.at("depthCompareOp").get_to(p.depthCompareOp);

}

inline void from_json(const nlohmann::json & j, vk::PipelineMultisampleStateCreateInfo& p)
{
    j.at("alphaToCoverageEnable").get_to(p.alphaToCoverageEnable);
    j.at("minSampleShading").get_to(p.minSampleShading);
    j.at("sampleShadingEnable").get_to(p.sampleShadingEnable);
    j.at("alphaToCoverageEnable").get_to(p.alphaToCoverageEnable);
    j.at("rasterizationSamples").get_to(p.rasterizationSamples);
}

inline void from_json(const nlohmann::json & j, vk::PipelineRasterizationStateCreateInfo& p)
{
    j.at("cullMode").get_to(p.cullMode);
    j.at("frontFace").get_to(p.frontFace);
    j.at("polygonMode").get_to(p.polygonMode);

    j.at("depthBiasClamp").get_to(p.depthBiasClamp);
    j.at("depthClampEnable").get_to(p.depthClampEnable);
    j.at("depthBiasEnable").get_to(p.depthBiasEnable);
    j.at("rasterizerDiscardEnable").get_to(p.rasterizerDiscardEnable);

}

inline void from_json(const nlohmann::json & j, vk::VertexInputBindingDescription& p)
{
    j.at("stride").get_to(p.stride);
    j.at("binding").get_to(p.binding);
    j.at("inputRate").get_to(p.inputRate);
}

inline void from_json(const nlohmann::json & j, vk::VertexInputAttributeDescription& p)
{
    j.at("location").get_to(p.location);
    j.at("binding").get_to(p.binding);
    j.at("offset").get_to(p.offset);
    j.at("format").get_to(p.format);
}

}
#endif


