#ifndef VKB_SPIRV_DESCRIPTORSETLAYOUTGENERATOR_H
#define VKB_SPIRV_DESCRIPTORSETLAYOUTGENERATOR_H

#include <spirv_cross/spirv_cross.hpp>
#include <vulkan/vulkan.h>
#include <map>
#include <vector>
#include <cstdint>
#include <iostream>

namespace vkb
{

/**
 * @brief The SPIRV_DescriptorSetLayoutGenerator struct
 *
 * This class reads a set of SPIRV code and
 * creates the descriptorSetLayoutBindings for you.
 *
    G.addSPIRVCode( V_shaderCode, vk::ShaderStageFlagBits::eVertex);
    G.addSPIRVCode( F_shaderCode, vk::ShaderStageFlagBits::eFragment);

    G.generate([](std::map< uint32_t, std::vector<vk::DescriptorSetLayoutBinding> > & D)
    {
        for(auto & d : D)
        {
            std::cout << "Set: " << d.first << std::endl;

            // each d.second is is a vector of descriptorSetLayoutBindings for set: d.first
            for(auto & e : d.second)
            {
                std::cout << fmt::format("   Binding {}  Count: {}   Type: {}  Stage: {} ", e.binding, e.descriptorCount, to_string(e.descriptorType), to_string(e.stageFlags)) << std::endl;
            }
        }
    });
 */
struct SPIRV_DescriptorSetLayoutGenerator
{
    std::map< uint32_t , std::map<uint32_t, VkDescriptorSetLayoutBinding> > setBindings;

    std::vector<VkPushConstantRange> m_pushRangeV;


    void addSPIRVCode( std::vector<uint32_t> spvCode, VkShaderStageFlagBits stage)
    {
        spirv_cross::Compiler comp(move(spvCode));

        spirv_cross::ShaderResources resources = comp.get_shader_resources();

        auto _handleDescriptorType = [&](spirv_cross::SmallVector<spirv_cross::Resource> & desc, VkDescriptorType _type)
        {
            for (auto &u : desc)
            {
                uint32_t set = comp.get_decoration(u.id, spv::DecorationDescriptorSet);
                uint32_t binding = comp.get_decoration(u.id, spv::DecorationBinding);
                auto arraySize   = comp.get_type(u.type_id).array[0];

                auto & bind          = setBindings[set][binding];
                bind.binding         = binding;
                bind.descriptorCount = std::max(1u, arraySize);
                bind.descriptorType  = _type;
                bind.stageFlags     |= static_cast<VkShaderStageFlags>(stage);
            }

        };
        _handleDescriptorType(resources.uniform_buffers,   VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER         );//vk::DescriptorType::eUniformBuffer);
        _handleDescriptorType(resources.storage_buffers,   VK_DESCRIPTOR_TYPE_STORAGE_BUFFER         );//vk::DescriptorType::eStorageBuffer);
        _handleDescriptorType(resources.sampled_images,    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER );//vk::DescriptorType::eCombinedImageSampler);

        _handleDescriptorType(resources.storage_images,    VK_DESCRIPTOR_TYPE_STORAGE_IMAGE          );//vk::DescriptorType::eSampledImage);

        _handleDescriptorType(resources.separate_samplers, VK_DESCRIPTOR_TYPE_SAMPLER                );//vk::DescriptorType::eSampler);
        _handleDescriptorType(resources.separate_images,   VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE          );//vk::DescriptorType::eSampledImage);


        uint32_t id = resources.push_constant_buffers[0].id;
        auto ranges = comp.get_active_buffer_ranges(id);


        for (auto &range : ranges)
        {
            auto & R = m_pushRangeV.emplace_back();
            R.size       = static_cast<uint32_t>(range.range);
            R.offset     = static_cast<uint32_t>(range.offset);
            R.stageFlags = stage;
         //   std::cout << fmt::format("Accessing member #{}, offset {}, size {}\n", range.index, range.offset, range.range);
        }
    }

