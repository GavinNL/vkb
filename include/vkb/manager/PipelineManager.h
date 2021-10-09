#ifndef VKB_PIPELINE_MANAGER_H
#define VKB_PIPELINE_MANAGER_H

#include <vulkan/vulkan.h>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <map>


namespace vkb
{

#if defined(__ANDROID__)
#define VK_CHECK_RESULT(f)																				\
{																										\
    VkResult res = (f);																					\
    if (res != VK_SUCCESS)																				\
    {																									\
        LOGE("Fatal : VkResult is \" %d \" in %s at line %d", res, __FILE__, __LINE__);					\
        assert(res == VK_SUCCESS);																		\
    }																									\
}
#else
#define VK_CHECK_RESULT(f)																				\
{																										\
    VkResult res = (f);																					\
    if (res != VK_SUCCESS)																				\
    {																									\
        std::cout << "Fatal : VkResult is \"" << res << "\" in " << __FILE__ << " at line " << __LINE__ << std::endl; \
        assert(res == VK_SUCCESS);																		\
    }																									\
}
#endif

enum FormatSizeFlags : int
{
    FORMAT_SIZE_UNKNOWN        = 0,
    FORMAT_SIZE_PACKED_BIT     = 1,
    FORMAT_SIZE_DEPTH_BIT      = 2,
    FORMAT_SIZE_STENCIL_BIT    = 4,
    FORMAT_SIZE_COMPRESSED_BIT = 8
};

struct FormatInfo
{
    FormatSizeFlags flags;
    uint32_t        paletteSizeInBits;
    uint32_t        blockSizeInBits;
    uint32_t        blockWidth;
    uint32_t        blockHeight;
    uint32_t        blockDepth;
    uint32_t        minBlocksX;
    uint32_t        minBlocksY;
};

static inline FormatInfo getFormatInfo( VkFormat format )
{
    FormatInfo info;
    auto * pFormatSize = &info;
    pFormatSize->minBlocksX = pFormatSize->minBlocksY = 1;
    switch ( format )
    {
        case VK_FORMAT_R4G4_UNORM_PACK8:
            pFormatSize->flags = FORMAT_SIZE_PACKED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 1 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
        case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
        case VK_FORMAT_R5G6B5_UNORM_PACK16:
        case VK_FORMAT_B5G6R5_UNORM_PACK16:
        case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
        case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
        case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
            pFormatSize->flags = FORMAT_SIZE_PACKED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 2 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R8_UNORM:
        case VK_FORMAT_R8_SNORM:
        case VK_FORMAT_R8_USCALED:
        case VK_FORMAT_R8_SSCALED:
        case VK_FORMAT_R8_UINT:
        case VK_FORMAT_R8_SINT:
        case VK_FORMAT_R8_SRGB:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 1 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R8G8_UNORM:
        case VK_FORMAT_R8G8_SNORM:
        case VK_FORMAT_R8G8_USCALED:
        case VK_FORMAT_R8G8_SSCALED:
        case VK_FORMAT_R8G8_UINT:
        case VK_FORMAT_R8G8_SINT:
        case VK_FORMAT_R8G8_SRGB:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 2 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R8G8B8_UNORM:
        case VK_FORMAT_R8G8B8_SNORM:
        case VK_FORMAT_R8G8B8_USCALED:
        case VK_FORMAT_R8G8B8_SSCALED:
        case VK_FORMAT_R8G8B8_UINT:
        case VK_FORMAT_R8G8B8_SINT:
        case VK_FORMAT_R8G8B8_SRGB:
        case VK_FORMAT_B8G8R8_UNORM:
        case VK_FORMAT_B8G8R8_SNORM:
        case VK_FORMAT_B8G8R8_USCALED:
        case VK_FORMAT_B8G8R8_SSCALED:
        case VK_FORMAT_B8G8R8_UINT:
        case VK_FORMAT_B8G8R8_SINT:
        case VK_FORMAT_B8G8R8_SRGB:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 3 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R8G8B8A8_UNORM:
        case VK_FORMAT_R8G8B8A8_SNORM:
        case VK_FORMAT_R8G8B8A8_USCALED:
        case VK_FORMAT_R8G8B8A8_SSCALED:
        case VK_FORMAT_R8G8B8A8_UINT:
        case VK_FORMAT_R8G8B8A8_SINT:
        case VK_FORMAT_R8G8B8A8_SRGB:
        case VK_FORMAT_B8G8R8A8_UNORM:
        case VK_FORMAT_B8G8R8A8_SNORM:
        case VK_FORMAT_B8G8R8A8_USCALED:
        case VK_FORMAT_B8G8R8A8_SSCALED:
        case VK_FORMAT_B8G8R8A8_UINT:
        case VK_FORMAT_B8G8R8A8_SINT:
        case VK_FORMAT_B8G8R8A8_SRGB:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 4 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
        case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
        case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
        case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
        case VK_FORMAT_A8B8G8R8_UINT_PACK32:
        case VK_FORMAT_A8B8G8R8_SINT_PACK32:
        case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
            pFormatSize->flags = FORMAT_SIZE_PACKED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 4 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
        case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
        case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
        case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
        case VK_FORMAT_A2R10G10B10_UINT_PACK32:
        case VK_FORMAT_A2R10G10B10_SINT_PACK32:
        case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
        case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
        case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
        case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
        case VK_FORMAT_A2B10G10R10_UINT_PACK32:
        case VK_FORMAT_A2B10G10R10_SINT_PACK32:
            pFormatSize->flags = FORMAT_SIZE_PACKED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 4 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R16_UNORM:
        case VK_FORMAT_R16_SNORM:
        case VK_FORMAT_R16_USCALED:
        case VK_FORMAT_R16_SSCALED:
        case VK_FORMAT_R16_UINT:
        case VK_FORMAT_R16_SINT:
        case VK_FORMAT_R16_SFLOAT:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 2 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R16G16_UNORM:
        case VK_FORMAT_R16G16_SNORM:
        case VK_FORMAT_R16G16_USCALED:
        case VK_FORMAT_R16G16_SSCALED:
        case VK_FORMAT_R16G16_UINT:
        case VK_FORMAT_R16G16_SINT:
        case VK_FORMAT_R16G16_SFLOAT:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 4 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R16G16B16_UNORM:
        case VK_FORMAT_R16G16B16_SNORM:
        case VK_FORMAT_R16G16B16_USCALED:
        case VK_FORMAT_R16G16B16_SSCALED:
        case VK_FORMAT_R16G16B16_UINT:
        case VK_FORMAT_R16G16B16_SINT:
        case VK_FORMAT_R16G16B16_SFLOAT:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 6 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R16G16B16A16_UNORM:
        case VK_FORMAT_R16G16B16A16_SNORM:
        case VK_FORMAT_R16G16B16A16_USCALED:
        case VK_FORMAT_R16G16B16A16_SSCALED:
        case VK_FORMAT_R16G16B16A16_UINT:
        case VK_FORMAT_R16G16B16A16_SINT:
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 8 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R32_UINT:
        case VK_FORMAT_R32_SINT:
        case VK_FORMAT_R32_SFLOAT:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 4 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R32G32_UINT:
        case VK_FORMAT_R32G32_SINT:
        case VK_FORMAT_R32G32_SFLOAT:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 8 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R32G32B32_UINT:
        case VK_FORMAT_R32G32B32_SINT:
        case VK_FORMAT_R32G32B32_SFLOAT:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 12 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R32G32B32A32_UINT:
        case VK_FORMAT_R32G32B32A32_SINT:
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R64_UINT:
        case VK_FORMAT_R64_SINT:
        case VK_FORMAT_R64_SFLOAT:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 8 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R64G64_UINT:
        case VK_FORMAT_R64G64_SINT:
        case VK_FORMAT_R64G64_SFLOAT:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R64G64B64_UINT:
        case VK_FORMAT_R64G64B64_SINT:
        case VK_FORMAT_R64G64B64_SFLOAT:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 24 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_R64G64B64A64_UINT:
        case VK_FORMAT_R64G64B64A64_SINT:
        case VK_FORMAT_R64G64B64A64_SFLOAT:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 32 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
        case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
            pFormatSize->flags = FORMAT_SIZE_PACKED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 4 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_D16_UNORM:
            pFormatSize->flags = FORMAT_SIZE_DEPTH_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 2 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_X8_D24_UNORM_PACK32:
            pFormatSize->flags = static_cast<FormatSizeFlags>(FORMAT_SIZE_PACKED_BIT | FORMAT_SIZE_DEPTH_BIT);
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 4 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_D32_SFLOAT:
            pFormatSize->flags = FORMAT_SIZE_DEPTH_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 4 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_S8_UINT:
            pFormatSize->flags = FORMAT_SIZE_STENCIL_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 1 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_D16_UNORM_S8_UINT:
            pFormatSize->flags = static_cast<FormatSizeFlags>(FORMAT_SIZE_DEPTH_BIT | FORMAT_SIZE_STENCIL_BIT);
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 3 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_D24_UNORM_S8_UINT:
            pFormatSize->flags = static_cast<FormatSizeFlags>(FORMAT_SIZE_DEPTH_BIT | FORMAT_SIZE_STENCIL_BIT);
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 4 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            pFormatSize->flags = static_cast<FormatSizeFlags>(FORMAT_SIZE_DEPTH_BIT | FORMAT_SIZE_STENCIL_BIT);
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 8 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
        case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
        case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
        case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
        case VK_FORMAT_BC4_UNORM_BLOCK:
        case VK_FORMAT_BC4_SNORM_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 8 * 8;
            pFormatSize->blockWidth = 4;
            pFormatSize->blockHeight = 4;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_BC2_UNORM_BLOCK:
        case VK_FORMAT_BC2_SRGB_BLOCK:
        case VK_FORMAT_BC3_UNORM_BLOCK:
        case VK_FORMAT_BC3_SRGB_BLOCK:
        case VK_FORMAT_BC5_UNORM_BLOCK:
        case VK_FORMAT_BC5_SNORM_BLOCK:
        case VK_FORMAT_BC6H_UFLOAT_BLOCK:
        case VK_FORMAT_BC6H_SFLOAT_BLOCK:
        case VK_FORMAT_BC7_UNORM_BLOCK:
        case VK_FORMAT_BC7_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 4;
            pFormatSize->blockHeight = 4;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
        case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
        case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
        case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 8 * 8;
            pFormatSize->blockWidth = 4;
            pFormatSize->blockHeight = 4;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
        case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
        case VK_FORMAT_EAC_R11_UNORM_BLOCK:
        case VK_FORMAT_EAC_R11_SNORM_BLOCK:
        case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
        case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 4;
            pFormatSize->blockHeight = 4;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
        case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 8 * 8;
            pFormatSize->blockWidth = 8;
            pFormatSize->blockHeight = 4;
            pFormatSize->blockDepth = 1;
            pFormatSize->minBlocksX = 2;
            pFormatSize->minBlocksY = 2;
            break;
        case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
        case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 8 * 8;
            pFormatSize->blockWidth = 8;
            pFormatSize->blockHeight = 4;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
        case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 8 * 8;
            pFormatSize->blockWidth = 4;
            pFormatSize->blockHeight = 4;
            pFormatSize->blockDepth = 1;
            pFormatSize->minBlocksX = 2;
            pFormatSize->minBlocksY = 2;
            break;
        case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
        case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 8 * 8;
            pFormatSize->blockWidth = 4;
            pFormatSize->blockHeight = 4;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
        case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 4;
            pFormatSize->blockHeight = 4;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
        case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 5;
            pFormatSize->blockHeight = 4;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
        case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 5;
            pFormatSize->blockHeight = 5;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
        case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 6;
            pFormatSize->blockHeight = 5;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
        case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 6;
            pFormatSize->blockHeight = 6;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
        case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 8;
            pFormatSize->blockHeight = 5;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
        case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 8;
            pFormatSize->blockHeight = 6;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
        case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 8;
            pFormatSize->blockHeight = 8;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
        case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 10;
            pFormatSize->blockHeight = 5;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
        case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 10;
            pFormatSize->blockHeight = 6;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
        case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 10;
            pFormatSize->blockHeight = 8;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
        case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 10;
            pFormatSize->blockHeight = 10;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
        case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 12;
            pFormatSize->blockHeight = 10;
            pFormatSize->blockDepth = 1;
            break;
        case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
        case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
            pFormatSize->flags = FORMAT_SIZE_COMPRESSED_BIT;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 16 * 8;
            pFormatSize->blockWidth = 12;
            pFormatSize->blockHeight = 12;
            pFormatSize->blockDepth = 1;
            break;
        default:
            pFormatSize->flags = FORMAT_SIZE_UNKNOWN;
            pFormatSize->paletteSizeInBits = 0;
            pFormatSize->blockSizeInBits = 0 * 8;
            pFormatSize->blockWidth = 1;
            pFormatSize->blockHeight = 1;
            pFormatSize->blockDepth = 1;
            break;
    }
    return info;
}

/**
 * @brief The GraphicsPipelineCreateInfo struct
 *
 * A simplified GraphicsPipelineCreateInfo struct that
 * you can use to create a graphics pipeline with some default
 * states. T
 *
 * GraphicsPipelineCreateInfo CI;
 *
 * auto pipeline = CI.create([](VkGraphicsPipelineCreateInfo & info)
 * {
 *     // modify info.XXX if you need to do any additional fine-tuning
 *    vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &info, nullptr, &pipeline) != VK_SUCCESS);
 *
 *    return pipeline;
 *  });
 *
 *  This structure can be hashed in an unordered map so that you can
 */
struct GraphicsPipelineCreateInfo
{
    using vertexAttributeLocationIndex_type = uint32_t;
    using vertexAttributeBindingIndex_type = uint32_t;

