#ifndef VKB_HASHFUNCTIONS_H
#define VKB_HASHFUNCTIONS_H

#include <iostream>
#include <vulkan/vulkan.hpp>
#include <functional>
#include <cstring>

namespace vkb
{
template<typename T>
struct hash;

//--------------------------------------------
template<typename>
struct is_std_vector : std::false_type {};

template<typename T, typename A>
struct is_std_vector<std::vector<T,A>> : std::true_type {};
//--------------------------------------------


inline size_t hash_combine(std::size_t seed, size_t v)
{
    seed ^= v + 0x9e3779b9 + (seed<<6) + (seed>>2);
    return seed;
}

template <class T>
inline void hash_c(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;

    if constexpr (std::is_enum<T>::value)
    {

    }
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

template <class T>
inline size_t hash_t(const T& v)
{
    if constexpr (std::is_enum<T>::value)
    {
        std::hash<size_t> H;
        return H( static_cast<size_t>(v));
    }
    else if constexpr ( std::is_convertible_v<T, VkFlags> )
    {
        std::hash<size_t> H;
        return H( static_cast<size_t>(static_cast<VkFlags>(v)) );
    }
    else if constexpr (std::is_fundamental<T>::value)
    {
        std::hash<T> H;
        return H(v);
    }
    else if constexpr ( is_std_vector<T>::value )
    {
        size_t seed = 0x9e3779b9;
        for(auto &x : v)
        {
            seed = hash_combine( seed, hash_t(x) );
        }
        return seed;
    }
    else
    {
        vkb::hash<T> H;
        return H(v);
    }
}

template<typename T>
inline size_t hash_e(T v)
{
    static_assert( std::is_enum<T>::value, "T is not an emum");
    std::hash<size_t> H;
    return H( static_cast<size_t>(v));
}

#if VK_HEADER_VERSION == 154

template<typename BitType>
inline size_t hash_f( vk::Flags<BitType> c)
{
    (void)c;
    std::hash<size_t> H;
    return H( static_cast<size_t>(static_cast<typename vk::Flags<BitType>::MaskType>(c)) );
}
#elif VK_HEADER_VERSION <= 131
template<typename BitType, typename MaskType>
inline size_t hash_f( vk::Flags<BitType, MaskType> c)
{
    (void)c;
    std::hash<size_t> H;
    return H( static_cast<size_t>(static_cast<MaskType>(c)) );
}
#endif

template<typename T>
inline size_t hash_pod(T const & v)
{
    static_assert( sizeof(T)%sizeof(uint32_t)==0, "must be a multiple of ");
    size_t seed = 0x9e3779b91a1d3e4f;
    std::hash<uint32_t> H;
    uint32_t const * b = reinterpret_cast<uint32_t const*>(&v);
    auto e = b + sizeof(T)/sizeof(uint32_t);

    while(b != e)
    {
        seed = hash_combine(seed, H(*b++) );
    }
    return seed;
}

}

#endif
