#ifndef VKJSON_to_SCHEMAS_H
#define VKJSON_to_SCHEMAS_H

#include <nlohmann/json.hpp>
#include <vulkan/vulkan.hpp>
#include <fstream>

#include "../vkb.h"

namespace vkb
{

template<typename T>
nlohmann::json to_schema();

template<>
inline nlohmann::json to_schema<uint32_t>()
{
    return
    nlohmann::json::parse(
R"foo(
     {
            "type": "integer",
            "minimum" : 0
    }
)foo");
}

template<>
inline nlohmann::json to_schema<int32_t>()
{
    return
    nlohmann::json::parse(
R"foo(
     {
            "type": "integer"
    }
)foo");
}
template<>
inline nlohmann::json to_schema<float>()
{
    return
    nlohmann::json::parse(
R"foo(
    {
       "type": "number"
    }
)foo");
}

template<>
inline nlohmann::json to_schema<bool>()
{
    return
    nlohmann::json::parse(
R"foo(
    {
       "type": "boolean"
    }
)foo");
}

template<>
inline nlohmann::json to_schema<std::string>()
{
    return
    nlohmann::json::parse(
R"foo(
    {
       "type": "string"
    }
)foo");
}

template<typename T>
inline nlohmann::json to_schema_vector( std::vector<T> const &def = std::vector<T>() )
{
   nlohmann::json J;
   J["type"] = "array";
   J["additionalItems"] = to_schema<T>();
   for(auto & d : def)
   {
       J["items"].push_back( to_schema<T>() );
       if constexpr ( std::is_fundamental<T>::value )
       {
           J["items"].back()["default"] = d;
       }
       else if constexpr ( std::is_same<T, std::string>::value )
       {
           J["items"].back()["default"] = d;
       }
   }
   return J;
}

template<typename T, size_t N>
inline nlohmann::json to_schema_array( std::array<T,N> const &def = std::array<T,N>() )
{
   nlohmann::json J;
   J["type"] = "array";

   for(auto & d : def)
   {
       J["items"].push_back( to_schema<T>() );
       if constexpr ( std::is_fundamental<T>::value )
       {
           J["items"].back()["default"] = d;
       }
       else if constexpr ( std::is_same<T, std::string>::value )
       {
           J["items"].back()["default"] = d;
       }
   }
   return J;
}


//=========================================================================
// Enums
//=========================================================================
template<>
inline nlohmann::json to_schema<vk::ImageLayout>()
{
    return
    nlohmann::json::parse(
R"foo(
     {
            "type": "string",
            "enum": [
                "Undefined",
                "General",
                "ColorAttachmentOptimal",
                "DepthStencilAttachmentOptimal",
                "DepthStencilReadOnlyOptimal",
                "ShaderReadOnlyOptimal",
                "TransferSrcOptimal",
                "TransferDstOptimal",
                "Preinitialized",
                "DepthReadOnlyStencilAttachmentOptimal",
                "DepthAttachmentStencilReadOnlyOptimal",
                "DepthAttachmentOptimal",
                "DepthReadOnlyOptimal",
                "StencilAttachmentOptimal",
                "StencilReadOnlyOptimal",
                "PresentSrcKHR",
                "SharedPresentKHR",
                "ShadingRateOptimalNV",
                "FragmentDensityMapOptimalEXT",
                "DepthReadOnlyStencilAttachmentOptimalKHR",
                "DepthAttachmentStencilReadOnlyOptimalKHR",
                "DepthAttachmentOptimalKHR",
                "DepthReadOnlyOptimalKHR",
                "StencilAttachmentOptimalKHR",
                "StencilReadOnlyOptimalKHR"
            ]
    }
)foo");
}

template<>
inline nlohmann::json to_schema<vk::DescriptorType>()
{
    nlohmann::json J;
    J["type"] = "string";
    #define CH(VV) J["enum"].push_back( # VV )
    CH(Sampler);
    CH(CombinedImageSampler);
    CH(SampledImage);
    CH(StorageImage);
    CH(UniformTexelBuffer);
    CH(StorageTexelBuffer);
    CH(UniformBuffer);
    CH(StorageBuffer);
    CH(UniformBufferDynamic);
    CH(StorageBufferDynamic);
    CH(InputAttachment);
    CH(InlineUniformBlockEXT);
    CH(AccelerationStructureNV);
    #undef CH
    return J;
}


template<>
inline nlohmann::json to_schema<vk::CompareOp>()
{
    nlohmann::json J;
    J["type"] = "string";

#define CH(VV) J["enum"].push_back( vk::to_string( vk::CompareOp::e ## VV ) )
    CH(Never);
    CH(Less);
    CH(Equal);
    CH(LessOrEqual);
    CH(Greater);
    CH(NotEqual);
    CH(GreaterOrEqual);
    CH(Always);
#undef CH
    return J;
}

template<>
inline nlohmann::json to_schema<vk::PolygonMode>()
{
    nlohmann::json J;
    J["type"] = "string";
#define CH(VV) J["enum"].push_back( vk::to_string( vk::PolygonMode::e ## VV));
    CH(Fill);
    CH(Line);
    CH(Fill);
    CH(Point);
#undef CH
    return J;
}





template<>
inline nlohmann::json to_schema<vk::FrontFace>()
{
    nlohmann::json J;
    J["type"] = "string";
#define CH(VV) J["enum"].push_back( vk::to_string( vk::FrontFace::e ## VV));
    CH(CounterClockwise);
    CH(Clockwise);
#undef CH
    return J;
}


template<>
inline nlohmann::json to_schema<vk::VertexInputRate>()
{
    nlohmann::json J;
    J["type"] = "string";
#define CH(VV) J["enum"].push_back( vk::to_string( vk::VertexInputRate::e ## VV));
    CH(Vertex);
    CH(Instance);
#undef CH
    return J;
}

template<>
inline nlohmann::json to_schema<vk::AttachmentLoadOp>()
{
    nlohmann::json J;
    J["type"] = "string";
    #define CH(A) J["enum"].push_back( vk::to_string( vk::AttachmentLoadOp::e ## A));
    CH(Load)
    CH(Clear)
    CH(DontCare)
    #undef CH
    return J;
}

template<>
inline nlohmann::json to_schema<vk::AttachmentStoreOp>()
{
    nlohmann::json J;
    J["type"] = "string";
    #define CH(A) J["enum"].push_back( vk::to_string( vk::AttachmentStoreOp::e ## A));
    CH(Store)
    CH(DontCare)
    #undef CH
    return J;
}


template<>
inline nlohmann::json to_schema<vk::PipelineBindPoint>()
{
    nlohmann::json J;
    J["type"] = "string";
    J["enum"].push_back(vk::to_string(vk::PipelineBindPoint::eGraphics));
    J["enum"].push_back(vk::to_string(vk::PipelineBindPoint::eCompute));
    J["enum"].push_back(vk::to_string(vk::PipelineBindPoint::eRayTracingNV));

    return J;
}


template<>
inline nlohmann::json to_schema<vk::BlendFactor>()
{
    nlohmann::json J;
    J["type"] = "string";
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eZero) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eOne) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eSrcColor) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eOneMinusSrcColor) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eDstColor) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eOneMinusDstColor) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eSrcAlpha) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eOneMinusSrcAlpha) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eDstAlpha) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eOneMinusDstAlpha) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eConstantColor) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eOneMinusConstantColor) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eConstantAlpha) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eOneMinusConstantAlpha) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eSrcAlphaSaturate) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eSrc1Color) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eOneMinusSrc1Color) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eSrc1Alpha) );
    J["enum"].push_back( vk::to_string(vk::BlendFactor::eOneMinusSrc1Alpha) );

    return J;
}