    struct BindingDescription
    {
        uint32_t          stride    = 0;
        VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    };
    struct AttributeDescription
    {
        VkFormat format;
        uint32_t binding;
    };


    // this map maps [location]->set of bindings
    std::map<vertexAttributeLocationIndex_type, std::vector<AttributeDescription> > vertexAttributes;
    std::map<vertexAttributeBindingIndex_type, BindingDescription > vertexBindings;


    VkViewport                  viewport          = {0, 0, 256, 256, 0, 1.f};
    VkRect2D                    scissor           = {{0, 0}, {256, 256}};
    VkPrimitiveTopology         topology          = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    VkPolygonMode               polygonMode       = VK_POLYGON_MODE_FILL;
    VkCullModeFlags             cullMode          = VK_CULL_MODE_NONE;
    VkFrontFace                 frontFace         = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    bool                        enableDepthTest   = false;
    bool                        enableDepthWrite  = false;
    uint32_t                    tesselationPatchControlPoints = 3;

    // the number of output framebuffers
    // this would normally be 1 if you are rendering to the swapchain.
    // if you are rendering to deferred framebuffer, change this value.
    // This value sets the blend values to some default.
    uint32_t                    outputTargetCount = 1;
    bool                        enableBlending    = true; // enables blending for all targets, if you want
                                                          // specific control, you will need to modify CreateInfo
                                                          // after the call to .create( VkGraphicsPipelineCreateInfo & info )


