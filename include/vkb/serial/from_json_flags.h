#ifndef VKJSON_FROM_JSON_FLAGS_H
#define VKJSON_FROM_JSON_FLAGS_H

#include "from_json_enums.h"

namespace vk
{

template<typename T, typename b>
inline void from_json(const nlohmann::json & j, vk::Flags<T,b> & p)
{
    if( !j.is_array())
        throw std::runtime_error("Flags must be an array of strings.");

    auto v = j.get< std::vector<T> >();
    vk::Flags<T,b> flag = {};
    for(auto & f : v)
    {
        flag |= f;
    }
    p = flag;
}

}
#endif