template<>
inline nlohmann::json to_schema<vk::LogicOp>()
{
    nlohmann::json J;
    J["type"] = "string";
    J["enum"].push_back( vk::to_string( vk::LogicOp::eClear) );
    J["enum"].push_back( vk::to_string( vk::LogicOp::eAnd) );
    J["enum"].push_back( vk::to_string( vk::LogicOp::eAndReverse) );
    J["enum"].push_back( vk::to_string( vk::LogicOp::eCopy) );
    J["enum"].push_back( vk::to_string( vk::LogicOp::eAndInverted) );
    J["enum"].push_back( vk::to_string( vk::LogicOp::eNoOp) );
    J["enum"].push_back( vk::to_string( vk::LogicOp::eXor) );
    J["enum"].push_back( vk::to_string( vk::LogicOp::eOr) );
    J["enum"].push_back( vk::to_string( vk::LogicOp::eNor) );
    J["enum"].push_back( vk::to_string( vk::LogicOp::eEquivalent) );
    J["enum"].push_back( vk::to_string( vk::LogicOp::eInvert) );
    J["enum"].push_back( vk::to_string( vk::LogicOp::eOrReverse) );
    J["enum"].push_back( vk::to_string( vk::LogicOp::eCopyInverted) );
    J["enum"].push_back( vk::to_string( vk::LogicOp::eOrInverted) );
    J["enum"].push_back( vk::to_string( vk::LogicOp::eNand) );
    J["enum"].push_back( vk::to_string( vk::LogicOp::eSet) );
    return J;
}

template<>
inline nlohmann::json to_schema<vk::BlendOp>()
{
    nlohmann::json J;
    J["type"] = "string";
    J["enum"].push_back(vk::to_string(vk::BlendOp::eAdd));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eSubtract));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eReverseSubtract));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eMin));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eMax));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eZeroEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eSrcEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eDstEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eSrcOverEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eDstOverEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eSrcInEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eDstInEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eSrcOutEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eDstOutEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eSrcAtopEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eDstAtopEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eXorEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eMultiplyEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eScreenEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eOverlayEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eDarkenEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eLightenEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eColordodgeEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eColorburnEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eHardlightEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eSoftlightEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eDifferenceEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eExclusionEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eInvertEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eInvertRgbEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eLineardodgeEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eLinearburnEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eVividlightEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eLinearlightEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::ePinlightEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eHardmixEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eHslHueEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eHslSaturationEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eHslColorEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eHslLuminosityEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::ePlusEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::ePlusClampedEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::ePlusClampedAlphaEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::ePlusDarkerEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eMinusEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eMinusClampedEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eContrastEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eInvertOvgEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eRedEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eGreenEXT));
    J["enum"].push_back(vk::to_string(vk::BlendOp::eBlueEXT));

    return J;
}

template<>
inline nlohmann::json to_schema<vk::PrimitiveTopology>()
{
    nlohmann::json J;
    J["type"] = "string";
    J["enum"].push_back( vk::to_string(vk::PrimitiveTopology::ePointList));
    J["enum"].push_back( vk::to_string(vk::PrimitiveTopology::eLineList));
    J["enum"].push_back( vk::to_string(vk::PrimitiveTopology::eLineStrip));
    J["enum"].push_back( vk::to_string(vk::PrimitiveTopology::eTriangleList));
    J["enum"].push_back( vk::to_string(vk::PrimitiveTopology::eTriangleStrip));
    J["enum"].push_back( vk::to_string(vk::PrimitiveTopology::eTriangleFan));
    J["enum"].push_back( vk::to_string(vk::PrimitiveTopology::eLineListWithAdjacency));
    J["enum"].push_back( vk::to_string(vk::PrimitiveTopology::eLineStripWithAdjacency));
    J["enum"].push_back( vk::to_string(vk::PrimitiveTopology::eTriangleListWithAdjacency));
    J["enum"].push_back( vk::to_string(vk::PrimitiveTopology::eTriangleStripWithAdjacency));
    J["enum"].push_back( vk::to_string(vk::PrimitiveTopology::ePatchList));

    return J;
}