    // These must not be null
    VkShaderModule   vertexShader      = VK_NULL_HANDLE;
    VkShaderModule   tessEvalShader    = VK_NULL_HANDLE; // can be left null
    VkShaderModule   tessControlShader = VK_NULL_HANDLE; // can be left null
    VkShaderModule   fragmentShader    = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout    = VK_NULL_HANDLE;
    VkRenderPass     renderPass        = VK_NULL_HANDLE;

    std::vector<VkDynamicState> dynamicStates;


    /**
     * @brief create
     * @param C
     * @return
     *
     * This function is used to generate an actual VkGraphicsPipelineCreateInfo structure
     * which you can use to generate your pipeline
     *
     * GraphicsPipelineCreateInfo CI;
     *
     * auto pipeline = CI.create([](VkGraphicsPipelineCreateInfo & info)
     * {
     *     // modify info.XXX if you need to do any additional fine-tuning
     *
     *    vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &info, nullptr, &pipeline) != VK_SUCCESS);
     *
     *    return pipeline;
     *  });
     */
    template<typename callable_t>
    VkPipeline create(callable_t && C) const
    {
        std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

        {
            auto & vertShaderStageInfo = shaderStages.emplace_back();
            vertShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            vertShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
            vertShaderStageInfo.module = vertexShader;
            vertShaderStageInfo.pName  = "main";
        }

        {
            auto & fragShaderStageInfo = shaderStages.emplace_back();
            fragShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            fragShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
            fragShaderStageInfo.module = fragmentShader;
            fragShaderStageInfo.pName  = "main";
        }

        if( tessControlShader != VK_NULL_HANDLE &&  tessEvalShader != VK_NULL_HANDLE )
        {
            {
                auto & stageInfo = shaderStages.emplace_back();
                stageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                stageInfo.stage  = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
                stageInfo.module = tessControlShader;
                stageInfo.pName  = "main";
            }
            {
                auto & stageInfo = shaderStages.emplace_back();
                stageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                stageInfo.stage  = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
                stageInfo.module = tessEvalShader;
                stageInfo.pName  = "main";
            }
        }

        std::vector<VkVertexInputBindingDescription>   inputBindings;
        std::vector<VkVertexInputAttributeDescription> inputVertexAttributes;

        {
            for(auto & [binding, bindingInfo] : vertexBindings)
            {
                auto & info = inputBindings.emplace_back();
                info.binding   = binding;
                info.inputRate = bindingInfo.inputRate;
                info.stride    = bindingInfo.stride;
            }
            for(auto & [location, attributes] : vertexAttributes)
            {
                uint32_t offset = 0;
                for(auto & a : attributes)
                {
                    auto &  va = inputVertexAttributes.emplace_back();
                    va.location = location;
                    va.binding  = a.binding;
                    va.format   = a.format;
                    va.offset   = offset;
                    offset += getFormatInfo(va.format).blockSizeInBits / 8;
                    if( vertexBindings.count(a.binding) == 0)
                    {
                        throw std::runtime_error("Binding" + std::to_string(a.binding) + " is not in the vertexBinding map");
                    }
                }
            }
        }


        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributes.size());
        vertexInputInfo.pVertexAttributeDescriptions    = inputVertexAttributes.data();