    template<typename Callable_t>
    void generate(Callable_t && C)
    {
        std::map< uint32_t, std::vector<VkDescriptorSetLayoutBinding> > setBind;

        for(auto & S : setBindings)
        {
            auto & binding = setBind[ S.first];//.emplace_back();
            for(auto & b  : S.second)
            {
                binding.push_back(b.second);
            }
        }

        C(setBind);
    }
};


struct SPIRV_PipelineReflector
{
    struct
    {
        std::shared_ptr<spirv_cross::Compiler> comp;
        spirv_cross::ShaderResources res;
        VkShaderStageFlagBits stage;
    } vertex, tessControl, tessEval, geometry, fragment;

    SPIRV_PipelineReflector()
    {
        vertex.stage      = VK_SHADER_STAGE_VERTEX_BIT;
        tessControl.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        tessEval.stage    = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        geometry.stage    = VK_SHADER_STAGE_GEOMETRY_BIT;
        fragment.stage    = VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    static std::shared_ptr<spirv_cross::Compiler> _makeCompiler(std::vector<uint32_t> const & spvCode)
    {
        return std::make_shared<spirv_cross::Compiler>(spvCode.data(), spvCode.size());
    }
    /**
     * @brief addSPIRVCode
     * @param spvCode
     * @param stage
     *
     * Your first step:
     *    Add spirv code to the reflector.
     *
     * Some functions require all shader stages to be added,
     * such as determining descriptor layouts
     *
     */
    void addSPIRVCode( std::vector<uint32_t> const & spvCode, VkShaderStageFlagBits stage)
    {
        switch(stage)
        {
        case VK_SHADER_STAGE_VERTEX_BIT:
            vertex.comp = _makeCompiler(spvCode);
            vertex.res  = vertex.comp->get_shader_resources();
            break;
        case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
            tessControl.comp = _makeCompiler(spvCode);
            tessControl.res  = tessControl.comp->get_shader_resources();
            break;
        case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
            tessEval.comp = _makeCompiler(spvCode);
            tessEval.res  = tessEval.comp->get_shader_resources();
            break;
        case VK_SHADER_STAGE_GEOMETRY_BIT:
            geometry.comp = _makeCompiler(spvCode);
            geometry.res  = geometry.comp->get_shader_resources();
            break;
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            fragment.comp = _makeCompiler(spvCode);
            fragment.res  = fragment.comp->get_shader_resources();
            break;
        case VK_SHADER_STAGE_COMPUTE_BIT:
            break;
        case VK_SHADER_STAGE_ALL_GRAPHICS:
            break;
        case VK_SHADER_STAGE_ALL:
            break;
        //case VK_SHADER_STAGE_RAYGEN_BIT_KHR:
        //    break;
        //case VK_SHADER_STAGE_ANY_HIT_BIT_KHR:
        //    break;
        //case VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR:
        //    break;
        //case VK_SHADER_STAGE_MISS_BIT_KHR:
        //    break;
        //case VK_SHADER_STAGE_INTERSECTION_BIT_KHR:
        //    break;
        //case VK_SHADER_STAGE_CALLABLE_BIT_KHR:
        //    break;
        case VK_SHADER_STAGE_TASK_BIT_NV:
            break;
        case VK_SHADER_STAGE_MESH_BIT_NV:
            break;
        default:
            break;
        //case VK_SHADER_STAGE_RAYGEN_BIT_NV:
        //    break;
        //case VK_SHADER_STAGE_ANY_HIT_BIT_NV:
        //    break;
        //case VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV:
        //    break;
        //case VK_SHADER_STAGE_MISS_BIT_NV:
        //    break;
        //case VK_SHADER_STAGE_INTERSECTION_BIT_NV:
        //    break;
        //case VK_SHADER_STAGE_CALLABLE_BIT_NV:
        //    break;
        //case VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM:
        //    break;

        }
    }


    /**
     * @brief generateBlendStateAttachments
     * @return
     *
     * Returns a vector of the color blend attachment state.
     *
     * The fragment resources must have been created first.
     *
     * This will read the fragment shader's output
     * and generate default values for each
     */
    std::vector< VkPipelineColorBlendAttachmentState > generateBlendStateAttachments() const
    {
        std::vector< VkPipelineColorBlendAttachmentState > attachments;
        for(auto & res : fragment.res.stage_outputs)
        {
            (void)res;
            auto & _blendstate = attachments.emplace_back();

            _blendstate.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            _blendstate.colorBlendOp        = VK_BLEND_OP_ADD;// vk::BlendOp::eAdd;
            _blendstate.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;//vk::BlendFactor::eSrcAlpha;
            _blendstate.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA; //vk::BlendFactor::eSrcAlpha;
            _blendstate.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; //vk::BlendFactor::eOneMinusDstAlpha;
            _blendstate.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA; //vk::BlendFactor::eOneMinusDstAlpha;
        }
        return attachments;
    }

    /**
     * @brief generatePushConstantRange
     * @return
     *
     * Returns a push consts range vector for this pipeline
     */
    std::vector<VkPushConstantRange> generatePushConstantRange() const
    {
        std::vector<VkPushConstantRange> pushRangeV;

        // stores the offset and size of each variable in the push consts
        std::map< VkShaderStageFlagBits, std::vector<VkPushConstantRange> > rangePerStage;

        // loop through each stage and
        // add the variable's offset and size to the vector for that stage.
        for(auto * stage : { &vertex, &tessControl, &tessEval, &geometry, &fragment})
        {
            if( !stage->comp)
                continue;
            auto & comp = *stage->comp;
            uint32_t id = stage->res.push_constant_buffers[0].id;
            auto ranges = comp.get_active_buffer_ranges(id);

            for (auto &range : ranges)
            {
                auto &R      = rangePerStage[stage->stage].emplace_back();
                R.size       = static_cast<uint32_t>(range.range);
                R.offset     = static_cast<uint32_t>(range.offset);
                R.stageFlags = stage->stage;
            }
        }

        // loop through each stage now
        for(auto & stg : rangePerStage)
        {
            // and stort all the variables based on their
            // offset
            std::sort( stg.second.begin(),
                       stg.second.end(),
                       [](auto & a, auto &b)
            {
                return a.offset < b.offset;
            });

            auto & rg = pushRangeV.emplace_back();

            // create a single range for this stage
            // starting at the min offset and ending
            // and the max offset + size
            rg.stageFlags = stg.first;
            rg.size = stg.second.back().offset + stg.second.back().size;
            rg.offset = stg.second.front().offset;
        }

        return pushRangeV;
    }

    std::map< uint32_t, std::vector<VkDescriptorSetLayoutBinding> > generateDescriptorSetLayoutBindings() const
    {
        std::map< uint32_t , std::map<uint32_t, VkDescriptorSetLayoutBinding> > setBindings;

        auto _handleDescriptorType = [&](spirv_cross::SmallVector<spirv_cross::Resource> const& desc,
                                         spirv_cross::Compiler & comp,
                                         VkShaderStageFlagBits stage,
                                         VkDescriptorType _type)
        {
            for (auto &u : desc)
            {
                uint32_t set     = comp.get_decoration(u.id, spv::DecorationDescriptorSet);
                uint32_t binding = comp.get_decoration(u.id, spv::DecorationBinding);
                auto arraySize   = comp.get_type(u.type_id).array[0];

                auto & bind          = setBindings[set][binding];
                bind.binding         = binding;
                bind.descriptorCount = std::max(1u, arraySize);
                bind.descriptorType  = _type;
                bind.stageFlags     |= static_cast<VkShaderStageFlags>(stage);
            }
        };

        for(auto * stage : { &vertex, &tessControl, &tessEval, &geometry, &fragment})
        {
            if( !stage->comp)
                continue;
            auto & comp = *stage->comp;
            uint32_t id = stage->res.push_constant_buffers[0].id;
            auto ranges = comp.get_active_buffer_ranges(id);
            auto & resources = stage->res;

            _handleDescriptorType(resources.uniform_buffers,   comp, stage->stage, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER         );//vk::DescriptorType::eUniformBuffer);
            _handleDescriptorType(resources.storage_buffers,   comp, stage->stage, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER         );//vk::DescriptorType::eStorageBuffer);
            _handleDescriptorType(resources.sampled_images,    comp, stage->stage, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER );//vk::DescriptorType::eCombinedImageSampler);
            _handleDescriptorType(resources.storage_images,    comp, stage->stage, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE          );//vk::DescriptorType::eSampledImage);
            _handleDescriptorType(resources.separate_samplers, comp, stage->stage, VK_DESCRIPTOR_TYPE_SAMPLER                );//vk::DescriptorType::eSampler);
            _handleDescriptorType(resources.separate_images,   comp, stage->stage, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE          );//vk::DescriptorType::eSampledImage);
        }

        std::map< uint32_t, std::vector<VkDescriptorSetLayoutBinding> > out;
        for(auto & x : setBindings)
        {
            auto & set = out[x.first];
            for(auto & b : x.second)
            {
                set.push_back( b.second );
            }
        }
        return out;
    }

    /**
     * @brief generateVertexInputDescriptions
     * @return
     *
     * Returns the vertex input attribute descriptions.
     * This will assume that the it will set the
     * offsets as 0 for each attribute
     */
    std::vector< VkVertexInputAttributeDescription > generateVertexInputDescriptions() const
    {
        std::vector< VkVertexInputAttributeDescription > out;
        auto & comp = *vertex.comp;
        for(auto & res : vertex.res.stage_inputs)
        {
            //uint32_t set     = comp.get_decoration(i.id, spv::DecorationDescriptorSet);
            uint32_t binding = comp.get_decoration(res.id, spv::DecorationBinding);
            uint32_t location = comp.get_decoration(res.id, spv::DecorationLocation);
            //auto arraySize   = comp.get_type(u.type_id).array[0];

            const auto &base_type = comp.get_type(res.base_type_id);
            const auto &type      = comp.get_type(res.type_id);

            (void) base_type;

            auto &v    = out.emplace_back();

            (void)binding;
            v.binding  = location; // set these to the same since binding will always be zero for vertex inputs
            v.location = location;
            v.format = _getFormat(type.basetype, type.vecsize );
        }
        std::sort(out.begin(), out.end(),[](auto & a, auto & b)
        {
            return a.location < b.location;
        });

        return out;
    }

    std::vector< std::string > generateVertexInputNames() const
    {
        std::vector< std::string > names;
        std::map<uint32_t, std::string> names_map;

        auto & comp = *vertex.comp;
        for(auto & res : vertex.res.stage_inputs)
        {
            uint32_t location = comp.get_decoration(res.id, spv::DecorationLocation);

            names_map[location] = res.name;
        }
        for(auto & x : names_map)
            names.push_back(x.second);
        return names;
    }

    static VkFormat _getFormat(spirv_cross::SPIRType::BaseType baseType, uint32_t vecSize)
    {
        switch(baseType)
        {
            case spirv_cross::SPIRType::Unknown:
                break;
            case spirv_cross::SPIRType::Void:
                break;
            case spirv_cross::SPIRType::Boolean:
                break;
            case spirv_cross::SPIRType::SByte:
                return std::array<VkFormat, 4>({{VK_FORMAT_R8_SINT, VK_FORMAT_R8G8_SINT, VK_FORMAT_R8G8B8_SINT, VK_FORMAT_R8G8B8A8_SINT}})[vecSize-1];
            case spirv_cross::SPIRType::UByte:
                return std::array<VkFormat, 4>({{VK_FORMAT_R8_UINT, VK_FORMAT_R8G8_UINT, VK_FORMAT_R8G8B8_UINT, VK_FORMAT_R8G8B8A8_UINT}})[vecSize-1];
            case spirv_cross::SPIRType::Short:
                return std::array<VkFormat, 4>({{VK_FORMAT_R16_SINT, VK_FORMAT_R16G16_SINT, VK_FORMAT_R16G16B16_SINT, VK_FORMAT_R16G16B16A16_SINT}})[vecSize-1];
            case spirv_cross::SPIRType::UShort:
                return std::array<VkFormat, 4>({{VK_FORMAT_R16_UINT, VK_FORMAT_R16G16_UINT, VK_FORMAT_R16G16B16_UINT, VK_FORMAT_R16G16B16A16_UINT}})[vecSize-1];
            case spirv_cross::SPIRType::Int:
                return std::array<VkFormat, 4>({{VK_FORMAT_R32_SINT, VK_FORMAT_R32G32_SINT, VK_FORMAT_R32G32B32_SINT, VK_FORMAT_R32G32B32A32_SINT}})[vecSize-1];
            case spirv_cross::SPIRType::UInt:
                return std::array<VkFormat, 4>({{VK_FORMAT_R32_UINT, VK_FORMAT_R32G32_UINT, VK_FORMAT_R32G32B32_UINT, VK_FORMAT_R32G32B32A32_UINT}})[vecSize-1];
            case spirv_cross::SPIRType::Int64:
                return std::array<VkFormat, 4>({{VK_FORMAT_R64_SINT, VK_FORMAT_R64G64_SINT, VK_FORMAT_R64G64B64_SINT, VK_FORMAT_R64G64B64A64_SINT}})[vecSize-1];
            case spirv_cross::SPIRType::UInt64:
                return std::array<VkFormat, 4>({{VK_FORMAT_R64_UINT, VK_FORMAT_R64G64_UINT, VK_FORMAT_R64G64B64_UINT, VK_FORMAT_R64G64B64A64_UINT}})[vecSize-1];
            case spirv_cross::SPIRType::AtomicCounter:
                break;
            case spirv_cross::SPIRType::Half:
                return std::array<VkFormat, 4>({{VK_FORMAT_R16_SFLOAT, VK_FORMAT_R16G16_SFLOAT, VK_FORMAT_R16G16B16_SFLOAT, VK_FORMAT_R16G16B16A16_SFLOAT}})[vecSize-1];
            case spirv_cross::SPIRType::Float:
                return std::array<VkFormat, 4>({{VK_FORMAT_R32_SFLOAT, VK_FORMAT_R32G32_SFLOAT, VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32B32A32_SFLOAT}})[vecSize-1];
            case spirv_cross::SPIRType::Double:
                return std::array<VkFormat, 4>({{VK_FORMAT_R64_SFLOAT, VK_FORMAT_R64G64_SFLOAT, VK_FORMAT_R64G64B64_SFLOAT, VK_FORMAT_R64G64B64A64_SFLOAT}})[vecSize-1];
            case spirv_cross::SPIRType::Struct:
                break;
            case spirv_cross::SPIRType::Image:
                break;
            case spirv_cross::SPIRType::SampledImage:
                break;
            case spirv_cross::SPIRType::Sampler:
                break;
            case spirv_cross::SPIRType::AccelerationStructure:
                break;
            case spirv_cross::SPIRType::RayQuery:
                break;
            case spirv_cross::SPIRType::ControlPointArray:
                break;
            case spirv_cross::SPIRType::Interpolant:
                break;
            case spirv_cross::SPIRType::Char:
                break;

        }
        return VK_FORMAT_UNDEFINED;
    }
};

}

#endif