template<>
inline nlohmann::json to_schema<vk::Format>()
{
    nlohmann::json J;
    J["type"] = "string";
    #define CH(A) J["enum"].push_back( vk::to_string( vk::Format::e ## A));
    CH(R4G4UnormPack8)
    CH(R4G4B4A4UnormPack16)
    CH(B4G4R4A4UnormPack16)
    CH(R5G6B5UnormPack16)
    CH(B5G6R5UnormPack16)
    CH(R5G5B5A1UnormPack16)
    CH(B5G5R5A1UnormPack16)
    CH(A1R5G5B5UnormPack16)
    CH(R8Unorm)
    CH(R8Snorm)
    CH(R8Uscaled)
    CH(R8Sscaled)
    CH(R8Uint)
    CH(R8Sint)
    CH(R8Srgb)
    CH(R8G8Unorm)
    CH(R8G8Snorm)
    CH(R8G8Uscaled)
    CH(R8G8Sscaled)
    CH(R8G8Uint)
    CH(R8G8Sint)
    CH(R8G8Srgb)
    CH(R8G8B8Unorm)
    CH(R8G8B8Snorm)
    CH(R8G8B8Uscaled)
    CH(R8G8B8Sscaled)
    CH(R8G8B8Uint)
    CH(R8G8B8Sint)
    CH(R8G8B8Srgb)
    CH(B8G8R8Unorm)
    CH(B8G8R8Snorm)
    CH(B8G8R8Uscaled)
    CH(B8G8R8Sscaled)
    CH(B8G8R8Uint)
    CH(B8G8R8Sint)
    CH(B8G8R8Srgb)
    CH(R8G8B8A8Unorm)
    CH(R8G8B8A8Snorm)
    CH(R8G8B8A8Uscaled)
    CH(R8G8B8A8Sscaled)
    CH(R8G8B8A8Uint)
    CH(R8G8B8A8Sint)
    CH(R8G8B8A8Srgb)
    CH(B8G8R8A8Unorm)
    CH(B8G8R8A8Snorm)
    CH(B8G8R8A8Uscaled)
    CH(B8G8R8A8Sscaled)
    CH(B8G8R8A8Uint)
    CH(B8G8R8A8Sint)
    CH(B8G8R8A8Srgb)
    CH(A8B8G8R8UnormPack32)
    CH(A8B8G8R8SnormPack32)
    CH(A8B8G8R8UscaledPack32)
    CH(A8B8G8R8SscaledPack32)
    CH(A8B8G8R8UintPack32)
    CH(A8B8G8R8SintPack32)
    CH(A8B8G8R8SrgbPack32)
    CH(A2R10G10B10UnormPack32)
    CH(A2R10G10B10SnormPack32)
    CH(A2R10G10B10UscaledPack32)
    CH(A2R10G10B10SscaledPack32)
    CH(A2R10G10B10UintPack32)
    CH(A2R10G10B10SintPack32)
    CH(A2B10G10R10UnormPack32)
    CH(A2B10G10R10SnormPack32)
    CH(A2B10G10R10UscaledPack32)
    CH(A2B10G10R10SscaledPack32)
    CH(A2B10G10R10UintPack32)
    CH(A2B10G10R10SintPack32)
    CH(R16Unorm)
    CH(R16Snorm)
    CH(R16Uscaled)
    CH(R16Sscaled)
    CH(R16Uint)
    CH(R16Sint)
    CH(R16Sfloat)
    CH(R16G16Unorm)
    CH(R16G16Snorm)
    CH(R16G16Uscaled)
    CH(R16G16Sscaled)
    CH(R16G16Uint)
    CH(R16G16Sint)
    CH(R16G16Sfloat)
    CH(R16G16B16Unorm)
    CH(R16G16B16Snorm)
    CH(R16G16B16Uscaled)
    CH(R16G16B16Sscaled)
    CH(R16G16B16Uint)
    CH(R16G16B16Sint)
    CH(R16G16B16Sfloat)
    CH(R16G16B16A16Unorm)
    CH(R16G16B16A16Snorm)
    CH(R16G16B16A16Uscaled)
    CH(R16G16B16A16Sscaled)
    CH(R16G16B16A16Uint)
    CH(R16G16B16A16Sint)
    CH(R16G16B16A16Sfloat)
    CH(R32Uint)
    CH(R32Sint)
    CH(R32Sfloat)
    CH(R32G32Uint)
    CH(R32G32Sint)
    CH(R32G32Sfloat)
    CH(R32G32B32Uint)
    CH(R32G32B32Sint)
    CH(R32G32B32Sfloat)
    CH(R32G32B32A32Uint)
    CH(R32G32B32A32Sint)
    CH(R32G32B32A32Sfloat)
    CH(R64Uint)
    CH(R64Sint)
    CH(R64Sfloat)
    CH(R64G64Uint)
    CH(R64G64Sint)
    CH(R64G64Sfloat)
    CH(R64G64B64Uint)
    CH(R64G64B64Sint)
    CH(R64G64B64Sfloat)
    CH(R64G64B64A64Uint)
    CH(R64G64B64A64Sint)
    CH(R64G64B64A64Sfloat)
    CH(B10G11R11UfloatPack32)
    CH(E5B9G9R9UfloatPack32)
    CH(D16Unorm)
    CH(X8D24UnormPack32)
    CH(D32Sfloat)
    CH(S8Uint)
    CH(D16UnormS8Uint)
    CH(D24UnormS8Uint)
    CH(D32SfloatS8Uint)
    CH(Bc1RgbUnormBlock)
    CH(Bc1RgbSrgbBlock)
    CH(Bc1RgbaUnormBlock)
    CH(Bc1RgbaSrgbBlock)
    CH(Bc2UnormBlock)
    CH(Bc2SrgbBlock)
    CH(Bc3UnormBlock)
    CH(Bc3SrgbBlock)
    CH(Bc4UnormBlock)
    CH(Bc4SnormBlock)
    CH(Bc5UnormBlock)
    CH(Bc5SnormBlock)
    CH(Bc6HUfloatBlock)
    CH(Bc6HSfloatBlock)
    CH(Bc7UnormBlock)
    CH(Bc7SrgbBlock)
    CH(Etc2R8G8B8UnormBlock)
    CH(Etc2R8G8B8SrgbBlock)
    CH(Etc2R8G8B8A1UnormBlock)
    CH(Etc2R8G8B8A1SrgbBlock)
    CH(Etc2R8G8B8A8UnormBlock)
    CH(Etc2R8G8B8A8SrgbBlock)
    CH(EacR11UnormBlock)
    CH(EacR11SnormBlock)
    CH(EacR11G11UnormBlock)
    CH(EacR11G11SnormBlock)
    CH(Astc4x4UnormBlock)
    CH(Astc4x4SrgbBlock)
    CH(Astc5x4UnormBlock)
    CH(Astc5x4SrgbBlock)
    CH(Astc5x5UnormBlock)
    CH(Astc5x5SrgbBlock)
    CH(Astc6x5UnormBlock)
    CH(Astc6x5SrgbBlock)
    CH(Astc6x6UnormBlock)
    CH(Astc6x6SrgbBlock)
    CH(Astc8x5UnormBlock)
    CH(Astc8x5SrgbBlock)
    CH(Astc8x6UnormBlock)
    CH(Astc8x6SrgbBlock)
    CH(Astc8x8UnormBlock)
    CH(Astc8x8SrgbBlock)
    CH(Astc10x5UnormBlock)
    CH(Astc10x5SrgbBlock)
    CH(Astc10x6UnormBlock)
    CH(Astc10x6SrgbBlock)
    CH(Astc10x8UnormBlock)
    CH(Astc10x8SrgbBlock)
    CH(Astc10x10UnormBlock)
    CH(Astc10x10SrgbBlock)
    CH(Astc12x10UnormBlock)
    CH(Astc12x10SrgbBlock)
    CH(Astc12x12UnormBlock)
    CH(Astc12x12SrgbBlock)
    CH(G8B8G8R8422Unorm)
    CH(B8G8R8G8422Unorm)
    CH(G8B8R83Plane420Unorm)
    CH(G8B8R82Plane420Unorm)
    CH(G8B8R83Plane422Unorm)
    CH(G8B8R82Plane422Unorm)
    CH(G8B8R83Plane444Unorm)
    CH(R10X6UnormPack16)
    CH(R10X6G10X6Unorm2Pack16)
    CH(R10X6G10X6B10X6A10X6Unorm4Pack16)
    CH(G10X6B10X6G10X6R10X6422Unorm4Pack16)
    CH(B10X6G10X6R10X6G10X6422Unorm4Pack16)
    CH(G10X6B10X6R10X63Plane420Unorm3Pack16)
    CH(G10X6B10X6R10X62Plane420Unorm3Pack16)
    CH(G10X6B10X6R10X63Plane422Unorm3Pack16)
    CH(G10X6B10X6R10X62Plane422Unorm3Pack16)
    CH(G10X6B10X6R10X63Plane444Unorm3Pack16)
    CH(R12X4UnormPack16)
    CH(R12X4G12X4Unorm2Pack16)
    CH(R12X4G12X4B12X4A12X4Unorm4Pack16)
    CH(G12X4B12X4G12X4R12X4422Unorm4Pack16)
    CH(B12X4G12X4R12X4G12X4422Unorm4Pack16)
    CH(G12X4B12X4R12X43Plane420Unorm3Pack16)
    CH(G12X4B12X4R12X42Plane420Unorm3Pack16)
    CH(G12X4B12X4R12X43Plane422Unorm3Pack16)
    CH(G12X4B12X4R12X42Plane422Unorm3Pack16)
    CH(G12X4B12X4R12X43Plane444Unorm3Pack16)
    CH(G16B16G16R16422Unorm)
    CH(B16G16R16G16422Unorm)
    CH(G16B16R163Plane420Unorm)
    CH(G16B16R162Plane420Unorm)
    CH(G16B16R163Plane422Unorm)
    CH(G16B16R162Plane422Unorm)
    CH(G16B16R163Plane444Unorm)
    CH(Pvrtc12BppUnormBlockIMG)
    CH(Pvrtc14BppUnormBlockIMG)
    CH(Pvrtc22BppUnormBlockIMG)
    CH(Pvrtc24BppUnormBlockIMG)
    CH(Pvrtc12BppSrgbBlockIMG)
    CH(Pvrtc14BppSrgbBlockIMG)
    CH(Pvrtc22BppSrgbBlockIMG)
    CH(Pvrtc24BppSrgbBlockIMG)
    CH(Astc4x4SfloatBlockEXT)
    CH(Astc5x4SfloatBlockEXT)
    CH(Astc5x5SfloatBlockEXT)
    CH(Astc6x5SfloatBlockEXT)
    CH(Astc6x6SfloatBlockEXT)
    CH(Astc8x5SfloatBlockEXT)
    CH(Astc8x6SfloatBlockEXT)
    CH(Astc8x8SfloatBlockEXT)
    CH(Astc10x5SfloatBlockEXT)
    CH(Astc10x6SfloatBlockEXT)
    CH(Astc10x8SfloatBlockEXT)
    CH(Astc10x10SfloatBlockEXT)
    CH(Astc12x10SfloatBlockEXT)
    CH(Astc12x12SfloatBlockEXT)
    CH(G8B8G8R8422UnormKHR)
    CH(B8G8R8G8422UnormKHR)
    CH(G8B8R83Plane420UnormKHR)
    CH(G8B8R82Plane420UnormKHR)
    CH(G8B8R83Plane422UnormKHR)
    CH(G8B8R82Plane422UnormKHR)
    CH(G8B8R83Plane444UnormKHR)
    CH(R10X6UnormPack16KHR)
    CH(R10X6G10X6Unorm2Pack16KHR)
    CH(R10X6G10X6B10X6A10X6Unorm4Pack16KHR)
    CH(G10X6B10X6G10X6R10X6422Unorm4Pack16KHR)
    CH(B10X6G10X6R10X6G10X6422Unorm4Pack16KHR)
    CH(G10X6B10X6R10X63Plane420Unorm3Pack16KHR)
    CH(G10X6B10X6R10X62Plane420Unorm3Pack16KHR)
    CH(G10X6B10X6R10X63Plane422Unorm3Pack16KHR)
    CH(G10X6B10X6R10X62Plane422Unorm3Pack16KHR)
    CH(G10X6B10X6R10X63Plane444Unorm3Pack16KHR)
    CH(R12X4UnormPack16KHR)
    CH(R12X4G12X4Unorm2Pack16KHR)
    CH(R12X4G12X4B12X4A12X4Unorm4Pack16KHR)
    CH(G12X4B12X4G12X4R12X4422Unorm4Pack16KHR)
    CH(B12X4G12X4R12X4G12X4422Unorm4Pack16KHR)
    CH(G12X4B12X4R12X43Plane420Unorm3Pack16KHR)
    CH(G12X4B12X4R12X42Plane420Unorm3Pack16KHR)
    CH(G12X4B12X4R12X43Plane422Unorm3Pack16KHR)
    CH(G12X4B12X4R12X42Plane422Unorm3Pack16KHR)
    CH(G12X4B12X4R12X43Plane444Unorm3Pack16KHR)
    CH(G16B16G16R16422UnormKHR)
    CH(B16G16R16G16422UnormKHR)
    CH(G16B16R163Plane420UnormKHR)
    CH(G16B16R162Plane420UnormKHR)
    CH(G16B16R163Plane422UnormKHR)
    CH(G16B16R162Plane422UnormKHR)
    CH(G16B16R163Plane444UnormKHR)
    #undef CH
    return J;
}



