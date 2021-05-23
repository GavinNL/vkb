#ifndef VKB_PIPELINECREATEINFO2_H
#define VKB_PIPELINECREATEINFO2_H

#include <vulkan/vulkan.hpp>

#include "HashFunctions.h"
#include "ShaderModuleCreateInfo2.h"
#include "PipelineLayoutCreateInfo2.h"
#include "RenderPassCreateInfo2.h"
#include <fstream>

namespace vkb
{

template<>
struct hash<vk::Viewport>
{
    size_t operator()(vk::Viewport const &v) const
    {
        auto seed = hash_t(v.height);
        seed = hash_combine(seed, hash_t(v.width))  ;
        seed = hash_combine(seed, hash_t(v.maxDepth))  ;
        seed = hash_combine(seed, hash_t(v.minDepth))  ;
        seed = hash_combine(seed, hash_t(v.x))  ;
        seed = hash_combine(seed, hash_t(v.y))  ;
        return seed;
    }
};

template<>
struct hash<vk::Extent2D>
{
    size_t operator()(vk::Extent2D const &v) const
    {
        auto seed = hash_t(v.height);
        seed = hash_combine(seed, hash_t(v.width))  ;
        return seed;
    }
};

template<>
struct hash<vk::Offset2D>
{
    size_t operator()(vk::Offset2D const &v) const
    {
        auto seed = hash_t(v.x);
        seed = hash_combine(seed, hash_t(v.y))  ;
        return seed;
    }
};

template<>
struct hash<vk::Rect2D>
{
    size_t operator()(vk::Rect2D const &v) const
    {
        auto seed = hash_t(v.extent);
        seed = hash_combine(seed, hash_t(v.offset))  ;
        return seed;
    }
};

struct PipelineShaderStageCreateInfo2
{
    std::string             name;
    vk::ShaderStageFlagBits stage = vk::ShaderStageFlagBits::eVertex;

    vk::ShaderModule        module;
    std::vector<uint32_t>   code; // if given, the PipelineCreateInfo2 will generate the shader module
};

template<>
struct hash<PipelineShaderStageCreateInfo2>
{
    size_t operator()(PipelineShaderStageCreateInfo2 const &v) const
    {
        std::hash<std::string> Hs;
        std::hash<void const*> Hv;

        auto seed = Hs(v.name);
        seed = hash_combine(seed, hash_t(v.stage));
        seed = hash_combine(seed, Hv(static_cast<void const*>(v.module)));
        return seed;
    }
};

template<>
struct hash<vk::VertexInputBindingDescription>
{
    size_t operator()(vk::VertexInputBindingDescription const &v) const
    {
        auto seed = hash_t(v.binding);
        seed = hash_combine(seed, hash_t(v.stride) );
        seed = hash_combine(seed, hash_t(v.inputRate) );

        return seed;
    }
};

template<>
struct hash<vk::VertexInputAttributeDescription>
{
    size_t operator()(vk::VertexInputAttributeDescription const &v) const
    {
        auto seed = hash_t(v.binding);

        seed = hash_combine(seed, hash_t(v.location) );
        seed = hash_combine(seed, hash_t(v.offset) );
        seed = hash_combine(seed, hash_t(v.format) );

        return seed;
    }
};

struct PipelineViewportStateCreateInfo2
{
    std::vector< vk::Viewport>   viewports;
    std::vector< vk::Rect2D>     scissors;
};

template<>
struct hash<PipelineViewportStateCreateInfo2>
{
    size_t operator()(PipelineViewportStateCreateInfo2 const &v) const
    {
        auto seed = hash_t(v.viewports);

        seed = hash_combine(seed, hash_t(v.scissors) );

        return seed;
    }
};

struct PipelineVertexInputStateCreateInfo2
{
    std::vector< vk::VertexInputBindingDescription>   vertexBindingDescriptions;
    std::vector< vk::VertexInputAttributeDescription> vertexAttributeDescriptions;
};

template<>
struct hash<PipelineVertexInputStateCreateInfo2>
{
    size_t operator()(PipelineVertexInputStateCreateInfo2 const &v) const
    {
        auto seed = hash_t(v.vertexBindingDescriptions);

        seed = hash_combine(seed, hash_t(v.vertexAttributeDescriptions) );

        return seed;
    }
};

template<>
struct hash<vk::PipelineColorBlendAttachmentState>
{
    size_t operator()(vk::PipelineColorBlendAttachmentState const &v) const
    {
        auto seed = hash_t(v.alphaBlendOp);

        seed = hash_combine(seed, hash_t(v.blendEnable) );
        seed = hash_combine(seed, hash_t(v.colorBlendOp) );
        seed = hash_combine(seed, hash_t(static_cast<VkColorComponentFlags>(v.colorWriteMask) ) );
        seed = hash_combine(seed, hash_t(v.dstAlphaBlendFactor) );
        seed = hash_combine(seed, hash_t(v.dstColorBlendFactor) );
        seed = hash_combine(seed, hash_t(v.srcAlphaBlendFactor) );
        seed = hash_combine(seed, hash_t(v.srcColorBlendFactor) );

        return seed;
    }
};

struct PipelineColorBlendStateCreateInfo2
{
    std::vector< vk::PipelineColorBlendAttachmentState > attachments;

