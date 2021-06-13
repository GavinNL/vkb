#ifndef VKB_SPIRV_DESCRIPTORSETLAYOUTGENERATOR_H
#define VKB_SPIRV_DESCRIPTORSETLAYOUTGENERATOR_H

#include <spirv_cross/spirv_cross.hpp>
#include <vulkan/vulkan.h>
#include <map>
#include <vector>
#include <cstdint>

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
                bind.stageFlags     |= stage;
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
            R.size       = range.range;
            R.offset     = range.offset;
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

}

#endif
