#ifndef VKB_FORMAT_INFO_H
#define VKB_FORMAT_INFO_H

#include <vulkan/vulkan.h>

namespace vkb
{

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


}

#endif