    vk::PipelineColorBlendStateCreateFlags flags = {};
    vk::Bool32  logicOpEnable = {};
    vk::LogicOp logicOp = vk::LogicOp::eClear;

    float blendConstants[4] = {};
};

template<>
struct hash<PipelineColorBlendStateCreateInfo2>
{
    size_t operator()(PipelineColorBlendStateCreateInfo2 const &v) const
    {
        std::hash<float> Hf;

        size_t seed = 0x9e3779b9;
        seed = hash_combine(seed, hash_t( static_cast<VkPipelineColorBlendStateCreateFlags>(v.flags) ));
        seed = hash_combine(seed, hash_t(v.logicOp));
        seed = hash_combine(seed, hash_t(v.logicOpEnable));
        seed = hash_combine(seed, hash_t(v.attachments));

        seed = hash_combine(seed, Hf(v.blendConstants[0]) );
        seed = hash_combine(seed, Hf(v.blendConstants[1]) );
        seed = hash_combine(seed, Hf(v.blendConstants[2]) );
        seed = hash_combine(seed, Hf(v.blendConstants[3]) );
        return seed;
    }
};

template<>
struct hash<vk::PipelineRasterizationStateCreateInfo>
{
    size_t operator()(vk::PipelineRasterizationStateCreateInfo const &v) const
    {
        auto seed = hash_t(v.depthClampEnable);
        seed = hash_combine(seed, hash_t(v.rasterizerDiscardEnable));
        seed = hash_combine(seed, hash_t(v.polygonMode));
        seed = hash_combine(seed, hash_t(static_cast<VkCullModeFlags>(v.cullMode) ));
        seed = hash_combine(seed, hash_t(v.frontFace));
        seed = hash_combine(seed, hash_t(v.depthBiasEnable));
        seed = hash_combine(seed, hash_t(v.depthBiasConstantFactor));
        seed = hash_combine(seed, hash_t(v.depthBiasClamp));
        seed = hash_combine(seed, hash_t(v.depthBiasSlopeFactor));
        seed = hash_combine(seed, hash_t(v.lineWidth));

        return seed;
    }
};

template<>
struct hash<vk::PipelineMultisampleStateCreateInfo>
{
    size_t operator()(vk::PipelineMultisampleStateCreateInfo const &v) const
    {
        auto seed = hash_t(v.alphaToCoverageEnable);
        seed = hash_combine(seed, hash_t(v.alphaToOneEnable));
        seed = hash_combine(seed, hash_t(v.minSampleShading));
        seed = hash_combine(seed, hash_t(v.rasterizationSamples));
        seed = hash_combine(seed, hash_t(v.sampleShadingEnable));

        return seed;
    }
};

template<>
struct hash<vk::StencilOpState>
{
    size_t operator()(vk::StencilOpState const &v) const
    {
        auto seed = hash_t(v.failOp);
        seed = hash_combine(seed, hash_t(v.passOp));
        seed = hash_combine(seed, hash_t(v.depthFailOp));
        seed = hash_combine(seed, hash_t(v.compareOp));
        seed = hash_combine(seed, hash_t(v.compareMask));
        seed = hash_combine(seed, hash_t(v.writeMask));
        seed = hash_combine(seed, hash_t(v.reference));

        return seed;
    }
};

template<>
struct hash<vk::PipelineDepthStencilStateCreateInfo>
{
    size_t operator()(vk::PipelineDepthStencilStateCreateInfo const &v) const
    {
        auto seed = hash_t(v.depthTestEnable);
        seed = hash_combine(seed, hash_t(v.depthWriteEnable));
        seed = hash_combine(seed, hash_t(v.depthCompareOp));
        seed = hash_combine(seed, hash_t(v.depthBoundsTestEnable));
        seed = hash_combine(seed, hash_t(v.stencilTestEnable));
        seed = hash_combine(seed, hash_t(v.front));
        seed = hash_combine(seed, hash_t(v.back));
        seed = hash_combine(seed, hash_t(v.minDepthBounds));
        seed = hash_combine(seed, hash_t(v.maxDepthBounds));

        return seed;
    }
};

template<>
struct hash<vk::PipelineInputAssemblyStateCreateInfo>
{
    size_t operator()(vk::PipelineInputAssemblyStateCreateInfo const &v) const
    {
        auto seed = hash_t(v.topology);
        seed = hash_combine(seed, hash_t(v.primitiveRestartEnable));

        return seed;
    }
};

template<>
struct hash<vk::PipelineTessellationStateCreateInfo>
{
    size_t operator()(vk::PipelineTessellationStateCreateInfo const &v) const
    {
        auto seed = hash_t(v.patchControlPoints);
        return seed;
    }
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
        _blendState.attachmentCount    = static_cast<uint32_t>(blendState.attachments.size());
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
        _dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
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


