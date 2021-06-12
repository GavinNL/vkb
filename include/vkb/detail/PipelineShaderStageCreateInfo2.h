#ifndef VKB_PIPELINESHADERSTAGECREATEINFO2_H
#define VKB_PIPELINESHADERSTAGECREATEINFO2_H

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan.h>
#include "HashFunctions.h"
//#include "ShaderModuleCreateInfo2.h"
//#include "PipelineLayoutCreateInfo2.h"
//#include "RenderPassCreateInfo2.h"
//#include <fstream>

namespace vkb
{

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

}

#endif




