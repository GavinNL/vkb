#include "catch.hpp"
#include <fstream>
#include <cstring>
#include <iostream>
#include <fmt/format.h>
#include <vkb/utils/SPIRV_DescriptorSetLayoutGenerator.h>
#include <vulkan/vulkan.hpp>
#include <map>

std::vector<uint32_t> readSPV(std::string path)
{
    std::ifstream stream(path, std::ios::in | std::ios::binary);
    std::vector<char> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

    assert( contents.size() % sizeof(uint32_t) == 0);
    std::vector<uint32_t> code;
    code.resize( contents.size() / sizeof(uint32_t));
    std::memcpy( code.data(), contents.data(), contents.size());
    return code;
}

SCENARIO("Scenario 1: Test SPIRV_DescriptorSetLayoutGenerator")
{
    vkb::SPIRV_DescriptorSetLayoutGenerator G;
    std::vector<uint32_t> spirv_binary = readSPV(CMAKE_SOURCE_DIR "/share/shaders/pbr_frag.spv");

    G.addSPIRVCode(readSPV(CMAKE_SOURCE_DIR "/share/shaders/pbr_frag.spv"),
                   VK_SHADER_STAGE_FRAGMENT_BIT);

    G.addSPIRVCode(readSPV(CMAKE_SOURCE_DIR "/share/shaders/pbr_vert.spv"),
                   VK_SHADER_STAGE_VERTEX_BIT);

    G.generate([](auto & D)
    {
        for(auto & d : D)
        {
            std::cout << "Set: " << d.first << std::endl;
            for(auto & e : d.second)
            {
                std::cout << fmt::format("   Binding {}  Count: {}   Type: {}  Stage: {} ", e.binding, e.descriptorCount, to_string(static_cast<vk::DescriptorType>(e.descriptorType)), to_string( static_cast<vk::ShaderStageFlags>(e.stageFlags))) << std::endl;
            }
        }
    });
}

#if 1
SCENARIO( " Scenario 1: Create a DescriptorSetLayout" )
{
    using namespace spirv_cross;
    std::vector<uint32_t> spirv_binary = readSPV(CMAKE_SOURCE_DIR "/share/shaders/pbr_vert.spv");

    Compiler comp(move(spirv_binary));

    std::vector<vk::DescriptorSetLayoutBinding> bindings;

    std::map< uint32_t , std::map<uint32_t, vk::DescriptorSetLayoutBinding> > setBindings;

    // The SPIR-V is now parsed, and we can perform reflection on it.
    ShaderResources resources = comp.get_shader_resources();

    vk::ShaderStageFlagBits currentStage = vk::ShaderStageFlagBits::eFragment;

    auto _handleDescriptorType = [&](SmallVector<Resource> & desc, vk::DescriptorType _type)
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
            bind.stageFlags     |= currentStage;

            std::cout << fmt::format("Found {}  name: {} at set = {}, binding = {}    arraySize: {}", to_string(_type), u.name, set, binding, arraySize) << std::endl;

        }
    };

    _handleDescriptorType(resources.uniform_buffers, vk::DescriptorType::eUniformBuffer);
    _handleDescriptorType(resources.storage_buffers, vk::DescriptorType::eStorageBuffer);
    _handleDescriptorType(resources.sampled_images, vk::DescriptorType::eCombinedImageSampler);
    _handleDescriptorType(resources.separate_samplers, vk::DescriptorType::eSampler);
    _handleDescriptorType(resources.separate_images, vk::DescriptorType::eSampledImage);



    auto & desc = resources.uniform_buffers;
    vk::DescriptorType _type = vk::DescriptorType::eUniformBuffer;
    for (auto &u : desc)
    {
        uint32_t set = comp.get_decoration(u.id, spv::DecorationDescriptorSet);
        uint32_t binding = comp.get_decoration(u.id, spv::DecorationBinding);
        auto arraySize   = comp.get_type(u.type_id).array[0];

        auto & bind = setBindings[set][binding];
        bind.binding         = binding;
        bind.descriptorCount = std::max(1u, arraySize);
        bind.descriptorType  = _type;
        bind.stageFlags     |= currentStage;

        std::cout << fmt::format("Found UBO {} at set = {}, binding = {}    arraySize: {}", u.name, set, binding, arraySize) << std::endl;
    }

    for (auto &u : resources.storage_buffers)
    {
        uint32_t set = comp.get_decoration(u.id, spv::DecorationDescriptorSet);
        uint32_t binding = comp.get_decoration(u.id, spv::DecorationBinding);
        auto arraySize   = comp.get_type(u.type_id).array[0];

        auto vecSize = comp.get_type(u.type_id).array;

        auto & bind = setBindings[set][binding];
        bind.binding         = binding;
        bind.descriptorCount = 1;
        bind.descriptorType  = vk::DescriptorType::eUniformBuffer;
        bind.stageFlags     |= currentStage;

        std::cout << fmt::format("Found Storage {} at set = {}, binding = {}   size: {}", u.name, set, binding, fmt::join(vecSize,",")) << std::endl;
    }

    for (auto &u : resources.sampled_images)
    {
        uint32_t set = comp.get_decoration(u.id, spv::DecorationDescriptorSet);
        uint32_t binding = comp.get_decoration(u.id, spv::DecorationBinding);
        auto arraySize   = comp.get_type(u.type_id).array[0];

        auto vecSize = comp.get_type(u.type_id).array;
        std::cout << fmt::format("Found Sampled Image {} at set = {}, binding = {}    size: {}", u.name, set, binding, fmt::join(vecSize,",")) << std::endl;
    }

    for (auto &u : resources.separate_images)
    {
        uint32_t set = comp.get_decoration(u.id, spv::DecorationDescriptorSet);
        uint32_t binding = comp.get_decoration(u.id, spv::DecorationBinding);
        auto arraySize   = comp.get_type(u.type_id).array[0];

        auto vecSize = comp.get_type(u.type_id).array;
        std::cout << fmt::format("Found Separate Image Image {} at set = {}, binding = {}    size: {}", u.name, set, binding, fmt::join(vecSize,",")) << std::endl;
    }

    for (auto &u : resources.separate_samplers)
    {
        uint32_t set = comp.get_decoration(u.id, spv::DecorationDescriptorSet);
        uint32_t binding = comp.get_decoration(u.id, spv::DecorationBinding);

        auto vecSize = comp.get_type(u.type_id).array;
        std::cout << fmt::format("Found Separate Sampler {} at set = {}, binding = {}    size: {}", u.name, set, binding, fmt::join(vecSize,",")) << std::endl;
    }

    uint32_t id = resources.push_constant_buffers[0].id;
    auto ranges = comp.get_active_buffer_ranges(id);

    for (auto &range : ranges)
    {
        std::cout << fmt::format("Accessing member #{}, offset {}, size {}\n", range.index, range.offset, range.range);
    }
    // Possible to get names for struct members as well 

}

#endif
