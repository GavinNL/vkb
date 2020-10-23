#ifndef VKJSON_STORAGE_H
#define VKJSON_STORAGE_H

#include <vulkan/vulkan.hpp>
#include <typeindex>
#include <map>
#include <any>

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
    void destroy( vk::DeviceMemory d, vk::Device dev )
    {
        dev.freeMemory(d);
        m_createInfos.erase(d);
    }
    void destroy( vk::Buffer d, vk::Device dev )
    {
        dev.destroyBuffer(d);
        m_createInfos.erase(d);
    }
    void destroy( vk::DescriptorPool d, vk::Device dev )
    {
        dev.destroyDescriptorPool(d);
        m_createInfos.erase(d);
    }
    void destroy( vk::Pipeline d, vk::Device dev)
    {
        dev.destroyPipeline(d);
    }
    void destroy( vk::Sampler d, vk::Device dev)
    {
        dev.destroySampler(d);
        _remove(d, samplers);
        m_createInfos.erase(d);
    }
    /**
     * @brief mapMemory
     * @param m
     * @param device
     * @return
     *
     * Maps the device memory and returns a pointer to
     * the memory address. This maps the entire
     * memory space and returns a pointer to the beginning
     * You can leave this mapped for as long as long as you want.
     * You may call this function multiple times without unmapping
     */
    void* mapMemory(vk::DeviceMemory m, vk::Device device);

    /**
     * @brief unmapMemory
     * @param m
     * @param device
     *
     * Forces the memory to be unmapped. You probably shouldn't
     * use this. Map the memory and leave it mapped for as long
     * as the memory is active. Destorying the memory will
     * implicitly unap the memory.
     */
    void  unmapMemory(vk::DeviceMemory m, vk::Device device);



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
        for(auto & x : samplers)
            d.destroySampler(x.second);

        samplers.clear();
        descriptorSetLayouts.clear();
        pipelineLayouts.clear();
        shaderModules.clear();
        renderPasses.clear();
    }

    /**
     * @brief getCreateInfo
     * @param d
     * @return
     *
     * Returns the CreateInfo struct that created this vulkan object.
     * Will throw an exception if the object has not been created
     * using the storage
     */
    template<typename CreateInfoStruct, typename vulkan_handle>
    CreateInfoStruct const& getCreateInfo( vulkan_handle d) const
    {
        try
        {
            return std::any_cast<CreateInfoStruct const&>(m_createInfos.at( static_cast<void*>(d) ));
        }
        catch( std::exception & e)
        {
            throw std::out_of_range("Cound not find the object in the storage. Was this object created using the create(storage&, &createinfo) ?");
        }
    }
protected:
    template<typename CreateInfoStruct, typename vulkan_handle>
    CreateInfoStruct & _getCreateInfo( vulkan_handle d)
    {
        try
        {
            return std::any_cast<CreateInfoStruct&>(m_createInfos.at( static_cast<void*>(d) ));
        }
        catch( std::exception & e)
        {
            throw std::out_of_range("Cound not find the object in the storage. Was this object created using the create(storage&, &createinfo) ?");
        }
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
        m_createInfos.erase(d);
    }
public:
    template<typename vulkan_handle, typename CreateInfo>
    void storeCreateInfo( vulkan_handle h, CreateInfo && c )
    {
        if( m_createInfos.count(static_cast<void*>(h) ))
            throw std::runtime_error("Object already exists");
        m_createInfos[ static_cast<void*>(h) ] = std::move(c);
    }

    std::map< size_t , vk::Sampler >             samplers;
    std::map< size_t , vk::DescriptorSetLayout > descriptorSetLayouts;
    std::map< size_t , vk::PipelineLayout >      pipelineLayouts;
    std::map< size_t , vk::ShaderModule>         shaderModules;
    std::map< size_t , vk::RenderPass>           renderPasses;

    std::map< void*, std::any> m_createInfos;
};




}

#endif