    /**
     * @brief create
     * @param S
     * @param device
     * @return
     *
     * Create the pipeline and return the layout and renderpass as well. If the CreateInfo struct
     * provided the layout/renderpass, then the returned values are teh same.
     *
     * If the createInfo struct provided layout and renderpass descriptions, then the new layout/renderpasses
     * will be created and returned.
     */
    std::tuple<object_type, vk::PipelineLayout, vk::RenderPass> create(Storage & S, vk::Device device) const
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

        auto x = std::make_tuple(cpy.create(device), std::get<vk::PipelineLayout>(cpy.layout), std::get<vk::RenderPass>(cpy.renderPass) );
        if( std::get<0>(x) )
            S.storeCreateInfo( std::get<0>(x), std::move(cpy));
        return x;
    }

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

    void addStage(vk::ShaderStageFlagBits stage, std::string entryPoint, std::vector<uint32_t> const & spvCode)
    {
        auto & c = stages.emplace_back();

        c.name = entryPoint;
        c.stage = stage;
        c.code = spvCode;
    }

    void addStage(vk::ShaderStageFlagBits stage, std::string entryPoint, std::string path)
    {
        {
            std::ifstream stream(path, std::ios::in | std::ios::binary);
            std::vector<char> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

            std::vector<uint32_t> code;
            assert( contents.size() % sizeof(uint32_t) == 0);
            code.resize( contents.size() / sizeof(uint32_t));
            std::memcpy( code.data(), contents.data(), contents.size());

            addStage(stage, entryPoint, code);
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

template<>
struct hash<vkb::GraphicsPipelineCreateInfo2>
{
    size_t operator()(vkb::GraphicsPipelineCreateInfo2 const & C) const
    {
        auto seed = hash_t(C.stages);

        seed = hash_combine(seed, hash_t(C.blendState));
        seed = hash_combine(seed, hash_t(C.vertexInputState) );
        seed = hash_combine(seed, hash_t(C.rasterizationState));
        seed = hash_combine(seed, hash_t(C.multisampleState));
        seed = hash_combine(seed, hash_t(C.depthStencilState));
        seed = hash_combine(seed, hash_t(C.inputAssemblyState));

        seed = hash_combine(seed, hash_t(C.dynamicStates));

        seed = hash_combine(seed, hash_t(C.viewportState) );
        seed = hash_combine(seed, hash_t(C.tessellation) );

        std::hash<void const*> Hv;
        if( std::holds_alternative<vk::PipelineLayout>(C.layout) )
        {
            seed = hash_combine(seed, Hv( static_cast<void const*>( std::get<vk::PipelineLayout>(C.layout))));
        }
        else
        {
            seed = hash_combine(seed, std::get<vkb::PipelineLayoutCreateInfo2>(C.layout).hash() );
        }

        if( std::holds_alternative<vk::RenderPass>(C.renderPass) )
        {
            seed = hash_combine(seed, Hv( static_cast<void const*>( std::get<vk::RenderPass>(C.renderPass))));
        }
        else
        {
            seed = hash_combine(seed, std::get<vkb::RenderPassCreateInfo2>(C.renderPass).hash() );
        }

        return seed;
    }
};

}

#endif