//=========================================================================
// FlagBits
//=========================================================================
template<>
inline nlohmann::json to_schema<vk::ShaderStageFlagBits>()
{
    return
    nlohmann::json::parse(
R"foo(
     {
            "type": "string",
            "enum": [
                    "Vertex",
                    "TessellationControl",
                    "TessellationEvaluation",
                    "Geometry",
                    "Fragment",
                    "Compute",
                    "RaygenNV",
                    "AnyHitNV",
                    "ClosestHitNV",
                    "MissNV",
                    "IntersectionNV",
                    "CallableNV",
                    "TaskNV",
                    "MeshNV"
            ]
    }
)foo");
}

template<>
inline nlohmann::json to_schema<vk::DescriptorSetLayoutCreateFlagBits>()
{
    nlohmann::json J;
    J["type"] = "string";
#define CH(VV) J["enum"].push_back(  vk::to_string( vk::DescriptorSetLayoutCreateFlagBits::e ## VV ) );
    CH(UpdateAfterBindPool);
    CH(PushDescriptorKHR);
    CH(UpdateAfterBindPoolEXT);
    #undef CH
    return J;
}

template<>
inline nlohmann::json to_schema<vk::ColorComponentFlagBits>()
{
    return
    nlohmann::json::parse(
R"foo(
     {
            "type": "string",
            "enum": [
                    "R",
                    "G",
                    "B",
                    "A"
            ]
    }
)foo");
}

