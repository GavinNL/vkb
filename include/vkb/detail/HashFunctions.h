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

template<typename T>
inline size_t hash_pod(T const & v)
{
    static_assert( sizeof(T)%sizeof(uint32_t)==0, "must be a multiple of ");
    size_t seed = 0x9e3779b9;
    std::hash<uint32_t> H;
    uint32_t const * b = reinterpret_cast<uint32_t const*>(&v);
    auto e = b + sizeof(T)/sizeof(uint32_t);
    while(b != e)
    {
        hash_c(seed, H(*b++));
    }
    return seed;
}

}

#endif
