#include "catch.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

#include <vulkan/vulkan.hpp>
#include <iostream>

#include <vkb/vkb.h>
#include <vkb/serial/from_json.h>
#include <vkb/serial/to_schema.h>

SCENARIO("test")
{
  //  nlohmann::json J;

    auto J = vkb::to_schema<vkb::GraphicsPipelineCreateInfo2>();
    std::cout << J.dump() << std::endl;
    return;
    J["type"] = "object",
    J["ui:widget"] = "tabs";
    J["properties"]["DescriptorSetLayoutBinding"] = vkb::to_schema<vk::DescriptorSetLayoutBinding>();

    J["properties"]["pushConstantRange"] = vkb::to_schema<vk::PushConstantRange>();
    J["properties"]["AttachmentReference"] = vkb::to_schema<vk::AttachmentReference>();

    J["properties"]["PipelineColorBlendAttachmentState"] = vkb::to_schema<vk::PipelineColorBlendAttachmentState>();
    J["properties"]["PipelineDepthStencilStateCreateInfo"] = vkb::to_schema<vk::PipelineDepthStencilStateCreateInfo>();
    J["properties"]["PipelineMultisampleStateCreateInfo"] = vkb::to_schema<vk::PipelineMultisampleStateCreateInfo>();
    J["properties"]["PipelineRasterizationStateCreateInfo"] = vkb::to_schema<vk::PipelineRasterizationStateCreateInfo>();
    J["properties"]["VertexInputAttributeDescription"] = vkb::to_schema<vk::VertexInputAttributeDescription>();
    J["properties"]["VertexInputBindingDescription"] = vkb::to_schema<vk::VertexInputBindingDescription>();

    J["properties"]["testVector"] = vkb::to_schema_vector<float>( {4,2,4.2f});
    J["properties"]["testArray"] = vkb::to_schema_array( std::array<float,5>{1,2,3,4,5});
    std::cout << J.dump(4) << std::endl;
}