template<>
inline nlohmann::json to_schema<vk::SampleCountFlagBits>()
{
    nlohmann::json J;
    J["type"] = "string";
    J["enum"].push_back( "1"   );
    J["enum"].push_back( "2"   );
    J["enum"].push_back( "4"   );
    J["enum"].push_back( "8"   );
    J["enum"].push_back( "16"  );
    J["enum"].push_back( "32"  );
    J["enum"].push_back( "64"  );
    return J;
}

template<>
inline nlohmann::json to_schema<vk::CullModeFlagBits>()
{
    nlohmann::json J;
    J["type"] = "string";
#define CH(VV) J["enum"].push_back( vk::to_string( vk::CullModeFlagBits::e ## VV));
    CH(None);
    CH(Front);
    CH(Back);
    CH(FrontAndBack);
#undef CH
    return J;
}

template<>
inline nlohmann::json to_schema<vk::PipelineStageFlagBits>()
{
    nlohmann::json J;
    J["type"] = "string";
    #define CH(A) J["enum"].push_back( vk::to_string( vk::PipelineStageFlagBits::e ## A));
    CH(TopOfPipe)
    CH(DrawIndirect)
    CH(VertexInput)
    CH(VertexShader)
    CH(TessellationControlShader)
    CH(TessellationEvaluationShader)
    CH(GeometryShader)
    CH(FragmentShader)
    CH(EarlyFragmentTests)
    CH(LateFragmentTests)
    CH(ColorAttachmentOutput)
    CH(ComputeShader)
    CH(Transfer)
    CH(BottomOfPipe)
    CH(Host)
    CH(AllGraphics)
    CH(AllCommands)
    CH(TransformFeedbackEXT)
    CH(ConditionalRenderingEXT)
    CH(CommandProcessNVX)
    CH(ShadingRateImageNV)
    CH(RayTracingShaderNV)
    CH(AccelerationStructureBuildNV)
    CH(TaskShaderNV)
    CH(MeshShaderNV)
    CH(FragmentDensityProcessEXT)
    #undef CH
    return J;
}

template<>
inline nlohmann::json to_schema<vk::AccessFlagBits>()
{
    nlohmann::json J;
    J["type"] = "string";
    #define CH(A) J["enum"].push_back( vk::to_string( vk::AccessFlagBits::e ## A));
    CH(IndirectCommandRead)
    CH(IndexRead)
    CH(VertexAttributeRead)
    CH(UniformRead)
    CH(InputAttachmentRead)
    CH(ShaderRead)
    CH(ShaderWrite)
    CH(ColorAttachmentRead)
    CH(ColorAttachmentWrite)
    CH(DepthStencilAttachmentRead)
    CH(DepthStencilAttachmentWrite)
    CH(TransferRead)
    CH(TransferWrite)
    CH(HostRead)
    CH(HostWrite)
    CH(MemoryRead)
    CH(MemoryWrite)
    CH(TransformFeedbackWriteEXT)
    CH(TransformFeedbackCounterReadEXT)
    CH(TransformFeedbackCounterWriteEXT)
    CH(ConditionalRenderingReadEXT)
    CH(CommandProcessReadNVX)
    CH(CommandProcessWriteNVX)
    CH(ColorAttachmentReadNoncoherentEXT)
    CH(ShadingRateImageReadNV)
    CH(AccelerationStructureReadNV)
    CH(AccelerationStructureWriteNV)
    CH(FragmentDensityMapReadEXT)
    #undef CH
    return J;
}

template<>
inline nlohmann::json to_schema<vk::DependencyFlagBits>()
{
    nlohmann::json J;
    J["type"] = "string";
    #define CH(A) J["enum"].push_back( vk::to_string( vk::DependencyFlagBits::e ## A));
    CH(ByRegion)
    CH(DeviceGroup)
    CH(ViewLocal)
    CH(ViewLocalKHR)
    CH(DeviceGroupKHR)
    #undef CH
    return J;
}

//=========================================================================




//=========================================================================
// Flags
//=========================================================================
template<>
inline nlohmann::json to_schema<vk::CullModeFlags>()
{
    nlohmann::json J;
    J["type"] = "array";
    J["items"] = to_schema<vk::CullModeFlagBits>();
    J["uniqueItems"] = true;
    return J;
}

