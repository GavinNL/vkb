#ifndef VKJSON_VERTEXINPUTATTRIBUTEDESCRIPTION_H
#define VKJSON_VERTEXINPUTATTRIBUTEDESCRIPTION_H

#include <nlohmann/json.hpp>
#include <vulkan/vulkan.hpp>
#include <fstream>

namespace vk
{

void from_json(const nlohmann::json & j, vk::VertexInputBindingDescription& p);
void from_json(const nlohmann::json & j, vk::VertexInputAttributeDescription& p);
void from_json(const nlohmann::json & j, vk::PipelineColorBlendAttachmentState& p);
void from_json(const nlohmann::json & j, vk::Viewport& p);
void from_json(const nlohmann::json & j, vk::Rect2D& p);
void from_json(const nlohmann::json & j, vk::Offset2D& p);
void from_json(const nlohmann::json & j, vk::Extent2D& p);

}


namespace vk
{

inline void from_json(const nlohmann::json & j, vk::PushConstantRange& p)
{
    j.at("size").get_to(p.size);
    j.at("offset").get_to(p.offset);

    auto & S = j.at("stageFlags");

    p.stageFlags = {};

    if( S.is_array() )
    {
        for(auto & s : S)
        {
            auto str = s.get<std::string>();
            #define CH( VV ) if( str == #VV )  p.stageFlags |= vk::ShaderStageFlagBits::e ## VV
            CH(AnyHitNV);
            CH(CallableNV);
            CH(ClosestHitNV);
            CH(Compute);
            CH(Fragment);
            CH(Geometry);
            CH(IntersectionNV);
            CH(MeshNV);
            CH(MissNV);
            CH(RaygenNV);
            CH(TaskNV);
            CH(TessellationControl);
            CH(TessellationEvaluation);
            CH(Vertex);
            #undef CH
        }
    }
}

inline void from_json(const nlohmann::json & j, vk::DescriptorSetLayoutBinding& p)
{
    j.at("binding").get_to(p.binding);
    j.at("descriptorCount").get_to(p.descriptorCount);

    p.descriptorType = vk::DescriptorType::eSampler;
    auto tstr = j.at("descriptorType").get<std::string>();

    #define CH(VV) if( tstr == #VV ) p.descriptorType = vk::DescriptorType::e ## VV
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

    auto & S = j.at("stageFlags");

    p.stageFlags = {};

    if( S.is_array() )
    {
        for(auto & s : S)
        {
            auto str = s.get<std::string>();
            #define CH( VV ) if( str == #VV )  p.stageFlags |= vk::ShaderStageFlagBits::e ## VV
            CH(AnyHitNV);
            CH(CallableNV);
            CH(ClosestHitNV);
            CH(Compute);
            CH(Fragment);
            CH(Geometry);
            CH(IntersectionNV);
            CH(MeshNV);
            CH(MissNV);
            CH(RaygenNV);
            CH(TaskNV);
            CH(TessellationControl);
            CH(TessellationEvaluation);
            CH(Vertex);
            #undef CH
        }
    }

}

inline void from_json(const nlohmann::json & j, vk::ImageLayout& p)
{
    if(j.is_string())
    {
        auto const & str = j.get<std::string>();
        #define CC(VV) if( str == # VV) { p = vk::ImageLayout::e ## VV; return; }
        CC(Undefined)
        CC(General)
        CC(ColorAttachmentOptimal)
        CC(DepthStencilAttachmentOptimal)
        CC(DepthStencilReadOnlyOptimal)
        CC(ShaderReadOnlyOptimal)
        CC(TransferSrcOptimal)
        CC(TransferDstOptimal)
        CC(Preinitialized)
        CC(DepthReadOnlyStencilAttachmentOptimal)
        CC(DepthAttachmentStencilReadOnlyOptimal)
        CC(DepthAttachmentOptimal)
        CC(DepthReadOnlyOptimal)
        CC(StencilAttachmentOptimal)
        CC(StencilReadOnlyOptimal)
        CC(PresentSrcKHR)
        CC(SharedPresentKHR)
        CC(ShadingRateOptimalNV)
        CC(FragmentDensityMapOptimalEXT)
        CC(DepthReadOnlyStencilAttachmentOptimalKHR)
        CC(DepthAttachmentStencilReadOnlyOptimalKHR)
        CC(DepthAttachmentOptimalKHR)
        CC(DepthReadOnlyOptimalKHR)
        CC(StencilAttachmentOptimalKHR)
        CC(StencilReadOnlyOptimalKHR)

        #undef CC
    }

}

inline void from_json(const nlohmann::json & j, vk::AttachmentReference& p)
{
    j.at("attachment").get_to(p.attachment);
    p.layout = j.at("layout").get<vk::ImageLayout>();
}

inline void from_json(const nlohmann::json & j, vk::Extent2D& p)
{
    if( j.is_object())
    {
        j.at("width").get_to(p.width);
        j.at("height").get_to(p.height);
    }
    else if( j.is_array() && j.size() >= 2)
    {
        j.at(0).get_to(p.width);
        j.at(1).get_to(p.height);
    }
}

inline void from_json(const nlohmann::json & j, vk::Offset2D& p)
{
    if( j.is_object())
    {
        j.at("x").get_to(p.x);
        j.at("y").get_to(p.y);
    }
    else if( j.is_array() && j.size() >= 2)
    {
        j.at(0).get_to(p.x);
        j.at(1).get_to(p.y);
    }
}


inline void from_json(const nlohmann::json & j, vk::Rect2D& p)
{
    p.extent = j.at("extent").get<vk::Extent2D>();
    p.offset = j.at("offset").get<vk::Offset2D>();
}

inline void from_json(const nlohmann::json & j, vk::Viewport& p)
{
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
    j.at("width").get_to(p.width);
    j.at("height").get_to(p.height);
    j.at("maxDepth").get_to(p.maxDepth);
    j.at("minDepth").get_to(p.minDepth);
}

inline void from_json(const nlohmann::json & j, vk::PipelineTessellationStateCreateInfo& p)
{
    j.at("patchControlPoints").get_to(p.patchControlPoints);
}


inline void from_json(const nlohmann::json & j, vk::PipelineInputAssemblyStateCreateInfo& p)
{


    auto getTopology = [](std::string const & str)
    {
        if( str == "PointList" ) return vk::PrimitiveTopology::ePointList;
        else if( str == "LineList" ) return vk::PrimitiveTopology::eLineList;
        else if( str == "LineStrip" ) return vk::PrimitiveTopology::eLineStrip;
        else if( str == "TriangleList" ) return vk::PrimitiveTopology::eTriangleList;
        else if( str == "TriangleStrip" ) return vk::PrimitiveTopology::eTriangleStrip;
        else if( str == "TriangleFan" ) return vk::PrimitiveTopology::eTriangleFan;
        else if( str == "LineListWithAdjacency" ) return vk::PrimitiveTopology::eLineListWithAdjacency;
        else if( str == "LineStripWithAdjacency" ) return vk::PrimitiveTopology::eLineStripWithAdjacency;
        else if( str == "TriangleListWithAdjacency" ) return vk::PrimitiveTopology::eTriangleListWithAdjacency;
        else if( str == "TriangleStripWithAdjacency" ) return vk::PrimitiveTopology::eTriangleStripWithAdjacency;
        else if( str == "PatchList" ) return vk::PrimitiveTopology::ePatchList;
    };


    p.topology  =  getTopology(j.at("topology").get<std::string>() );
    j.at("primitiveRestartEnable").get_to(p.primitiveRestartEnable);
}

inline void from_json(const nlohmann::json & j, vk::PipelineColorBlendAttachmentState& p)
{


    auto getBlendFactor = [](std::string const & str)
    {
        if( str == "Zero" )  return vk::BlendFactor::eZero;
        else if( str == "One" )  return vk::BlendFactor::eOne;
        else if( str == "SrcColor" )  return vk::BlendFactor::eSrcColor;
        else if( str == "OneMinusSrcColor" )  return vk::BlendFactor::eOneMinusSrcColor;
        else if( str == "DstColor" )  return vk::BlendFactor::eDstColor;
        else if( str == "OneMinusDstColor" )  return vk::BlendFactor::eOneMinusDstColor;
        else if( str == "SrcAlpha" )  return vk::BlendFactor::eSrcAlpha;
        else if( str == "OneMinusSrcAlpha" )  return vk::BlendFactor::eOneMinusSrcAlpha;
        else if( str == "DstAlpha" )  return vk::BlendFactor::eDstAlpha;
        else if( str == "OneMinusDstAlpha" )  return vk::BlendFactor::eOneMinusDstAlpha;
        else if( str == "ConstantColor" )  return vk::BlendFactor::eConstantColor;
        else if( str == "OneMinusConstantColor" )  return vk::BlendFactor::eOneMinusConstantColor;
        else if( str == "ConstantAlpha" )  return vk::BlendFactor::eConstantAlpha;
        else if( str == "OneMinusConstantAlpha" )  return vk::BlendFactor::eOneMinusConstantAlpha;
        else if( str == "SrcAlphaSaturate" )  return vk::BlendFactor::eSrcAlphaSaturate;
        else if( str == "Src1Color" )  return vk::BlendFactor::eSrc1Color;
        else if( str == "OneMinusSrc1Color" )  return vk::BlendFactor::eOneMinusSrc1Color;
        else if( str == "Src1Alpha" )  return vk::BlendFactor::eSrc1Alpha;
        else if( str == "OneMinusSrc1Alpha" )  return vk::BlendFactor::eOneMinusSrc1Alpha;
    };
    auto getBlendOp = [](std::string const & src)
    {
        if( src == "Add") return vk::BlendOp::eAdd;
        else if( src == "Subtract") return vk::BlendOp::eSubtract;
        else if( src == "ReverseSubtract") return vk::BlendOp::eReverseSubtract;
        else if( src == "Min") return vk::BlendOp::eMin;
        else if( src == "Max") return vk::BlendOp::eMax;
        else if( src == "ZeroEXT") return vk::BlendOp::eZeroEXT;
        else if( src == "SrcEXT") return vk::BlendOp::eSrcEXT;
        else if( src == "DstEXT") return vk::BlendOp::eDstEXT;
        else if( src == "SrcOverEXT") return vk::BlendOp::eSrcOverEXT;
        else if( src == "DstOverEXT") return vk::BlendOp::eDstOverEXT;
        else if( src == "SrcInEXT") return vk::BlendOp::eSrcInEXT;
        else if( src == "DstInEXT") return vk::BlendOp::eDstInEXT;
        else if( src == "SrcOutEXT") return vk::BlendOp::eSrcOutEXT;
        else if( src == "DstOutEXT") return vk::BlendOp::eDstOutEXT;
        else if( src == "SrcAtopEXT") return vk::BlendOp::eSrcAtopEXT;
        else if( src == "DstAtopEXT") return vk::BlendOp::eDstAtopEXT;
        else if( src == "XorEXT") return vk::BlendOp::eXorEXT;
        else if( src == "MultiplyEXT") return vk::BlendOp::eMultiplyEXT;
        else if( src == "ScreenEXT") return vk::BlendOp::eScreenEXT;
        else if( src == "OverlayEXT") return vk::BlendOp::eOverlayEXT;
        else if( src == "DarkenEXT") return vk::BlendOp::eDarkenEXT;
        else if( src == "LightenEXT") return vk::BlendOp::eLightenEXT;
        else if( src == "ColordodgeEXT") return vk::BlendOp::eColordodgeEXT;
        else if( src == "ColorburnEXT") return vk::BlendOp::eColorburnEXT;
        else if( src == "HardlightEXT") return vk::BlendOp::eHardlightEXT;
        else if( src == "SoftlightEXT") return vk::BlendOp::eSoftlightEXT;
        else if( src == "DifferenceEXT") return vk::BlendOp::eDifferenceEXT;
        else if( src == "ExclusionEXT") return vk::BlendOp::eExclusionEXT;
        else if( src == "InvertEXT") return vk::BlendOp::eInvertEXT;
        else if( src == "InvertRgbEXT") return vk::BlendOp::eInvertRgbEXT;
        else if( src == "LineardodgeEXT") return vk::BlendOp::eLineardodgeEXT;
        else if( src == "LinearburnEXT") return vk::BlendOp::eLinearburnEXT;
        else if( src == "VividlightEXT") return vk::BlendOp::eVividlightEXT;
        else if( src == "LinearlightEXT") return vk::BlendOp::eLinearlightEXT;
        else if( src == "PinlightEXT") return vk::BlendOp::ePinlightEXT;
        else if( src == "HardmixEXT") return vk::BlendOp::eHardmixEXT;
        else if( src == "HslHueEXT") return vk::BlendOp::eHslHueEXT;
        else if( src == "HslSaturationEXT") return vk::BlendOp::eHslSaturationEXT;
        else if( src == "HslColorEXT") return vk::BlendOp::eHslColorEXT;
        else if( src == "HslLuminosityEXT") return vk::BlendOp::eHslLuminosityEXT;
        else if( src == "PlusEXT") return vk::BlendOp::ePlusEXT;
        else if( src == "PlusClampedEXT") return vk::BlendOp::ePlusClampedEXT;
        else if( src == "PlusClampedAlphaEXT") return vk::BlendOp::ePlusClampedAlphaEXT;
        else if( src == "PlusDarkerEXT") return vk::BlendOp::ePlusDarkerEXT;
        else if( src == "MinusEXT") return vk::BlendOp::eMinusEXT;
        else if( src == "MinusClampedEXT") return vk::BlendOp::eMinusClampedEXT;
        else if( src == "ContrastEXT") return vk::BlendOp::eContrastEXT;
        else if( src == "InvertOvgEXT") return vk::BlendOp::eInvertOvgEXT;
        else if( src == "RedEXT") return vk::BlendOp::eRedEXT;
        else if( src == "GreenEXT") return vk::BlendOp::eGreenEXT;
        else if( src == "BlueEXT") return vk::BlendOp::eBlueEXT;
    };

    p.srcAlphaBlendFactor  =  getBlendFactor(j.at("srcAlphaBlendFactor").get<std::string>() );
    p.dstAlphaBlendFactor  =  getBlendFactor(j.at("dstAlphaBlendFactor").get<std::string>() );
    p.srcColorBlendFactor  =  getBlendFactor(j.at("srcColorBlendFactor").get<std::string>() );
    p.dstColorBlendFactor  =  getBlendFactor(j.at("dstColorBlendFactor").get<std::string>() );
    j.at("blendEnable").get_to(p.blendEnable);

    p.alphaBlendOp = getBlendOp(j.at("alphaBlendOp").get<std::string>() );
    p.colorBlendOp = getBlendOp(j.at("colorBlendOp").get<std::string>() );
}

inline void from_json(const nlohmann::json & j, vk::PipelineDepthStencilStateCreateInfo& p)
{

    j.at("depthBoundsTestEnable").get_to(p.depthBoundsTestEnable);
    j.at("maxDepthBounds").get_to(p.maxDepthBounds);
    j.at("minDepthBounds").get_to(p.minDepthBounds);
    j.at("depthTestEnable").get_to(p.depthTestEnable);

    j.at("depthWriteEnable").get_to(p.depthWriteEnable);
    j.at("stencilTestEnable").get_to(p.stencilTestEnable);

    auto depthCompareOp = j.at("depthCompareOp").get<std::string>();

    #define CH(VV) if( depthCompareOp == #VV ) p.depthCompareOp = vk::CompareOp::e ## VV
        CH(Never);
        CH(Less);
        CH(Equal);
        CH(LessOrEqual);
        CH(Greater);
        CH(NotEqual);
        CH(GreaterOrEqual);
        CH(Always);
    #undef CH

}

inline void from_json(const nlohmann::json & j, vk::PipelineMultisampleStateCreateInfo& p)
{
    j.at("alphaToCoverageEnable").get_to(p.alphaToCoverageEnable);
    j.at("minSampleShading").get_to(p.minSampleShading);
    j.at("sampleShadingEnable").get_to(p.sampleShadingEnable);
    j.at("alphaToCoverageEnable").get_to(p.alphaToCoverageEnable);

    auto ss = j.at("rasterizationSamples").get<std::string>();
    if( ss== "1")
        p.rasterizationSamples = vk::SampleCountFlagBits::e1;
    if( ss== "2")
        p.rasterizationSamples = vk::SampleCountFlagBits::e2;
    if( ss== "4")
        p.rasterizationSamples = vk::SampleCountFlagBits::e4;
    if( ss== "8")
        p.rasterizationSamples = vk::SampleCountFlagBits::e8;
    if( ss== "16")
        p.rasterizationSamples = vk::SampleCountFlagBits::e16;
    if( ss== "32")
        p.rasterizationSamples = vk::SampleCountFlagBits::e32;
}

inline void from_json(const nlohmann::json & j, vk::PipelineRasterizationStateCreateInfo& p)
{
    std::string cullMode = j.at("cullMode").get<std::string>();
    std::string frontFace = j.at("frontFace").get<std::string>();
    std::string polygonMode = j.at("polygonMode").get<std::string>();

    #define CH(VV) if( cullMode == #VV ) p.cullMode = vk::CullModeFlagBits::e ## VV
        CH(None);
        CH(Front);
        CH(Back);
        CH(FrontAndBack);
    #undef CH


    #define CH(VV) if( cullMode == #VV ) p.frontFace = vk::FrontFace::e ## VV
        CH(CounterClockwise);
        CH(Clockwise);
    #undef CH

    #define CH(VV) if( polygonMode == #VV ) p.polygonMode = vk::PolygonMode::e ## VV
        CH(Fill);
        CH(Line);
        CH(Fill);
        CH(Point);
    #undef CH

    j.at("cullMode").get_to(p.depthBiasClamp);
    j.at("depthClampEnable").get_to(p.depthClampEnable);
    j.at("depthBiasEnable").get_to(p.depthBiasEnable);
    j.at("rasterizerDiscardEnable").get_to(p.rasterizerDiscardEnable);

}

inline void from_json(const nlohmann::json & j, vk::VertexInputRate& p)
{
    if( !j.is_string())
        std::runtime_error("VertexInput rate must be a string");

    if( j == "Vertex")
    {
        p = vk::VertexInputRate::eVertex;
        return;
    }
    else if( j == "Instance")
    {
        p = vk::VertexInputRate::eInstance;
        return;
    }

    std::runtime_error("Invalid VertexInputRate");
}

inline void from_json(const nlohmann::json & j, vk::VertexInputBindingDescription& p)
{
    p.stride=0;
    if( j.count("stride"))
        j.at("stride").get_to(p.stride);


    j.at("binding").get_to(p.binding);

    p.inputRate  = j.at("inputRate").get<vk::VertexInputRate>();
}

inline void from_json(const nlohmann::json & j, vk::VertexInputAttributeDescription& p)
{
    j.at("location").get_to(p.location);
    j.at("binding").get_to(p.binding);
    j.at("offset").get_to(p.offset);

    auto fstr = j.at("format").get<std::string>();

    #define CHi(VV) if( fstr == #VV ) p.format = vk::Format::e ## VV;
    #define CH(VV) else if( fstr == #VV ) p.format = vk::Format::e ## VV;
    CHi(R4G4UnormPack8)
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
}

}
#endif


