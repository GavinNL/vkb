#ifndef VKJSON_PIPELINECREATEINFO2_H
#define VKJSON_PIPELINECREATEINFO2_H

#include <nlohmann/json.hpp>
#include <vulkan/vulkan.hpp>

#include "ShaderModuleCreateInfo2.h"
#include "PipelineLayoutCreateInfo2.h"
#include "RenderPassCreateInfo2.h"

namespace vkb
{

struct PipelineShaderStageCreateInfo2
{
    std::string             name;
    vk::ShaderStageFlagBits stage = vk::ShaderStageFlagBits::eVertex;

    vk::ShaderModule        module;
    std::vector<uint32_t>   code; // if given, the PipelineCreateInfo2 will generate the shader module
};

struct PipelineViewportStateCreateInfo2
{
    std::vector< vk::Viewport>   viewports;
    std::vector< vk::Rect2D>     scissors;
};

struct PipelineVertexInputStateCreateInfo2
{
    std::vector< vk::VertexInputBindingDescription>   vertexBindingDescriptions;
    std::vector< vk::VertexInputAttributeDescription> vertexAttributeDescriptions;
};

struct PipelineColorBlendStateCreateInfo2
{
    std::vector< vk::PipelineColorBlendAttachmentState > attachments;

    vk::PipelineColorBlendStateCreateFlags flags = {};
    vk::Bool32  logicOpEnable = {};
    vk::LogicOp logicOp = vk::LogicOp::eClear;

    float blendConstants[4] = {};
};

struct GraphicsPipelineCreateInfo2
{
    using object_type           = vk::Pipeline;
    using base_create_info_type = vk::GraphicsPipelineCreateInfo;

    //=======================================================================
    // The main structures, you can fill these out yourself
    // or use the helper functions.
    //=======================================================================
    std::vector<vkb::PipelineShaderStageCreateInfo2>      stages;

    vkb::PipelineColorBlendStateCreateInfo2               blendState;
    vkb::PipelineVertexInputStateCreateInfo2              vertexInputState;
    vk::PipelineRasterizationStateCreateInfo              rasterizationState;
    vk::PipelineMultisampleStateCreateInfo                multisampleState;
    vk::PipelineDepthStencilStateCreateInfo               depthStencilState;
    vk::PipelineInputAssemblyStateCreateInfo              inputAssemblyState;
    std::vector<vk::DynamicState>                         dynamicStates;
    vkb::PipelineViewportStateCreateInfo2                 viewportState;
    vk::PipelineTessellationStateCreateInfo               tessellation;

    std::variant< vkb::PipelineLayoutCreateInfo2,
                  vk::PipelineLayout>                     layout;

    std::variant< vkb::RenderPassCreateInfo2,
                  vk::RenderPass>                         renderPass;
    //=======================================================================

    GraphicsPipelineCreateInfo2()
    {
        // default values
        depthStencilState
                           .setDepthTestEnable       ( VK_FALSE)
                           .setDepthWriteEnable      ( VK_FALSE)
                           .setDepthCompareOp        ( vk::CompareOp::eLess)// VK_COMPARE_OP_LESS;
                           .setDepthBoundsTestEnable ( VK_FALSE)
                           .setStencilTestEnable     ( VK_FALSE)
                           .setMinDepthBounds        ( 0.0f) // Optional
                           .setMaxDepthBounds        ( 1.0f) // Optional
                           .setStencilTestEnable     ( VK_FALSE);


        rasterizationState.setDepthClampEnable(VK_FALSE)
                          .setDepthBiasClamp(VK_FALSE)
                          .setRasterizerDiscardEnable(VK_FALSE)
                          .setPolygonMode(vk::PolygonMode::eFill)
                          .setLineWidth(1.0f)
                          .setCullMode(vk::CullModeFlagBits::eNone)
                          .setFrontFace( vk::FrontFace::eCounterClockwise)
                          .setDepthBiasEnable(VK_FALSE);

        multisampleState
                          .setRasterizationSamples(vk::SampleCountFlagBits::e1)
                          .setSampleShadingEnable(VK_FALSE);

        inputAssemblyState.topology = vk::PrimitiveTopology::eTriangleList;
    }

