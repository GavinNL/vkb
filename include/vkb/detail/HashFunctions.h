#ifndef VKJSON_HASHFUNCTIONS_H
#define VKJSON_HASHFUNCTIONS_H

#include <vulkan/vulkan.hpp>
#include <functional>

namespace vkb
{

template <class T>
inline void hash_c(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

template<typename T>
inline size_t hash_e(T v)
{
    static_assert( std::is_enum<T>::value, "T is not an emum");
    static std::hash<size_t> H;
    return H( static_cast<size_t>(v));
}

template<typename Bits, typename Flags>
inline size_t hash_f( vk::Flags<Bits,Flags>  c)
{
    static std::hash<size_t> H;
    return H( static_cast<Flags>(c));
}

}

#endif
