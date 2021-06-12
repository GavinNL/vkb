#ifndef VKB_COMPUTEPIPELINECREATEINFO2_H
#define VKB_COMPUTEPIPELINECREATEINFO2_H

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan.h>
#include "HashFunctions.h"
#include "ShaderModuleCreateInfo2.h"
#include "PipelineLayoutCreateInfo2.h"
#include "PipelineShaderStageCreateInfo2.h"
#include <fstream>

namespace vkb
{

struct ComputePipelineCreateInfo2
{
    using object_type           = vk::Pipeline;
    using base_create_info_type = vk::ComputePipelineCreateInfo;

    std::variant< vkb::PipelineLayoutCreateInfo2,
                  vk::PipelineLayout>                     layout;
    vkb::PipelineShaderStageCreateInfo2 stage;

    ComputePipelineCreateInfo2()
    {
        stage.stage = vk::ShaderStageFlagBits::eCompute;
        stage.name = "main";
    }

    template<typename Callable_t>
    object_type create_t(Callable_t && CC) const
    {
        if( !std::holds_alternative<vk::PipelineLayout>( layout ))
        {
            throw std::runtime_error("Cannot call create_t when the .layout is vkb::PipelineLayoutCreateInfo2");
        }

        vk::ComputePipelineCreateInfo C;
        C.stage.module = stage.module;
        C.stage.pName  = stage.name.data();
        C.stage.stage  = stage.stage;
        C.layout       = std::get<vk::PipelineLayout>(layout);

        return CC(C);
    }

    object_type create(vk::Device d) const
    {
        return create_t( [d](base_create_info_type & C)
        {
            #if VK_HEADER_VERSION >= 154
            return  d.createComputePipeline(vk::PipelineCache(), C).value;
            #elif VK_HEADER_VERSION <= 131 // ubuntu 20.04
            return  d.createComputePipeline(vk::PipelineCache(), C);
            #endif
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
    object_type create(Storage & S, vk::Device device) const
    {
        //auto h = hash();
        //auto & _map = S.shaderModules;

        ComputePipelineCreateInfo2 cpy = *this;

        // Loop through all the shader stages and make
        // sure that they are all compiled.
        auto & s = cpy.stage;

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

        // check if the pipeline layout needs to be compiled.
        if( !std::holds_alternative< vk::PipelineLayout >(cpy.layout) )
        {
            cpy.layout =  std::get<vkb::PipelineLayoutCreateInfo2>(cpy.layout).create(S , device);
        }

        auto x = cpy.create(device);
        S.storeCreateInfo(x, cpy);
        return x;
        //return std::make_tuple(x, std::get<vk::PipelineLayout>(cpy.layout) );
    }
};

template<>
struct hash<vkb::ComputePipelineCreateInfo2>
{
    size_t operator()(vkb::ComputePipelineCreateInfo2 const & C) const
    {
        auto seed = hash_t(C.stage);

        std::hash<void const*> Hv;
        if( std::holds_alternative<vk::PipelineLayout>(C.layout) )
        {
            seed = hash_combine(seed, Hv( static_cast<void const*>( std::get<vk::PipelineLayout>(C.layout))));
        }
        else
        {
            seed = hash_combine(seed, std::get<vkb::PipelineLayoutCreateInfo2>(C.layout).hash() );
        }
        return seed;
    }
};

}

#endif




