#ifndef VKJSON_STORAGE_H
#define VKJSON_STORAGE_H

#include <vulkan/vulkan.hpp>

namespace vkb
{

/**
 * @brief The Storage struct
 *
 * A container that stores all re-usable items such as
 * descriptorSetLayouts, pipelineLayouts, renderpasses, samplers, etc.
 *
 */
struct Storage
{
    void destroy( vk::DescriptorSetLayout d, vk::Device dev )
    {
        dev.destroyDescriptorSetLayout(d);
        _remove(d,descriptorSetLayouts);
    }

    void destroy( vk::PipelineLayout d, vk::Device dev )
    {
        dev.destroyPipelineLayout(d);
        _remove(d,pipelineLayouts);
    }
    void destroy( vk::ShaderModule d, vk::Device dev )
    {
        dev.destroyShaderModule(d);
        _remove(d,shaderModules);
    }
    void destroy( vk::RenderPass d, vk::Device dev )
    {
        dev.destroyRenderPass(d);
        _remove(d,renderPasses);
    }
    void destroy( vk::Pipeline d, vk::Device dev)
    {
        dev.destroyPipeline(d);
    }

    template<typename T>
    void _remove( T d, std::map<size_t, T> & mp )
    {
        for (auto itr = mp.begin(); itr != mp.end(); ++itr)
        {
            if( itr->second == d)
            {
                mp.erase(itr);
                return;
            }
        }
    }

    /**
     * @brief destroyAll
     * @param d
     *
     * Destroys all remaining objects.
     */
    void destroyAll(vk::Device d)
    {
        for(auto & x : pipelineLayouts)
            d.destroyPipelineLayout(x.second);
        for(auto & x : descriptorSetLayouts)
            d.destroyDescriptorSetLayout(x.second);
        for(auto & x : shaderModules)
            d.destroyShaderModule(x.second);
        for(auto & x : renderPasses)
            d.destroyRenderPass(x.second);

        descriptorSetLayouts.clear();
        pipelineLayouts.clear();
        shaderModules.clear();
        renderPasses.clear();
    }
    std::map< size_t , vk::DescriptorSetLayout > descriptorSetLayouts;
    std::map< size_t , vk::PipelineLayout >      pipelineLayouts;
    std::map< size_t , vk::ShaderModule>         shaderModules;
    std::map< size_t , vk::RenderPass>           renderPasses;
};


}

#endif
