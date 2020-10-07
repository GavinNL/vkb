#ifndef VKJSON_SHADERMODULECREATEINFO_H
#define VKJSON_SHADERMODULECREATEINFO_H

#include <nlohmann/json.hpp>
#include <vulkan/vulkan.hpp>
#include <functional>

#include "HashFunctions.h"
#include "Storage.h"

namespace vkb
{

struct ShaderModuleCreateInfo2
{
    using object_type           = vk::ShaderModule;
    using base_create_info_type = vk::ShaderModuleCreateInfo;

    std::vector<uint32_t>   code;


    template<typename Callable_t>
    object_type create_t(Callable_t && CC) const
    {
        vk::ShaderModuleCreateInfo C;
        C.pCode = code.data();
        C.codeSize = code.size() * sizeof(uint32_t);
        return CC(C);
    }

    object_type create(vk::Device d) const
    {
        return
        create_t( [d](base_create_info_type & C)
        {
            return d.createShaderModule(C);
        });

    }

    object_type create(Storage & S, vk::Device device) const
    {
        auto h = hash();
        auto & _map = S.shaderModules;

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

    size_t hash() const
    {
        std::hash<uint32_t> H;

        size_t seed = 0x9e3779b9;

        for(auto & b : code)
        {
            hash_c(seed, H(b) );
        }
        return seed;
    }
};

//inline void from_json(const nlohmann::json & j, DescriptorSetLayoutCreateInfo2& p)
//{
//    p.bindings   = j.at("bindings").get<  std::vector< vk::DescriptorSetLayoutBinding > > ();
//
//    if( j.count("flags"))
//    {
//        for(auto & f : j.at("flags") )
//        {
//            auto s= f.get<std::string>();
//            if( s == "UpdateAfterBindPool")    p.flags |= vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool;
//            if( s == "PushDescriptorKHR")      p.flags |= vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR;
//            if( s == "UpdateAfterBindPoolEXT") p.flags |= vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPoolEXT;
//        }
//    }
//}

}

#endif
