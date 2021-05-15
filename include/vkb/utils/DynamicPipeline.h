#ifndef VKB_DYNAMICPIPELINE_H
#define VKB_DYNAMICPIPELINE_H

#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iterator>
#include "../detail/PipelineCreateInfo2.h"
#include "../detail/HashFunctions.h"

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

    vkb::GraphicsPipelineCreateInfo2 const & currentCreateInfo() const
    {
        return m_cci;
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
        //auto p = C.create(*m_storage, device);

        m_cci     = C;//

        auto & V = m_cci.vertexInputState.vertexAttributeDescriptions;
        auto & B = m_cci.vertexInputState.vertexBindingDescriptions;

        std::sort(V.begin(), V.end(), [](auto & A, auto & C)
        {
            return A.location < C.location;
        });
        std::sort(B.begin(), B.end(), [](auto & A, auto & C)
        {
            return A.binding < C.binding;
        });

        //m_cci     = m_storage->getCreateInfo<vkb::GraphicsPipelineCreateInfo2>( std::get<0>(p));

        //std::hash<vkb::GraphicsPipelineCreateInfo2> H;
        //auto h = H(m_cci);
        //m_pipelines[h] = p;
    }

    /**
     * @brief get
     * @return
     *
     * Returns the current pipeline. If the pipeline has
     * not been created, it will be created at this point.
     */
    value_type get()
    {
        {
            auto & V = m_cci.vertexInputState.vertexAttributeDescriptions;
            auto & B = m_cci.vertexInputState.vertexBindingDescriptions;

            std::sort(V.begin(), V.end(), [](auto & A, auto & C)
            {
                return A.location < C.location;
            });
            std::sort(B.begin(), B.end(), [](auto & A, auto & C)
            {
                return A.binding < C.binding;
            });
        }
        auto h = _hash(m_cci);//hash_t(m_cci);
        auto f = m_pipelines.find(h);

        if( f == m_pipelines.end() )
        {
            auto p = m_cci.create(*m_storage, m_device);

            m_cci     = m_storage->getCreateInfo<vkb::GraphicsPipelineCreateInfo2>( std::get<0>(p));

            m_pipelines[h] = p;
            return p;
        }
        else
        {
            return f->second;
        }
    }

    /**
     * @brief bind
     * @param cmd
     *
     * Binds the current pipeline, recompiling it if any
     * of the creation info data has changed.
     */
    void bind(vk::CommandBuffer cmd)
    {
        auto p = get();
        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, std::get<0>(p) );
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
    void setRenderPass( vk::RenderPass p)
    {
        m_cci.renderPass = p;
    }
    void setDepthBiasEnable(bool t)
    {
        m_cci.rasterizationState.setDepthBiasEnable(t);
    }
    void setDepthBiasConstantFactor(float t)
    {
        m_cci.rasterizationState.setDepthBiasConstantFactor(t);
    }
    void setDepthBiasClamp(float t)
    {
        m_cci.rasterizationState.setDepthBiasClamp(t);
    }
    void setDepthBiasSlopeFactor(float t)
    {
        m_cci.rasterizationState.setDepthBiasSlopeFactor(t);
    }
    void enableVertexAttribute(uint32_t location, uint32_t binding, vk::Format format, uint32_t offset )
    {
        auto & V = enableVertexAttribute(location);
        V.binding  = binding;
        V.format   = format;
        V.offset   = offset;
    }
    vk::VertexInputAttributeDescription & enableVertexAttribute(uint32_t location)
    {
        auto & v = m_cci.vertexInputState.vertexAttributeDescriptions;
        auto it = std::find_if( v.begin(), v.end(), [location](vk::VertexInputAttributeDescription const & A)
        {
            return A.location == location;
        });
        if( it == v.end())
        {
            auto & XX =  v.emplace_back();
            XX.location = location;
            return XX;
        }
        return *it;
    }
    void disableVertexAttribute(uint32_t location)
    {
        auto & v = m_cci.vertexInputState.vertexAttributeDescriptions;
        auto it = std::find_if( v.begin(), v.end(), [&](auto & A)
        {
            return A.location == location;
        });
        if( it != v.end())
        {
            v.erase(it);
        }
    }


    void disableVertexInputBinding(uint32_t binding)
    {
        auto & v = m_cci.vertexInputState.vertexAttributeDescriptions;
        auto it = std::find_if( v.begin(), v.end(), [&](auto & A)
        {
            return A.binding == binding;
        });
        if( it != v.end())
        {
            v.erase(it);
        }
    }
    void enableVertexInputBinding(uint32_t binding, uint32_t stride, vk::VertexInputRate rate)
    {
        auto & v = m_cci.vertexInputState.vertexBindingDescriptions;
        auto it = std::find_if( v.begin(), v.end(), [binding](vk::VertexInputBindingDescription const & A)
        {
            return A.binding == binding;
        });
        vk::VertexInputBindingDescription * XX = nullptr;
        if( it == v.end())
        {
            XX =  &v.emplace_back();
        }
        else
        {
            XX = &(*it);
        }
        XX->binding   = binding;
        XX->stride    = stride;
        XX->inputRate = rate;

    }
    //==============================================================

protected:
    // calcualte the hash based on the rasterization staate
    // and the input assembly.
    size_t _hash(vkb::GraphicsPipelineCreateInfo2 const & C) const
    {
        auto seed = hash_t(C.rasterizationState);
        seed = hash_combine(seed, hash_t(C.inputAssemblyState));
        seed = hash_combine(seed, hash_t(C.blendState));
        seed = hash_combine(seed, hash_t(C.vertexInputState));

        return seed;
    }
};


}


namespace vkb
{



}

#endif
