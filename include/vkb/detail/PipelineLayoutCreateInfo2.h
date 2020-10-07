#ifndef VKJSON_PIPELINELAYOUTCREATEINFO2_H
#define VKJSON_PIPELINELAYOUTCREATEINFO2_H

#include <vulkan/vulkan.hpp>
#include <functional>
#include <variant>

#include "DescriptorSetLayoutCreateInfo2.h"
#include "Storage.h"

namespace vkb
{

struct PipelineLayoutCreateInfo2
{
    using object_type           = vk::PipelineLayout;
    using base_create_info_type = vk::PipelineLayoutCreateInfo;

    std::vector<vk::DescriptorSetLayout>             setLayouts;
    std::vector<vk::PushConstantRange>               pushConstantRanges;

    // this is an alternative to setLayouts. The pipeline layout will
    // create the descriptorSetLayouts in this vector if it contains
    // any elements. This array is not included in the hash function
    std::vector<vkb::DescriptorSetLayoutCreateInfo2> setLayoutsDescriptions;


    template<typename Callable_t>
    object_type create_t(Callable_t && CC) const
    {
        vk::PipelineLayoutCreateInfo D;

        D.pPushConstantRanges    = pushConstantRanges.data();
        D.pushConstantRangeCount = pushConstantRanges.size();
        D.pSetLayouts            = setLayouts.data();
        D.setLayoutCount         = static_cast<uint32_t>(setLayouts.size());

        return CC(D);
    }

    object_type create(vk::Device d) const
    {
        return
        create_t( [d](base_create_info_type & C)
        {
            return d.createPipelineLayout(C);
        });

    }

    object_type create(Storage & S, vk::Device device) const
    {
        if( setLayoutsDescriptions.size() > 0 && setLayouts.size()==0)
        {
            auto & _setLayouts = setLayoutsDescriptions;

            PipelineLayoutCreateInfo2 plc2;

            for(auto & l : _setLayouts)
            {
                plc2.setLayouts.push_back( l.create(S,device) );
            }
            plc2.pushConstantRanges = pushConstantRanges;

            return plc2.create(S, device);
        }
        else
        {
            auto h = hash();
            auto & _map = S.pipelineLayouts;

            auto f = _map.find(h);
            if( f == _map.end())
            {
                auto l = create(device);
                _map[h] = l;
                return l;
            }
            else
            {
                return f->second;
            }
        }
    }

    size_t hash() const
    {
        std::hash<size_t> H;
        std::hash<void*> Hv;

        size_t seed = 0x9e3779b9;

        auto & _setLayouts = setLayouts;

        for(auto & b : _setLayouts)
        {
            hash_c(seed, Hv( static_cast<void*>(b) ) );
        }
        for(auto & b : pushConstantRanges)
        {
            hash_c(seed, H(b.size) );
            hash_c(seed, H(b.offset) );
            hash_c(seed, vkb::hash_f(b.stageFlags)  );
        }

        return seed;
    }


    //===============================================================
    // Helper functions
    //===============================================================
    vkb::DescriptorSetLayoutCreateInfo2& newDescriptorSet()
    {
        return setLayoutsDescriptions.emplace_back();
    }
    vkb::PipelineLayoutCreateInfo2& addPushConstantRange( vk::ShaderStageFlags stageFlags, uint32_t offset, uint32_t size)
    {
        pushConstantRanges.emplace_back( vk::PushConstantRange(stageFlags, offset, size) );
        return *this;
    }
};






}

#endif