template<>
inline nlohmann::json to_schema<vk::ShaderStageFlags>()
{
    auto J =
    nlohmann::json::parse(
R"foo(
     {
            "type": "array",
            "uniqueItems" : true
    }
)foo");
    J["items"] = to_schema<vk::ShaderStageFlagBits>();
    return J;
}




template<>
inline nlohmann::json to_schema<vk::ColorComponentFlags>()
{
    auto J =
    nlohmann::json::parse(
R"foo(
     {
            "type": "array",
            "uniqueItems" : true
    }
)foo");
    J["items"] = to_schema<vk::ColorComponentFlagBits>();
    return J;
}




template<>
inline nlohmann::json to_schema<vk::SampleCountFlags>()
{
    nlohmann::json J;
    J["type"] = "array";
    J["items"] = to_schema<vk::SampleCountFlagBits>();
    J["uniqueItems"] = true;
    return J;
}




template<>
inline nlohmann::json to_schema<vk::PipelineStageFlags>()
{
    auto J =
    nlohmann::json::parse(
    R"foo(
     {
            "type": "array",
            "uniqueItems" : true
    }
    )foo");
    J["items"] = to_schema<vk::PipelineStageFlagBits>();
    return J;
}

template<>
inline nlohmann::json to_schema<vk::DependencyFlags>()
{
    auto J =
    nlohmann::json::parse(
    R"foo(
     {
            "type": "array",
            "uniqueItems" : true
    }
    )foo");
    J["items"] = to_schema<vk::DependencyFlagBits>();
    return J;
}


template<>
inline nlohmann::json to_schema<vk::AccessFlags>()
{
    auto J =
    nlohmann::json::parse(
    R"foo(
     {
            "type": "array",
            "uniqueItems" : true
    }
    )foo");
    J["items"] = to_schema<vk::AccessFlagBits>();
    return J;
}

template<>
inline nlohmann::json to_schema<vk::DescriptorSetLayoutCreateFlags>()
{
    auto J =
    nlohmann::json::parse(
R"foo(
     {
            "type": "array",
            "uniqueItems" : true
    }
)foo");
    J["items"] = to_schema<vk::DescriptorSetLayoutCreateFlagBits>();
    return J;
}

//=========================================================================



//=========================================================================
// Simple Structs
//=========================================================================
template<>
inline nlohmann::json to_schema<vk::Extent2D>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["properties"]["width"]  = to_schema<uint32_t>();
    J["properties"]["height"] = to_schema<uint32_t>();
    J["ui:order"] = { "width", "height"};
    return J;
}

template<>
inline nlohmann::json to_schema<vk::Offset2D>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["properties"]["x"] = to_schema<int32_t>();
    J["properties"]["y"] = to_schema<int32_t>();
    return J;
}

template<>
inline nlohmann::json to_schema<vk::Rect2D>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["properties"]["extent"] = to_schema<vk::Extent2D>();
    J["properties"]["offset"] = to_schema<vk::Offset2D>();
    J["ui:order"] = { "offset", "extent"};
    return J;
}

template<>
inline nlohmann::json to_schema<vk::Viewport>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["properties"]["x"]        = to_schema<float>();
    J["properties"]["y"]        = to_schema<float>();
    J["properties"]["width"]    = to_schema<float>();
    J["properties"]["height"]   = to_schema<float>();
    J["properties"]["maxDepth"] = to_schema<float>();
    J["properties"]["minDepth"] = to_schema<float>();
    J["ui:order"] = { "x", "y", "width", "height", "minDepth", "maxDepth"};
    return J;
}

//=========================================================================



//=========================================================================
// CreateInfo members
//=========================================================================

template<>
inline nlohmann::json to_schema<vk::PushConstantRange>()
{
    auto J =
    nlohmann::json::parse(
R"foo(
     {
            "type": "object",
            "properties" : {
                "size" : { "type" : "integer", "minimum" : 0 },
                "offset" : { "type" : "integer", "minimum" : 0 }
            }
    }
)foo");
    J["properties"]["stageFlags"] = to_schema<vk::ShaderStageFlags>();
    return J;
}

template<>
inline nlohmann::json to_schema<vk::DescriptorSetLayoutBinding>()
{
    auto J =
    nlohmann::json::parse(
R"foo(
     {
            "type": "object",
            "properties" : {
                "binding" : { "type" : "integer", "minimum" : 0 },
                "descriptorCount" : { "type" : "integer", "minimum" : 0 }
            }
    }
)foo");
    J["properties"]["descriptorType"] = to_schema<vk::DescriptorType>();
    J["properties"]["stageFlags"] = to_schema<vk::ShaderStageFlags>();
    return J;
}

template<>
inline nlohmann::json to_schema<vk::AttachmentReference>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["properties"]["attachment"] = to_schema<uint32_t>();
    J["properties"]["layout"]     = to_schema<vk::ImageLayout>();
    return J;
}


template<>
inline nlohmann::json to_schema<vk::VertexInputBindingDescription>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["properties"]["stride"]    = to_schema<uint32_t>();
    J["properties"]["binding"]   = to_schema<uint32_t>();
    J["properties"]["inputRate"] = to_schema<vk::VertexInputRate>();
    J["ui:order"] = { "binding", "stride", "inputRate"};
    return J;
}

template<>
inline nlohmann::json to_schema<vk::AttachmentDescription>()
{
    nlohmann::json J;
    J["type"] = "object";

   J["properties"]["format"] = to_schema<vk::Format>();
   J["properties"]["samples"] = to_schema<vk::SampleCountFlagBits>();
   J["properties"]["loadOp"] = to_schema<vk::AttachmentLoadOp>();
   J["properties"]["storeOp"] = to_schema<vk::AttachmentStoreOp>();
   J["properties"]["stencilLoadOp"] = to_schema<vk::AttachmentLoadOp>();
   J["properties"]["stencilStoreOp"] = to_schema<vk::AttachmentStoreOp>();
   J["properties"]["initialLayout"] = to_schema<vk::ImageLayout>();
   J["properties"]["finalLayout"] = to_schema<vk::ImageLayout>();

    return J;
}