    template<typename Callable_t>
    object_type create_t(Callable_t && CC) const
    {
        if( !std::holds_alternative<vk::PipelineLayout>( layout ))
        {
            throw std::runtime_error("Cannot call create_t when the .layout is vkb::PipelineLayoutCreateInfo2");
        }
        if( !std::holds_alternative<vk::RenderPass>( renderPass ))
        {
            throw std::runtime_error("Cannot call create_t when the .renderPass is vkb::RenderPassCreateInfo2");
        }
        base_create_info_type C;

        vk::PipelineColorBlendStateCreateInfo _blendState;
        _blendState.pAttachments       = blendState.attachments.data();
        _blendState.attachmentCount    = blendState.attachments.size();
        _blendState.flags              = blendState.flags          ;
        _blendState.logicOpEnable      = blendState.logicOpEnable  ;
        _blendState.logicOp            = blendState.logicOp        ;
        _blendState.blendConstants[0]  = blendState.blendConstants[0] ;
        _blendState.blendConstants[1]  = blendState.blendConstants[1] ;
        _blendState.blendConstants[2]  = blendState.blendConstants[2] ;
        _blendState.blendConstants[3]  = blendState.blendConstants[3] ;

        vk::PipelineVertexInputStateCreateInfo _vertexInputState;
        _vertexInputState.pVertexBindingDescriptions      = vertexInputState.vertexBindingDescriptions.data();
        _vertexInputState.pVertexAttributeDescriptions    = vertexInputState.vertexAttributeDescriptions.data();
        _vertexInputState.vertexBindingDescriptionCount   = static_cast<uint32_t>(vertexInputState.vertexBindingDescriptions.size()  );
        _vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputState.vertexAttributeDescriptions.size());

        vk::PipelineDynamicStateCreateInfo _dynamicState;
        _dynamicState.dynamicStateCount = dynamicStates.size();
        _dynamicState.pDynamicStates    = dynamicStates.data();

        vk::PipelineViewportStateCreateInfo _viewportState;
        _viewportState.pScissors     = viewportState.scissors.data();
        _viewportState.pViewports    = viewportState.viewports.data();
        _viewportState.scissorCount  = static_cast<uint32_t>(viewportState.scissors.size() );
        _viewportState.viewportCount = static_cast<uint32_t>(viewportState.viewports.size());

        std::vector<vk::PipelineShaderStageCreateInfo>  _stages;
        for(auto & s : stages)
        {
            auto & _s = _stages.emplace_back();
            _s.pName  = s.name.c_str();
            _s.module = s.module;
            _s.stage  = s.stage;
            if( s.module == vk::ShaderModule()  && s.code.size() > 0)
            {
                throw std::runtime_error("Cannot create pipeline using this create_t( ) when shader source code is given. Use .create(vkb::Storage&, vk::device) to create the pipeline while compiling the shaders");
            }
        }

        C.pColorBlendState    = &_blendState;
        C.pTessellationState  = &tessellation;
        C.pInputAssemblyState = &inputAssemblyState;
        C.pMultisampleState   = &multisampleState;
        C.pRasterizationState = &rasterizationState;
        C.pDepthStencilState  = &depthStencilState;
        C.pDynamicState       = &_dynamicState;
        C.pViewportState      = &_viewportState;
        C.pVertexInputState   = &_vertexInputState;
        C.pStages             = _stages.data();
        C.stageCount          = static_cast<uint32_t>(_stages.size());
        C.renderPass          = std::get<vk::RenderPass>(renderPass);;
        C.layout              = std::get<vk::PipelineLayout>(layout);

        return CC(C);

    }

    object_type create(vk::Device d) const
    {
        return create_t( [d](base_create_info_type & C)
        {
            return  d.createGraphicsPipeline(vk::PipelineCache(), C);
        });
    }


