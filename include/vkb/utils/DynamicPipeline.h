#ifndef VKB_DYNAMICPIPELINE_H
#define VKB_DYNAMICPIPELINE_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iterator>
#include "../detail/PipelineCreateInfo2.h"

namespace vkb
{

struct DynamicPipeline
{
    using value_type = std::tuple<vk::Pipeline, vk::PipelineLayout, vk::RenderPass>;
protected:
    vkb::GraphicsPipelineCreateInfo2 m_cci;
    std::map< size_t, value_type >   m_pipelines;
    vkb::Storage                    *m_storage = nullptr;
    vk::Device                       m_device;

public:
    DynamicPipeline()
    {
    }

    // returns the total number of pipelines
    // that have been created.
    size_t pipelineCount() const
    {
        return m_pipelines.size();
    }

    // destroy all pipelines that have been created
    // renderpasses/layouts will not be destroyed
    void destroy()
    {
        for(auto &  x : m_pipelines)
        {
            m_device.destroyPipeline( std::get<0>( x.second ));
        }
        m_pipelines.clear();
    }
    /**
     * @brief init
     * @param S
     * @param C
     * @param device
     *
     * Initialize the DyamicPipeline with an initial state.
     *  This will compile the pipeline.
     */
    void init(vkb::Storage * S, vkb::GraphicsPipelineCreateInfo2 const &C, vk::Device device)
    {
        m_device  = device;
        m_storage = S;
        // compile the initial pipeline.
        auto p = C.create(*m_storage, device);

        m_cci     = m_storage->getCreateInfo<vkb::GraphicsPipelineCreateInfo2>( std::get<0>(p));

        auto h = _hash(m_cci);
        m_pipelines[h] = p;
    }

    value_type get()
    {
        auto h = _hash(m_cci);
        auto f = m_pipelines.find(h);

        if( f == m_pipelines.end() )
        {
            auto p = m_cci.create(*m_storage, m_device);

            m_cci     = m_storage->getCreateInfo<vkb::GraphicsPipelineCreateInfo2>( std::get<0>(p));

            m_pipelines[h] = p;
        }
        else
        {
            return f->second;
        }
    }


    //==============================================================
    // the properties of thes can be modified
    //==============================================================
    vk::PipelineInputAssemblyStateCreateInfo& getInputAssemblyState()
    {
        return m_cci.inputAssemblyState;
    }
    vk::PipelineRasterizationStateCreateInfo& getRasterizationState()
    {
        return m_cci.rasterizationState;
    }


    //==============================================================
    // extra helper functions for commonly used states
    //==============================================================
    void setTopology( vk::PrimitiveTopology p )
    {
        m_cci.inputAssemblyState.setTopology(p);
    }
    void setCullMode( vk::CullModeFlags p )
    {
        m_cci.rasterizationState.setCullMode(p);
    }
    void setFrontFace( vk::FrontFace p )
    {
        m_cci.rasterizationState.setFrontFace(p);
    }
    void setPolygonMode( vk::PolygonMode p )
    {
        m_cci.rasterizationState.setPolygonMode(p);
    }

protected:
    // calcualte the hash based on the rasterization staate
    // and the input assembly.
    static size_t _hash(vkb::GraphicsPipelineCreateInfo2 const & C)
    {
        size_t seed = 0x9e3779b9;

        hash_c(seed, hash_pod(C.rasterizationState));
        hash_c(seed, hash_pod(C.inputAssemblyState));

        return seed;
    }



};

}

#endif