        vertexInputInfo.pVertexBindingDescriptions = inputBindings.data();
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(inputBindings.size());

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology               = topology;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports    = &viewport;
        viewportState.scissorCount  = 1;
        viewportState.pScissors     = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable        = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode             = polygonMode;
        rasterizer.lineWidth               = 1.0f;
        rasterizer.cullMode                = cullMode;
        rasterizer.frontFace               = frontFace;
        rasterizer.depthBiasEnable         = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable  = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable         = enableBlending;
        colorBlendAttachment.colorBlendOp        = VK_BLEND_OP_ADD;// vk::BlendOp::eAdd;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;// vk::BlendFactor::eSrcAlpha;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; //vk::BlendFactor::eOneMinusDstAlpha;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;//vk::BlendFactor::eOneMinusDstAlpha;
        std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments( outputTargetCount, colorBlendAttachment);

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable     = VK_FALSE;
        colorBlending.logicOp           = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount   = 1;
        colorBlending.pAttachments      = colorBlendAttachments.data();
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;


        VkPipelineDepthStencilStateCreateInfo dsInfo = {};
        dsInfo.sType                                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        dsInfo.depthTestEnable                       = enableDepthTest;
        dsInfo.depthWriteEnable                      = enableDepthWrite;

        VkPipelineDynamicStateCreateInfo dynamicStatesCI = {};
        dynamicStatesCI.sType                            = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStatesCI.dynamicStateCount                = static_cast<uint32_t>(dynamicStates.size());
        dynamicStatesCI.pDynamicStates                   = dynamicStates.data();

        VkPipelineTessellationStateCreateInfo tessellationState;
        tessellationState.sType              = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
        tessellationState.patchControlPoints = tesselationPatchControlPoints;

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount          = static_cast<uint32_t>(shaderStages.size());
        pipelineInfo.pStages             = shaderStages.data();
        pipelineInfo.pVertexInputState   = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState      = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState   = &multisampling;
        pipelineInfo.pColorBlendState    = &colorBlending;
        pipelineInfo.pDepthStencilState  = &dsInfo;
        pipelineInfo.layout              = pipelineLayout;
        pipelineInfo.renderPass          = renderPass;
        pipelineInfo.pDynamicState       = &dynamicStatesCI;
        pipelineInfo.pTessellationState  = &tessellationState;
        if( tessControlShader == VK_NULL_HANDLE)
            pipelineInfo.pTessellationState  = nullptr;
        pipelineInfo.subpass             = 0;
        pipelineInfo.basePipelineHandle  = VK_NULL_HANDLE;

        return C(pipelineInfo);
    }
};

}
#endif