    object_type create(Storage & S, vk::Device device) const
    {
        //auto h = hash();
        //auto & _map = S.shaderModules;

        GraphicsPipelineCreateInfo2 cpy = *this;

        // Loop through all the shader stages and make
        // sure that they are all compiled.
        for(auto & s : cpy.stages)
        {
            if( s.module == vk::ShaderModule() )
            {
                if( s.code.size() > 0 )
                {
                    vkb::ShaderModuleCreateInfo2 sm;
                    sm.code = std::move(s.code);

                    s.module = sm.create(S, device);
                }
                else
                {
                    throw std::runtime_error("Attempting to create a pipeline with a null shader module, but no SPIRV code is given");
                }
            }
        }

        // check if the pipeline layout needs to be compiled.
        if( !std::holds_alternative< vk::PipelineLayout >(cpy.layout) )
        {
            cpy.layout =  std::get<vkb::PipelineLayoutCreateInfo2>(cpy.layout).create(S , device);
        }

        if( !std::holds_alternative< vk::RenderPass >(cpy.renderPass) )
        {
            cpy.renderPass =  std::get<vkb::RenderPassCreateInfo2>(cpy.renderPass).create(S , device);
        }

        return cpy.create(device);
    }

    //size_t hash() const
    //{
    //    std::hash<size_t> H;
    //
    //    size_t seed = 0x9e3779b9;
    //
    //    //for(auto & b : code)
    //    //{
    //    //    hash_c(seed, H(b) );
    //    //}
    //    return seed;
    //}


    //=====================================================================
    // Helper Functions
    //=====================================================================
    void setVertexInputAttribute( uint32_t location, uint32_t binding, vk::Format format, uint32_t offset)
    {
        vertexInputState.vertexAttributeDescriptions.emplace_back( vk::VertexInputAttributeDescription(location, binding, format,offset));
    }
    void setVertexInputBinding( uint32_t binding, uint32_t stride, vk::VertexInputRate rate)
    {
        vertexInputState.vertexBindingDescriptions.emplace_back( vk::VertexInputBindingDescription(binding,stride,rate));
    }

    void addStage(vk::ShaderStageFlagBits stage, std::string entryPoint, std::string path)
    {
        auto & c = stages.emplace_back();

        c.name = entryPoint;
        c.stage = stage;

        {
            std::ifstream stream(path, std::ios::in | std::ios::binary);
            std::vector<char> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

            assert( contents.size() % sizeof(uint32_t) == 0);
            c.code.resize( contents.size() / sizeof(uint32_t));
            std::memcpy( c.code.data(), contents.data(), contents.size());
        }
    }

    vkb::DescriptorSetLayoutCreateInfo2& newDescriptorSet()
    {
        //assert( layout == vk::PipelineLayout() );
        return std::get< vkb::PipelineLayoutCreateInfo2 >(layout).newDescriptorSet();
    }
    vkb::GraphicsPipelineCreateInfo2& addPushConstantRange( vk::ShaderStageFlags stageFlags, uint32_t offset, uint32_t size)
    {
        auto & layoutDescription = std::get< vkb::PipelineLayoutCreateInfo2 >(layout);;
        layoutDescription.pushConstantRanges.emplace_back( vk::PushConstantRange(stageFlags, offset, size) );
        return *this;
    }

    vk::PipelineColorBlendAttachmentState& addBlendStateAttachment(bool enableBlending=true)
    {
        auto & _blendstate = blendState.attachments.emplace_back();
        _blendstate.colorWriteMask      = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        _blendstate.blendEnable         = enableBlending;
        _blendstate.colorBlendOp        = vk::BlendOp::eAdd;
        _blendstate.srcAlphaBlendFactor = vk::BlendFactor::eSrcAlpha;
        _blendstate.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
        _blendstate.dstAlphaBlendFactor = vk::BlendFactor::eOneMinusDstAlpha;
        _blendstate.dstColorBlendFactor = vk::BlendFactor::eOneMinusDstAlpha;

        return _blendstate;
    }
};


}

#endif




