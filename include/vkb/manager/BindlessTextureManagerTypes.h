#ifndef VKB_BINDLESS_TEXTURE_MANAGER_TYPES_H
#define VKB_BINDLESS_TEXTURE_MANAGER_TYPES_H

#include <cstdint>

namespace vkb
{

/**
 * @brief The idTexture_t struct
 *
 * This is a wrapper around an index
 * into the array-of-textures in the shaders.
 *
 */
template<uint32_t _binding>
struct idTexture_t
{
    static constexpr auto binding = _binding;
    int32_t index = -1;
};

using idTexture2D      = idTexture_t<0>;
using idTextureCube    = idTexture_t<1>;
using idTexture2DArray = idTexture_t<2>;

}
#endif