template<>
inline nlohmann::json to_schema<vk::SubpassDependency>()
{
    nlohmann::json J;
    J["type"] = "object";

    J["properties"]["srcSubpass"]      = to_schema<uint32_t>();
    J["properties"]["dstSubpass"]      = to_schema<uint32_t>();
    J["properties"]["srcStageMask"]    = to_schema<vk::PipelineStageFlags>();
    J["properties"]["dstStageMask"]    = to_schema<vk::PipelineStageFlags>();
    J["properties"]["srcAccessMask"]   = to_schema<vk::AccessFlags>();
    J["properties"]["dstAccessMask"]   = to_schema<vk::AccessFlags>();
    J["properties"]["dependencyFlags"] = to_schema<vk::DependencyFlags>();

    J["ui:order"] =
    {
        "srcSubpass", "srcStageMask", "srcAccessMask",
        "dstSubpass", "dstStageMask", "dstAccessMask",
            "dependencyFlags"
    };
    return J;
}

template<>
inline nlohmann::json to_schema<vk::VertexInputAttributeDescription>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["properties"]["location"] = to_schema<uint32_t>();
    J["properties"]["binding"]  = to_schema<uint32_t>();
    J["properties"]["offset"]   = to_schema<uint32_t>();
    J["properties"]["format"]   = to_schema<vk::Format>();

    J["ui:order"] = { "location", "binding", "format", "offset"};
    return J;
}


//=========================================================================




//=========================================================================
// CreateInfos
//=========================================================================
template<>
inline nlohmann::json to_schema<vk::PipelineTessellationStateCreateInfo>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["properties"]["patchControlPoints"] = to_schema<uint32_t>();
    return J;
}


template<>
inline nlohmann::json to_schema<vk::PipelineInputAssemblyStateCreateInfo>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["properties"]["topology"] = to_schema<vk::PrimitiveTopology>();
    J["properties"]["primitiveRestartEnable"] = to_schema<bool>();

    return J;
}


template<>
inline nlohmann::json to_schema<vk::PipelineColorBlendAttachmentState>()
{
    nlohmann::json J;
    J["type"] = "object";

    #define SCH(STRUCT, var) J["properties"][ # var] = to_schema< decltype( STRUCT::var ) >()

    J["properties"]["blendEnable"] = to_schema< bool >();
    SCH(vk::PipelineColorBlendAttachmentState,srcColorBlendFactor);//( srcColorBlendFactor_ )
    SCH(vk::PipelineColorBlendAttachmentState,dstColorBlendFactor);//( dstColorBlendFactor_ )
    SCH(vk::PipelineColorBlendAttachmentState,colorBlendOp       );//( colorBlendOp_ )
    SCH(vk::PipelineColorBlendAttachmentState,srcAlphaBlendFactor);//( srcAlphaBlendFactor_ )
    SCH(vk::PipelineColorBlendAttachmentState,dstAlphaBlendFactor);//( dstAlphaBlendFactor_ )
    SCH(vk::PipelineColorBlendAttachmentState,alphaBlendOp       );//( alphaBlendOp_ )
    SCH(vk::PipelineColorBlendAttachmentState,colorWriteMask     );//( colorWriteMask_ )
    #undef SCH
    J["properties"]["colorBlendOp"       ]["default"] = vk::to_string(vk::BlendOp::eAdd);
    J["properties"]["srcAlphaBlendFactor"]["default"] = vk::to_string(vk::BlendFactor::eSrcAlpha);
    J["properties"]["srcColorBlendFactor"]["default"] = vk::to_string(vk::BlendFactor::eSrcAlpha);
    J["properties"]["dstAlphaBlendFactor"]["default"] = vk::to_string(vk::BlendFactor::eOneMinusDstAlpha);
    J["properties"]["dstColorBlendFactor"]["default"] = vk::to_string(vk::BlendFactor::eOneMinusDstAlpha);

    return J;
}


template<>
inline nlohmann::json to_schema<vk::PipelineDepthStencilStateCreateInfo>()
{
    nlohmann::json J;
    J["type"] = "object";

    #define SCH(STRUCT, var) J["properties"][ # var] = to_schema< decltype( STRUCT::var ) >()

    J["properties"]["depthBoundsTestEnable"] = to_schema< bool >();
    J["properties"]["depthTestEnable"]       = to_schema< bool >();
    J["properties"]["depthWriteEnable"]      = to_schema< bool >();
    J["properties"]["stencilTestEnable"]     = to_schema< bool >();

    //SCH(vk::PipelineDepthStencilStateCreateInfo,depthBoundsTestEnable);//( srcColorBlendFactor_ )
    SCH(vk::PipelineDepthStencilStateCreateInfo,maxDepthBounds);//( dstColorBlendFactor_ )
    SCH(vk::PipelineDepthStencilStateCreateInfo,minDepthBounds       );//( colorBlendOp_ )
    //SCH(vk::PipelineDepthStencilStateCreateInfo,depthTestEnable);//( srcAlphaBlendFactor_ )
    //SCH(vk::PipelineDepthStencilStateCreateInfo,depthWriteEnable);//( dstAlphaBlendFactor_ )
    //SCH(vk::PipelineDepthStencilStateCreateInfo,stencilTestEnable       );//( alphaBlendOp_ )
    SCH(vk::PipelineDepthStencilStateCreateInfo,depthCompareOp     );//( colorWriteMask_ )


    #undef SCH
    return J;
}

template<>
inline nlohmann::json to_schema<vk::PipelineMultisampleStateCreateInfo>()
{
    nlohmann::json J;
    J["type"] = "object";

    J["properties"]["alphaToCoverageEnable"] = to_schema< bool >();
    J["properties"]["sampleShadingEnable"]   = to_schema< bool >();
    J["properties"]["alphaToCoverageEnable"] = to_schema< bool >();
    J["properties"]["alphaToCoverageEnable"] = to_schema< bool >();
    J["properties"]["minSampleShading"] = to_schema< float >();
    J["properties"]["rasterizationSamples"] = to_schema< vk::SampleCountFlagBits >();
    J["properties"]["depthStencilSamples"] = to_schema< vk::SampleCountFlags >();
    J["properties"]["colorSamples"] = to_schema< vk::SampleCountFlags >();
    return J;
}


template<>
inline nlohmann::json to_schema<vk::PipelineRasterizationStateCreateInfo>()
{
    nlohmann::json J;
    J["type"] = "object";


    J["properties"]["depthClampEnable"]      = to_schema< bool >();
    J["properties"]["rasterizerDiscardEnable"]  = to_schema< bool >();
    J["properties"]["polygonMode"] = to_schema< vk::PolygonMode >();
    J["properties"]["cullMode"] = to_schema< vk::CullModeFlags>();
    J["properties"]["frontFace"]   = to_schema< vk::FrontFace>();

    J["properties"]["depthBiasEnable"]          = to_schema<bool>();
    J["properties"]["depthBiasConstantFactor"]  = to_schema<float>();
    J["properties"]["depthBiasClamp"]           = to_schema<bool>();
    J["properties"]["depthBiasSlopeFactor"]     = to_schema<float>();
    J["properties"]["lineWidth"]                = to_schema<float>();

    return J;
}


//=========================================================================


//=========================================================================
// vkb::CreateInfo2s
//=========================================================================
template<>
inline nlohmann::json to_schema<DescriptorSetLayoutCreateInfo2>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["properties"]["bindings"]      = to_schema_vector<vk::DescriptorSetLayoutBinding>();
    J["properties"]["flags"]  = to_schema<vk::DescriptorSetLayoutCreateFlags>();

   // std::vector<vkb::PipelineShaderStageCreateInfo2>      stages;

   // std::vector<vk::DynamicState>                         dynamicStates;

   //
   // std::variant< vkb::PipelineLayoutCreateInfo2,
   //               vk::PipelineLayout>                     layout;
   //
   // std::variant< vkb::RenderPassCreateInfo2,
   //               vk::RenderPass>                         renderPass;
    //=======================================================================
    return J;
}

template<>
inline nlohmann::json to_schema<PipelineLayoutCreateInfo2>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["properties"]["setLayouts"] = vkb::to_schema_vector< vkb::DescriptorSetLayoutCreateInfo2>();
    J["properties"]["pushConstantRanges"] = vkb::to_schema_vector< vk::PushConstantRange>();
    return J;
}

template<>
inline nlohmann::json to_schema<PipelineViewportStateCreateInfo2>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["properties"]["viewports"] = to_schema_vector<vk::Viewport>( std::vector<vk::Viewport>(1) ) ;
    J["properties"]["scissors"]  = to_schema_vector<vk::Rect2D>(   std::vector<vk::Rect2D>(1) ) ;
    return J;
}


template<>
inline nlohmann::json to_schema<PipelineColorBlendStateCreateInfo2>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["properties"]["logicOpEnable"]  = to_schema<bool>();
    J["properties"]["logicOp"]        = to_schema<vk::LogicOp>();
    J["properties"]["blendConstants"] = to_schema_array<float,4>();
    J["properties"]["attachments"]    = to_schema_vector<vk::PipelineColorBlendAttachmentState>();
    return J;
}

template<>
inline nlohmann::json to_schema<PipelineVertexInputStateCreateInfo2>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["properties"]["vertexBindingDescriptions"]    = to_schema_vector<vk::VertexInputBindingDescription>();
    J["properties"]["vertexAttributeDescriptions"]    = to_schema_vector<vk::VertexInputAttributeDescription>();
    return J;
}


template<>
inline nlohmann::json to_schema<vkb::SubpassDescription2>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["properties"]["pipelineBindPoint"]      = to_schema<vk::PipelineBindPoint>();
    J["properties"]["inputAttachments"]       = to_schema_vector<vk::AttachmentReference>();
    J["properties"]["colorAttachments"]       = to_schema_vector<vk::AttachmentReference>();
    J["properties"]["depthStencilAttachment"] = to_schema<vk::AttachmentReference>();

    //J["properties"]["resolveAttachment"] = to_schema<vk::AttachmentReference>();
    J["ui:order"] = {"pipelineBindPoint", "inputAttachments", "colorAttachments", "depthStencilAttachment"};
    return J;
}

template<>
inline nlohmann::json to_schema<RenderPassCreateInfo2>()
{
    nlohmann::json J;
    J["type"] = "object";

    J["properties"]["attachments"] = to_schema_vector<vk::AttachmentDescription>();
    J["properties"]["dependencies"] = to_schema_vector<vk::SubpassDependency>();
    J["properties"]["subpasses"] = to_schema_vector<vkb::SubpassDescription2>();


    return J;
}

template<>
inline nlohmann::json to_schema<GraphicsPipelineCreateInfo2>()
{
    nlohmann::json J;
    J["type"] = "object";
    J["ui:widget"] = "tabs";
    J["properties"]["viewportState"]      = to_schema<PipelineViewportStateCreateInfo2>();
    J["properties"]["tessellationState"]  = to_schema<vk::PipelineTessellationStateCreateInfo>();
    J["properties"]["inputAssemblyState"] = to_schema<vk::PipelineInputAssemblyStateCreateInfo>();
    J["properties"]["depthStencilState"]  = to_schema<vk::PipelineDepthStencilStateCreateInfo>();
    J["properties"]["blendState"]         = to_schema<vkb::PipelineColorBlendStateCreateInfo2>();
    J["properties"]["vertexInputState"]   = to_schema<vkb::PipelineVertexInputStateCreateInfo2>();
    J["properties"]["rasterizationState"] = to_schema<vk::PipelineRasterizationStateCreateInfo>();
    J["properties"]["multisampleState"]   = to_schema<vk::PipelineMultisampleStateCreateInfo>();
    J["properties"]["layout"]             = to_schema<vkb::PipelineLayoutCreateInfo2>();
    J["properties"]["renderPass"]         = to_schema<vkb::RenderPassCreateInfo2>();

   // std::vector<vkb::PipelineShaderStageCreateInfo2>      stages;

   // std::vector<vk::DynamicState>                         dynamicStates;

   //
   // std::variant< vkb::PipelineLayoutCreateInfo2,
   //               vk::PipelineLayout>                     layout;
   //
   // std::variant< vkb::RenderPassCreateInfo2,
   //               vk::RenderPass>                         renderPass;
    //=======================================================================
    return J;
}
















}
#endif



