#ifndef VKJSON_FROM_JSON_ENUMS_H
#define VKJSON_FROM_JSON_ENUMS_H

#include <nlohmann/json.hpp>
#include <vulkan/vulkan.hpp>
#include <map>

namespace vk
{

  #define VKB_BEGIN_ENUM_MACRO \
    p = {};\
    if( !j.is_string() ) throw std::runtime_error("Enums must be JSON Strings");\
    auto str = j.get<std::string>(); \
    static auto _M = std::map< std::string, enum_c_type> {

  #define VKB_ENUM_MACRO(A) { #A, enum_c_type::e ## A},
  #define VKB_END_ENUM_MACRO \
        };\
        p = _M.at(str);

  inline void from_json(nlohmann::json const & j, AccelerationStructureMemoryRequirementsTypeNV &p)
  {
      using enum_c_type = AccelerationStructureMemoryRequirementsTypeNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Object)
              VKB_ENUM_MACRO(BuildScratch)
              VKB_ENUM_MACRO(UpdateScratch)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, AccelerationStructureTypeNV &p)
  {
      using enum_c_type = AccelerationStructureTypeNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(TopLevel)
              VKB_ENUM_MACRO(BottomLevel)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, AccessFlagBits &p)
  {
      using enum_c_type = AccessFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(IndirectCommandRead)
              VKB_ENUM_MACRO(IndexRead)
              VKB_ENUM_MACRO(VertexAttributeRead)
              VKB_ENUM_MACRO(UniformRead)
              VKB_ENUM_MACRO(InputAttachmentRead)
              VKB_ENUM_MACRO(ShaderRead)
              VKB_ENUM_MACRO(ShaderWrite)
              VKB_ENUM_MACRO(ColorAttachmentRead)
              VKB_ENUM_MACRO(ColorAttachmentWrite)
              VKB_ENUM_MACRO(DepthStencilAttachmentRead)
              VKB_ENUM_MACRO(DepthStencilAttachmentWrite)
              VKB_ENUM_MACRO(TransferRead)
              VKB_ENUM_MACRO(TransferWrite)
              VKB_ENUM_MACRO(HostRead)
              VKB_ENUM_MACRO(HostWrite)
              VKB_ENUM_MACRO(MemoryRead)
              VKB_ENUM_MACRO(MemoryWrite)
              VKB_ENUM_MACRO(TransformFeedbackWriteEXT)
              VKB_ENUM_MACRO(TransformFeedbackCounterReadEXT)
              VKB_ENUM_MACRO(TransformFeedbackCounterWriteEXT)
              VKB_ENUM_MACRO(ConditionalRenderingReadEXT)
              VKB_ENUM_MACRO(CommandProcessReadNVX)
              VKB_ENUM_MACRO(CommandProcessWriteNVX)
              VKB_ENUM_MACRO(ColorAttachmentReadNoncoherentEXT)
              VKB_ENUM_MACRO(ShadingRateImageReadNV)
              VKB_ENUM_MACRO(AccelerationStructureReadNV)
              VKB_ENUM_MACRO(AccelerationStructureWriteNV)
              VKB_ENUM_MACRO(FragmentDensityMapReadEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, AcquireProfilingLockFlagBitsKHR &p)
  {
      using enum_c_type = AcquireProfilingLockFlagBitsKHR;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, AttachmentDescriptionFlagBits &p)
  {
      using enum_c_type = AttachmentDescriptionFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(MayAlias)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, AttachmentLoadOp &p)
  {
      using enum_c_type = AttachmentLoadOp;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Load)
              VKB_ENUM_MACRO(Clear)
              VKB_ENUM_MACRO(DontCare)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, AttachmentStoreOp &p)
  {
      using enum_c_type = AttachmentStoreOp;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Store)
              VKB_ENUM_MACRO(DontCare)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, BlendFactor &p)
  {
      using enum_c_type = BlendFactor;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Zero)
              VKB_ENUM_MACRO(One)
              VKB_ENUM_MACRO(SrcColor)
              VKB_ENUM_MACRO(OneMinusSrcColor)
              VKB_ENUM_MACRO(DstColor)
              VKB_ENUM_MACRO(OneMinusDstColor)
              VKB_ENUM_MACRO(SrcAlpha)
              VKB_ENUM_MACRO(OneMinusSrcAlpha)
              VKB_ENUM_MACRO(DstAlpha)
              VKB_ENUM_MACRO(OneMinusDstAlpha)
              VKB_ENUM_MACRO(ConstantColor)
              VKB_ENUM_MACRO(OneMinusConstantColor)
              VKB_ENUM_MACRO(ConstantAlpha)
              VKB_ENUM_MACRO(OneMinusConstantAlpha)
              VKB_ENUM_MACRO(SrcAlphaSaturate)
              VKB_ENUM_MACRO(Src1Color)
              VKB_ENUM_MACRO(OneMinusSrc1Color)
              VKB_ENUM_MACRO(Src1Alpha)
              VKB_ENUM_MACRO(OneMinusSrc1Alpha)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, BlendOp &p)
  {
      using enum_c_type = BlendOp;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Add)
              VKB_ENUM_MACRO(Subtract)
              VKB_ENUM_MACRO(ReverseSubtract)
              VKB_ENUM_MACRO(Min)
              VKB_ENUM_MACRO(Max)
              VKB_ENUM_MACRO(ZeroEXT)
              VKB_ENUM_MACRO(SrcEXT)
              VKB_ENUM_MACRO(DstEXT)
              VKB_ENUM_MACRO(SrcOverEXT)
              VKB_ENUM_MACRO(DstOverEXT)
              VKB_ENUM_MACRO(SrcInEXT)
              VKB_ENUM_MACRO(DstInEXT)
              VKB_ENUM_MACRO(SrcOutEXT)
              VKB_ENUM_MACRO(DstOutEXT)
              VKB_ENUM_MACRO(SrcAtopEXT)
              VKB_ENUM_MACRO(DstAtopEXT)
              VKB_ENUM_MACRO(XorEXT)
              VKB_ENUM_MACRO(MultiplyEXT)
              VKB_ENUM_MACRO(ScreenEXT)
              VKB_ENUM_MACRO(OverlayEXT)
              VKB_ENUM_MACRO(DarkenEXT)
              VKB_ENUM_MACRO(LightenEXT)
              VKB_ENUM_MACRO(ColordodgeEXT)
              VKB_ENUM_MACRO(ColorburnEXT)
              VKB_ENUM_MACRO(HardlightEXT)
              VKB_ENUM_MACRO(SoftlightEXT)
              VKB_ENUM_MACRO(DifferenceEXT)
              VKB_ENUM_MACRO(ExclusionEXT)
              VKB_ENUM_MACRO(InvertEXT)
              VKB_ENUM_MACRO(InvertRgbEXT)
              VKB_ENUM_MACRO(LineardodgeEXT)
              VKB_ENUM_MACRO(LinearburnEXT)
              VKB_ENUM_MACRO(VividlightEXT)
              VKB_ENUM_MACRO(LinearlightEXT)
              VKB_ENUM_MACRO(PinlightEXT)
              VKB_ENUM_MACRO(HardmixEXT)
              VKB_ENUM_MACRO(HslHueEXT)
              VKB_ENUM_MACRO(HslSaturationEXT)
              VKB_ENUM_MACRO(HslColorEXT)
              VKB_ENUM_MACRO(HslLuminosityEXT)
              VKB_ENUM_MACRO(PlusEXT)
              VKB_ENUM_MACRO(PlusClampedEXT)
              VKB_ENUM_MACRO(PlusClampedAlphaEXT)
              VKB_ENUM_MACRO(PlusDarkerEXT)
              VKB_ENUM_MACRO(MinusEXT)
              VKB_ENUM_MACRO(MinusClampedEXT)
              VKB_ENUM_MACRO(ContrastEXT)
              VKB_ENUM_MACRO(InvertOvgEXT)
              VKB_ENUM_MACRO(RedEXT)
              VKB_ENUM_MACRO(GreenEXT)
              VKB_ENUM_MACRO(BlueEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, BlendOverlapEXT &p)
  {
      using enum_c_type = BlendOverlapEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Uncorrelated)
              VKB_ENUM_MACRO(Disjoint)
              VKB_ENUM_MACRO(Conjoint)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, BorderColor &p)
  {
      using enum_c_type = BorderColor;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(FloatTransparentBlack)
              VKB_ENUM_MACRO(IntTransparentBlack)
              VKB_ENUM_MACRO(FloatOpaqueBlack)
              VKB_ENUM_MACRO(IntOpaqueBlack)
              VKB_ENUM_MACRO(FloatOpaqueWhite)
              VKB_ENUM_MACRO(IntOpaqueWhite)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, BufferCreateFlagBits &p)
  {
      using enum_c_type = BufferCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(SparseBinding)
              VKB_ENUM_MACRO(SparseResidency)
              VKB_ENUM_MACRO(SparseAliased)
              VKB_ENUM_MACRO(Protected)
              VKB_ENUM_MACRO(DeviceAddressCaptureReplay)
              VKB_ENUM_MACRO(DeviceAddressCaptureReplayEXT)
              VKB_ENUM_MACRO(DeviceAddressCaptureReplayKHR)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, BufferUsageFlagBits &p)
  {
      using enum_c_type = BufferUsageFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(TransferSrc)
              VKB_ENUM_MACRO(TransferDst)
              VKB_ENUM_MACRO(UniformTexelBuffer)
              VKB_ENUM_MACRO(StorageTexelBuffer)
              VKB_ENUM_MACRO(UniformBuffer)
              VKB_ENUM_MACRO(StorageBuffer)
              VKB_ENUM_MACRO(IndexBuffer)
              VKB_ENUM_MACRO(VertexBuffer)
              VKB_ENUM_MACRO(IndirectBuffer)
              VKB_ENUM_MACRO(ShaderDeviceAddress)
              VKB_ENUM_MACRO(TransformFeedbackBufferEXT)
              VKB_ENUM_MACRO(TransformFeedbackCounterBufferEXT)
              VKB_ENUM_MACRO(ConditionalRenderingEXT)
              VKB_ENUM_MACRO(RayTracingNV)
              VKB_ENUM_MACRO(ShaderDeviceAddressEXT)
              VKB_ENUM_MACRO(ShaderDeviceAddressKHR)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, BufferViewCreateFlagBits &p)
  {
      using enum_c_type = BufferViewCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, BuildAccelerationStructureFlagBitsNV &p)
  {
      using enum_c_type = BuildAccelerationStructureFlagBitsNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(AllowUpdate)
              VKB_ENUM_MACRO(AllowCompaction)
              VKB_ENUM_MACRO(PreferFastTrace)
              VKB_ENUM_MACRO(PreferFastBuild)
              VKB_ENUM_MACRO(LowMemory)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ChromaLocation &p)
  {
      using enum_c_type = ChromaLocation;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(CositedEven)
              VKB_ENUM_MACRO(Midpoint)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, CoarseSampleOrderTypeNV &p)
  {
      using enum_c_type = CoarseSampleOrderTypeNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Default)
              VKB_ENUM_MACRO(Custom)
              VKB_ENUM_MACRO(PixelMajor)
              VKB_ENUM_MACRO(SampleMajor)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ColorComponentFlagBits &p)
  {
      using enum_c_type = ColorComponentFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(R)
              VKB_ENUM_MACRO(G)
              VKB_ENUM_MACRO(B)
              VKB_ENUM_MACRO(A)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ColorSpaceKHR &p)
  {
      using enum_c_type = ColorSpaceKHR;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(SrgbNonlinear)
              VKB_ENUM_MACRO(DisplayP3NonlinearEXT)
              VKB_ENUM_MACRO(ExtendedSrgbLinearEXT)
              VKB_ENUM_MACRO(DisplayP3LinearEXT)
              VKB_ENUM_MACRO(DciP3NonlinearEXT)
              VKB_ENUM_MACRO(Bt709LinearEXT)
              VKB_ENUM_MACRO(Bt709NonlinearEXT)
              VKB_ENUM_MACRO(Bt2020LinearEXT)
              VKB_ENUM_MACRO(Hdr10St2084EXT)
              VKB_ENUM_MACRO(DolbyvisionEXT)
              VKB_ENUM_MACRO(Hdr10HlgEXT)
              VKB_ENUM_MACRO(AdobergbLinearEXT)
              VKB_ENUM_MACRO(AdobergbNonlinearEXT)
              VKB_ENUM_MACRO(PassThroughEXT)
              VKB_ENUM_MACRO(ExtendedSrgbNonlinearEXT)
              VKB_ENUM_MACRO(DisplayNativeAMD)
              VKB_ENUM_MACRO(VkColorspaceSrgbNonlinear)
              VKB_ENUM_MACRO(DciP3LinearEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, CommandBufferLevel &p)
  {
      using enum_c_type = CommandBufferLevel;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Primary)
              VKB_ENUM_MACRO(Secondary)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, CommandBufferResetFlagBits &p)
  {
      using enum_c_type = CommandBufferResetFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(ReleaseResources)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, CommandBufferUsageFlagBits &p)
  {
      using enum_c_type = CommandBufferUsageFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(OneTimeSubmit)
              VKB_ENUM_MACRO(RenderPassContinue)
              VKB_ENUM_MACRO(SimultaneousUse)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, CommandPoolCreateFlagBits &p)
  {
      using enum_c_type = CommandPoolCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Transient)
              VKB_ENUM_MACRO(ResetCommandBuffer)
              VKB_ENUM_MACRO(Protected)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, CommandPoolResetFlagBits &p)
  {
      using enum_c_type = CommandPoolResetFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(ReleaseResources)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, CompareOp &p)
  {
      using enum_c_type = CompareOp;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Never)
              VKB_ENUM_MACRO(Less)
              VKB_ENUM_MACRO(Equal)
              VKB_ENUM_MACRO(LessOrEqual)
              VKB_ENUM_MACRO(Greater)
              VKB_ENUM_MACRO(NotEqual)
              VKB_ENUM_MACRO(GreaterOrEqual)
              VKB_ENUM_MACRO(Always)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ComponentSwizzle &p)
  {
      using enum_c_type = ComponentSwizzle;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Identity)
              VKB_ENUM_MACRO(Zero)
              VKB_ENUM_MACRO(One)
              VKB_ENUM_MACRO(R)
              VKB_ENUM_MACRO(G)
              VKB_ENUM_MACRO(B)
              VKB_ENUM_MACRO(A)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ComponentTypeNV &p)
  {
      using enum_c_type = ComponentTypeNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Float16)
              VKB_ENUM_MACRO(Float32)
              VKB_ENUM_MACRO(Float64)
              VKB_ENUM_MACRO(Sint8)
              VKB_ENUM_MACRO(Sint16)
              VKB_ENUM_MACRO(Sint32)
              VKB_ENUM_MACRO(Sint64)
              VKB_ENUM_MACRO(Uint8)
              VKB_ENUM_MACRO(Uint16)
              VKB_ENUM_MACRO(Uint32)
              VKB_ENUM_MACRO(Uint64)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, CompositeAlphaFlagBitsKHR &p)
  {
      using enum_c_type = CompositeAlphaFlagBitsKHR;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Opaque)
              VKB_ENUM_MACRO(PreMultiplied)
              VKB_ENUM_MACRO(PostMultiplied)
              VKB_ENUM_MACRO(Inherit)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ConditionalRenderingFlagBitsEXT &p)
  {
      using enum_c_type = ConditionalRenderingFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Inverted)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ConservativeRasterizationModeEXT &p)
  {
      using enum_c_type = ConservativeRasterizationModeEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Disabled)
              VKB_ENUM_MACRO(Overestimate)
              VKB_ENUM_MACRO(Underestimate)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, CopyAccelerationStructureModeNV &p)
  {
      using enum_c_type = CopyAccelerationStructureModeNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Clone)
              VKB_ENUM_MACRO(Compact)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, CoverageModulationModeNV &p)
  {
      using enum_c_type = CoverageModulationModeNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(None)
              VKB_ENUM_MACRO(Rgb)
              VKB_ENUM_MACRO(Alpha)
              VKB_ENUM_MACRO(Rgba)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, CoverageReductionModeNV &p)
  {
      using enum_c_type = CoverageReductionModeNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Merge)
              VKB_ENUM_MACRO(Truncate)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, CullModeFlagBits &p)
  {
      using enum_c_type = CullModeFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(None)
              VKB_ENUM_MACRO(Front)
              VKB_ENUM_MACRO(Back)
              VKB_ENUM_MACRO(FrontAndBack)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DebugReportFlagBitsEXT &p)
  {
      using enum_c_type = DebugReportFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Information)
              VKB_ENUM_MACRO(Warning)
              VKB_ENUM_MACRO(PerformanceWarning)
              VKB_ENUM_MACRO(Error)
              VKB_ENUM_MACRO(Debug)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DebugReportObjectTypeEXT &p)
  {
      using enum_c_type = DebugReportObjectTypeEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Unknown)
              VKB_ENUM_MACRO(Instance)
              VKB_ENUM_MACRO(PhysicalDevice)
              VKB_ENUM_MACRO(Device)
              VKB_ENUM_MACRO(Queue)
              VKB_ENUM_MACRO(Semaphore)
              VKB_ENUM_MACRO(CommandBuffer)
              VKB_ENUM_MACRO(Fence)
              VKB_ENUM_MACRO(DeviceMemory)
              VKB_ENUM_MACRO(Buffer)
              VKB_ENUM_MACRO(Image)
              VKB_ENUM_MACRO(Event)
              VKB_ENUM_MACRO(QueryPool)
              VKB_ENUM_MACRO(BufferView)
              VKB_ENUM_MACRO(ImageView)
              VKB_ENUM_MACRO(ShaderModule)
              VKB_ENUM_MACRO(PipelineCache)
              VKB_ENUM_MACRO(PipelineLayout)
              VKB_ENUM_MACRO(RenderPass)
              VKB_ENUM_MACRO(Pipeline)
              VKB_ENUM_MACRO(DescriptorSetLayout)
              VKB_ENUM_MACRO(Sampler)
              VKB_ENUM_MACRO(DescriptorPool)
              VKB_ENUM_MACRO(DescriptorSet)
              VKB_ENUM_MACRO(Framebuffer)
              VKB_ENUM_MACRO(CommandPool)
              VKB_ENUM_MACRO(SurfaceKHR)
              VKB_ENUM_MACRO(SwapchainKHR)
              VKB_ENUM_MACRO(DebugReportCallbackEXT)
              VKB_ENUM_MACRO(DisplayKHR)
              VKB_ENUM_MACRO(DisplayModeKHR)
              VKB_ENUM_MACRO(ObjectTableNVX)
              VKB_ENUM_MACRO(IndirectCommandsLayoutNVX)
              VKB_ENUM_MACRO(ValidationCacheEXT)
              VKB_ENUM_MACRO(SamplerYcbcrConversion)
              VKB_ENUM_MACRO(DescriptorUpdateTemplate)
              VKB_ENUM_MACRO(AccelerationStructureNV)
              VKB_ENUM_MACRO(DebugReport)
              VKB_ENUM_MACRO(ValidationCache)
              VKB_ENUM_MACRO(DescriptorUpdateTemplateKHR)
              VKB_ENUM_MACRO(SamplerYcbcrConversionKHR)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DebugUtilsMessageSeverityFlagBitsEXT &p)
  {
      using enum_c_type = DebugUtilsMessageSeverityFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Verbose)
              VKB_ENUM_MACRO(Info)
              VKB_ENUM_MACRO(Warning)
              VKB_ENUM_MACRO(Error)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DebugUtilsMessageTypeFlagBitsEXT &p)
  {
      using enum_c_type = DebugUtilsMessageTypeFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(General)
              VKB_ENUM_MACRO(Validation)
              VKB_ENUM_MACRO(Performance)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DependencyFlagBits &p)
  {
      using enum_c_type = DependencyFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(ByRegion)
              VKB_ENUM_MACRO(DeviceGroup)
              VKB_ENUM_MACRO(ViewLocal)
              VKB_ENUM_MACRO(ViewLocalKHR)
              VKB_ENUM_MACRO(DeviceGroupKHR)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DescriptorBindingFlagBits &p)
  {
      using enum_c_type = DescriptorBindingFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(UpdateAfterBind)
              VKB_ENUM_MACRO(UpdateUnusedWhilePending)
              VKB_ENUM_MACRO(PartiallyBound)
              VKB_ENUM_MACRO(VariableDescriptorCount)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DescriptorPoolCreateFlagBits &p)
  {
      using enum_c_type = DescriptorPoolCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(FreeDescriptorSet)
              VKB_ENUM_MACRO(UpdateAfterBind)
              VKB_ENUM_MACRO(UpdateAfterBindEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DescriptorSetLayoutCreateFlagBits &p)
  {
      using enum_c_type = DescriptorSetLayoutCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(UpdateAfterBindPool)
              VKB_ENUM_MACRO(PushDescriptorKHR)
              VKB_ENUM_MACRO(UpdateAfterBindPoolEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DescriptorType &p)
  {
      using enum_c_type = DescriptorType;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Sampler)
              VKB_ENUM_MACRO(CombinedImageSampler)
              VKB_ENUM_MACRO(SampledImage)
              VKB_ENUM_MACRO(StorageImage)
              VKB_ENUM_MACRO(UniformTexelBuffer)
              VKB_ENUM_MACRO(StorageTexelBuffer)
              VKB_ENUM_MACRO(UniformBuffer)
              VKB_ENUM_MACRO(StorageBuffer)
              VKB_ENUM_MACRO(UniformBufferDynamic)
              VKB_ENUM_MACRO(StorageBufferDynamic)
              VKB_ENUM_MACRO(InputAttachment)
              VKB_ENUM_MACRO(InlineUniformBlockEXT)
              VKB_ENUM_MACRO(AccelerationStructureNV)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DescriptorUpdateTemplateType &p)
  {
      using enum_c_type = DescriptorUpdateTemplateType;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(DescriptorSet)
              VKB_ENUM_MACRO(PushDescriptorsKHR)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DeviceCreateFlagBits &p)
  {
      using enum_c_type = DeviceCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DeviceEventTypeEXT &p)
  {
      using enum_c_type = DeviceEventTypeEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(DisplayHotplug)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DeviceGroupPresentModeFlagBitsKHR &p)
  {
      using enum_c_type = DeviceGroupPresentModeFlagBitsKHR;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Local)
              VKB_ENUM_MACRO(Remote)
              VKB_ENUM_MACRO(Sum)
              VKB_ENUM_MACRO(LocalMultiDevice)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DeviceQueueCreateFlagBits &p)
  {
      using enum_c_type = DeviceQueueCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Protected)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DiscardRectangleModeEXT &p)
  {
      using enum_c_type = DiscardRectangleModeEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Inclusive)
              VKB_ENUM_MACRO(Exclusive)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DisplayEventTypeEXT &p)
  {
      using enum_c_type = DisplayEventTypeEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(FirstPixelOut)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DisplayPlaneAlphaFlagBitsKHR &p)
  {
      using enum_c_type = DisplayPlaneAlphaFlagBitsKHR;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Opaque)
              VKB_ENUM_MACRO(Global)
              VKB_ENUM_MACRO(PerPixel)
              VKB_ENUM_MACRO(PerPixelPremultiplied)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DisplayPowerStateEXT &p)
  {
      using enum_c_type = DisplayPowerStateEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Off)
              VKB_ENUM_MACRO(Suspend)
              VKB_ENUM_MACRO(On)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DriverId &p)
  {
      using enum_c_type = DriverId;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(AmdProprietary)
              VKB_ENUM_MACRO(AmdOpenSource)
              VKB_ENUM_MACRO(MesaRadv)
              VKB_ENUM_MACRO(NvidiaProprietary)
              VKB_ENUM_MACRO(IntelProprietaryWindows)
              VKB_ENUM_MACRO(IntelOpenSourceMESA)
              VKB_ENUM_MACRO(ImaginationProprietary)
              VKB_ENUM_MACRO(QualcommProprietary)
              VKB_ENUM_MACRO(ArmProprietary)
              VKB_ENUM_MACRO(GoogleSwiftshader)
              VKB_ENUM_MACRO(GgpProprietary)
              VKB_ENUM_MACRO(BroadcomProprietary)
              VKB_ENUM_MACRO(IntelOpenSourceMesa)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DynamicState &p)
  {
      using enum_c_type = DynamicState;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Viewport)
              VKB_ENUM_MACRO(Scissor)
              VKB_ENUM_MACRO(LineWidth)
              VKB_ENUM_MACRO(DepthBias)
              VKB_ENUM_MACRO(BlendConstants)
              VKB_ENUM_MACRO(DepthBounds)
              VKB_ENUM_MACRO(StencilCompareMask)
              VKB_ENUM_MACRO(StencilWriteMask)
              VKB_ENUM_MACRO(StencilReference)
              VKB_ENUM_MACRO(ViewportWScalingNV)
              VKB_ENUM_MACRO(DiscardRectangleEXT)
              VKB_ENUM_MACRO(SampleLocationsEXT)
              VKB_ENUM_MACRO(ViewportShadingRatePaletteNV)
              VKB_ENUM_MACRO(ViewportCoarseSampleOrderNV)
              VKB_ENUM_MACRO(ExclusiveScissorNV)
              VKB_ENUM_MACRO(LineStippleEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ExternalFenceFeatureFlagBits &p)
  {
      using enum_c_type = ExternalFenceFeatureFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Exportable)
              VKB_ENUM_MACRO(Importable)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ExternalFenceHandleTypeFlagBits &p)
  {
      using enum_c_type = ExternalFenceHandleTypeFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(OpaqueFd)
              VKB_ENUM_MACRO(OpaqueWin32)
              VKB_ENUM_MACRO(OpaqueWin32Kmt)
              VKB_ENUM_MACRO(SyncFd)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ExternalMemoryFeatureFlagBits &p)
  {
      using enum_c_type = ExternalMemoryFeatureFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(DedicatedOnly)
              VKB_ENUM_MACRO(Exportable)
              VKB_ENUM_MACRO(Importable)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ExternalMemoryFeatureFlagBitsNV &p)
  {
      using enum_c_type = ExternalMemoryFeatureFlagBitsNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(DedicatedOnly)
              VKB_ENUM_MACRO(Exportable)
              VKB_ENUM_MACRO(Importable)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ExternalMemoryHandleTypeFlagBits &p)
  {
      using enum_c_type = ExternalMemoryHandleTypeFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(OpaqueFd)
              VKB_ENUM_MACRO(OpaqueWin32)
              VKB_ENUM_MACRO(OpaqueWin32Kmt)
              VKB_ENUM_MACRO(D3D11Texture)
              VKB_ENUM_MACRO(D3D11TextureKmt)
              VKB_ENUM_MACRO(D3D12Heap)
              VKB_ENUM_MACRO(D3D12Resource)
              VKB_ENUM_MACRO(DmaBufEXT)
              VKB_ENUM_MACRO(AndroidHardwareBufferANDROID)
              VKB_ENUM_MACRO(HostAllocationEXT)
              VKB_ENUM_MACRO(HostMappedForeignMemoryEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ExternalMemoryHandleTypeFlagBitsNV &p)
  {
      using enum_c_type = ExternalMemoryHandleTypeFlagBitsNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(OpaqueWin32)
              VKB_ENUM_MACRO(OpaqueWin32Kmt)
              VKB_ENUM_MACRO(D3D11Image)
              VKB_ENUM_MACRO(D3D11ImageKmt)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ExternalSemaphoreFeatureFlagBits &p)
  {
      using enum_c_type = ExternalSemaphoreFeatureFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Exportable)
              VKB_ENUM_MACRO(Importable)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ExternalSemaphoreHandleTypeFlagBits &p)
  {
      using enum_c_type = ExternalSemaphoreHandleTypeFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(OpaqueFd)
              VKB_ENUM_MACRO(OpaqueWin32)
              VKB_ENUM_MACRO(OpaqueWin32Kmt)
              VKB_ENUM_MACRO(D3D12Fence)
              VKB_ENUM_MACRO(SyncFd)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, FenceCreateFlagBits &p)
  {
      using enum_c_type = FenceCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Signaled)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, FenceImportFlagBits &p)
  {
      using enum_c_type = FenceImportFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Temporary)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, Filter &p)
  {
      using enum_c_type = Filter;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Nearest)
              VKB_ENUM_MACRO(Linear)
              VKB_ENUM_MACRO(CubicIMG)
              VKB_ENUM_MACRO(CubicEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, Format &p)
  {
      using enum_c_type = Format;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Undefined)
              VKB_ENUM_MACRO(R4G4UnormPack8)
              VKB_ENUM_MACRO(R4G4B4A4UnormPack16)
              VKB_ENUM_MACRO(B4G4R4A4UnormPack16)
              VKB_ENUM_MACRO(R5G6B5UnormPack16)
              VKB_ENUM_MACRO(B5G6R5UnormPack16)
              VKB_ENUM_MACRO(R5G5B5A1UnormPack16)
              VKB_ENUM_MACRO(B5G5R5A1UnormPack16)
              VKB_ENUM_MACRO(A1R5G5B5UnormPack16)
              VKB_ENUM_MACRO(R8Unorm)
              VKB_ENUM_MACRO(R8Snorm)
              VKB_ENUM_MACRO(R8Uscaled)
              VKB_ENUM_MACRO(R8Sscaled)
              VKB_ENUM_MACRO(R8Uint)
              VKB_ENUM_MACRO(R8Sint)
              VKB_ENUM_MACRO(R8Srgb)
              VKB_ENUM_MACRO(R8G8Unorm)
              VKB_ENUM_MACRO(R8G8Snorm)
              VKB_ENUM_MACRO(R8G8Uscaled)
              VKB_ENUM_MACRO(R8G8Sscaled)
              VKB_ENUM_MACRO(R8G8Uint)
              VKB_ENUM_MACRO(R8G8Sint)
              VKB_ENUM_MACRO(R8G8Srgb)
              VKB_ENUM_MACRO(R8G8B8Unorm)
              VKB_ENUM_MACRO(R8G8B8Snorm)
              VKB_ENUM_MACRO(R8G8B8Uscaled)
              VKB_ENUM_MACRO(R8G8B8Sscaled)
              VKB_ENUM_MACRO(R8G8B8Uint)
              VKB_ENUM_MACRO(R8G8B8Sint)
              VKB_ENUM_MACRO(R8G8B8Srgb)
              VKB_ENUM_MACRO(B8G8R8Unorm)
              VKB_ENUM_MACRO(B8G8R8Snorm)
              VKB_ENUM_MACRO(B8G8R8Uscaled)
              VKB_ENUM_MACRO(B8G8R8Sscaled)
              VKB_ENUM_MACRO(B8G8R8Uint)
              VKB_ENUM_MACRO(B8G8R8Sint)
              VKB_ENUM_MACRO(B8G8R8Srgb)
              VKB_ENUM_MACRO(R8G8B8A8Unorm)
              VKB_ENUM_MACRO(R8G8B8A8Snorm)
              VKB_ENUM_MACRO(R8G8B8A8Uscaled)
              VKB_ENUM_MACRO(R8G8B8A8Sscaled)
              VKB_ENUM_MACRO(R8G8B8A8Uint)
              VKB_ENUM_MACRO(R8G8B8A8Sint)
              VKB_ENUM_MACRO(R8G8B8A8Srgb)
              VKB_ENUM_MACRO(B8G8R8A8Unorm)
              VKB_ENUM_MACRO(B8G8R8A8Snorm)
              VKB_ENUM_MACRO(B8G8R8A8Uscaled)
              VKB_ENUM_MACRO(B8G8R8A8Sscaled)
              VKB_ENUM_MACRO(B8G8R8A8Uint)
              VKB_ENUM_MACRO(B8G8R8A8Sint)
              VKB_ENUM_MACRO(B8G8R8A8Srgb)
              VKB_ENUM_MACRO(A8B8G8R8UnormPack32)
              VKB_ENUM_MACRO(A8B8G8R8SnormPack32)
              VKB_ENUM_MACRO(A8B8G8R8UscaledPack32)
              VKB_ENUM_MACRO(A8B8G8R8SscaledPack32)
              VKB_ENUM_MACRO(A8B8G8R8UintPack32)
              VKB_ENUM_MACRO(A8B8G8R8SintPack32)
              VKB_ENUM_MACRO(A8B8G8R8SrgbPack32)
              VKB_ENUM_MACRO(A2R10G10B10UnormPack32)
              VKB_ENUM_MACRO(A2R10G10B10SnormPack32)
              VKB_ENUM_MACRO(A2R10G10B10UscaledPack32)
              VKB_ENUM_MACRO(A2R10G10B10SscaledPack32)
              VKB_ENUM_MACRO(A2R10G10B10UintPack32)
              VKB_ENUM_MACRO(A2R10G10B10SintPack32)
              VKB_ENUM_MACRO(A2B10G10R10UnormPack32)
              VKB_ENUM_MACRO(A2B10G10R10SnormPack32)
              VKB_ENUM_MACRO(A2B10G10R10UscaledPack32)
              VKB_ENUM_MACRO(A2B10G10R10SscaledPack32)
              VKB_ENUM_MACRO(A2B10G10R10UintPack32)
              VKB_ENUM_MACRO(A2B10G10R10SintPack32)
              VKB_ENUM_MACRO(R16Unorm)
              VKB_ENUM_MACRO(R16Snorm)
              VKB_ENUM_MACRO(R16Uscaled)
              VKB_ENUM_MACRO(R16Sscaled)
              VKB_ENUM_MACRO(R16Uint)
              VKB_ENUM_MACRO(R16Sint)
              VKB_ENUM_MACRO(R16Sfloat)
              VKB_ENUM_MACRO(R16G16Unorm)
              VKB_ENUM_MACRO(R16G16Snorm)
              VKB_ENUM_MACRO(R16G16Uscaled)
              VKB_ENUM_MACRO(R16G16Sscaled)
              VKB_ENUM_MACRO(R16G16Uint)
              VKB_ENUM_MACRO(R16G16Sint)
              VKB_ENUM_MACRO(R16G16Sfloat)
              VKB_ENUM_MACRO(R16G16B16Unorm)
              VKB_ENUM_MACRO(R16G16B16Snorm)
              VKB_ENUM_MACRO(R16G16B16Uscaled)
              VKB_ENUM_MACRO(R16G16B16Sscaled)
              VKB_ENUM_MACRO(R16G16B16Uint)
              VKB_ENUM_MACRO(R16G16B16Sint)
              VKB_ENUM_MACRO(R16G16B16Sfloat)
              VKB_ENUM_MACRO(R16G16B16A16Unorm)
              VKB_ENUM_MACRO(R16G16B16A16Snorm)
              VKB_ENUM_MACRO(R16G16B16A16Uscaled)
              VKB_ENUM_MACRO(R16G16B16A16Sscaled)
              VKB_ENUM_MACRO(R16G16B16A16Uint)
              VKB_ENUM_MACRO(R16G16B16A16Sint)
              VKB_ENUM_MACRO(R16G16B16A16Sfloat)
              VKB_ENUM_MACRO(R32Uint)
              VKB_ENUM_MACRO(R32Sint)
              VKB_ENUM_MACRO(R32Sfloat)
              VKB_ENUM_MACRO(R32G32Uint)
              VKB_ENUM_MACRO(R32G32Sint)
              VKB_ENUM_MACRO(R32G32Sfloat)
              VKB_ENUM_MACRO(R32G32B32Uint)
              VKB_ENUM_MACRO(R32G32B32Sint)
              VKB_ENUM_MACRO(R32G32B32Sfloat)
              VKB_ENUM_MACRO(R32G32B32A32Uint)
              VKB_ENUM_MACRO(R32G32B32A32Sint)
              VKB_ENUM_MACRO(R32G32B32A32Sfloat)
              VKB_ENUM_MACRO(R64Uint)
              VKB_ENUM_MACRO(R64Sint)
              VKB_ENUM_MACRO(R64Sfloat)
              VKB_ENUM_MACRO(R64G64Uint)
              VKB_ENUM_MACRO(R64G64Sint)
              VKB_ENUM_MACRO(R64G64Sfloat)
              VKB_ENUM_MACRO(R64G64B64Uint)
              VKB_ENUM_MACRO(R64G64B64Sint)
              VKB_ENUM_MACRO(R64G64B64Sfloat)
              VKB_ENUM_MACRO(R64G64B64A64Uint)
              VKB_ENUM_MACRO(R64G64B64A64Sint)
              VKB_ENUM_MACRO(R64G64B64A64Sfloat)
              VKB_ENUM_MACRO(B10G11R11UfloatPack32)
              VKB_ENUM_MACRO(E5B9G9R9UfloatPack32)
              VKB_ENUM_MACRO(D16Unorm)
              VKB_ENUM_MACRO(X8D24UnormPack32)
              VKB_ENUM_MACRO(D32Sfloat)
              VKB_ENUM_MACRO(S8Uint)
              VKB_ENUM_MACRO(D16UnormS8Uint)
              VKB_ENUM_MACRO(D24UnormS8Uint)
              VKB_ENUM_MACRO(D32SfloatS8Uint)
              VKB_ENUM_MACRO(Bc1RgbUnormBlock)
              VKB_ENUM_MACRO(Bc1RgbSrgbBlock)
              VKB_ENUM_MACRO(Bc1RgbaUnormBlock)
              VKB_ENUM_MACRO(Bc1RgbaSrgbBlock)
              VKB_ENUM_MACRO(Bc2UnormBlock)
              VKB_ENUM_MACRO(Bc2SrgbBlock)
              VKB_ENUM_MACRO(Bc3UnormBlock)
              VKB_ENUM_MACRO(Bc3SrgbBlock)
              VKB_ENUM_MACRO(Bc4UnormBlock)
              VKB_ENUM_MACRO(Bc4SnormBlock)
              VKB_ENUM_MACRO(Bc5UnormBlock)
              VKB_ENUM_MACRO(Bc5SnormBlock)
              VKB_ENUM_MACRO(Bc6HUfloatBlock)
              VKB_ENUM_MACRO(Bc6HSfloatBlock)
              VKB_ENUM_MACRO(Bc7UnormBlock)
              VKB_ENUM_MACRO(Bc7SrgbBlock)
              VKB_ENUM_MACRO(Etc2R8G8B8UnormBlock)
              VKB_ENUM_MACRO(Etc2R8G8B8SrgbBlock)
              VKB_ENUM_MACRO(Etc2R8G8B8A1UnormBlock)
              VKB_ENUM_MACRO(Etc2R8G8B8A1SrgbBlock)
              VKB_ENUM_MACRO(Etc2R8G8B8A8UnormBlock)
              VKB_ENUM_MACRO(Etc2R8G8B8A8SrgbBlock)
              VKB_ENUM_MACRO(EacR11UnormBlock)
              VKB_ENUM_MACRO(EacR11SnormBlock)
              VKB_ENUM_MACRO(EacR11G11UnormBlock)
              VKB_ENUM_MACRO(EacR11G11SnormBlock)
              VKB_ENUM_MACRO(Astc4x4UnormBlock)
              VKB_ENUM_MACRO(Astc4x4SrgbBlock)
              VKB_ENUM_MACRO(Astc5x4UnormBlock)
              VKB_ENUM_MACRO(Astc5x4SrgbBlock)
              VKB_ENUM_MACRO(Astc5x5UnormBlock)
              VKB_ENUM_MACRO(Astc5x5SrgbBlock)
              VKB_ENUM_MACRO(Astc6x5UnormBlock)
              VKB_ENUM_MACRO(Astc6x5SrgbBlock)
              VKB_ENUM_MACRO(Astc6x6UnormBlock)
              VKB_ENUM_MACRO(Astc6x6SrgbBlock)
              VKB_ENUM_MACRO(Astc8x5UnormBlock)
              VKB_ENUM_MACRO(Astc8x5SrgbBlock)
              VKB_ENUM_MACRO(Astc8x6UnormBlock)
              VKB_ENUM_MACRO(Astc8x6SrgbBlock)
              VKB_ENUM_MACRO(Astc8x8UnormBlock)
              VKB_ENUM_MACRO(Astc8x8SrgbBlock)
              VKB_ENUM_MACRO(Astc10x5UnormBlock)
              VKB_ENUM_MACRO(Astc10x5SrgbBlock)
              VKB_ENUM_MACRO(Astc10x6UnormBlock)
              VKB_ENUM_MACRO(Astc10x6SrgbBlock)
              VKB_ENUM_MACRO(Astc10x8UnormBlock)
              VKB_ENUM_MACRO(Astc10x8SrgbBlock)
              VKB_ENUM_MACRO(Astc10x10UnormBlock)
              VKB_ENUM_MACRO(Astc10x10SrgbBlock)
              VKB_ENUM_MACRO(Astc12x10UnormBlock)
              VKB_ENUM_MACRO(Astc12x10SrgbBlock)
              VKB_ENUM_MACRO(Astc12x12UnormBlock)
              VKB_ENUM_MACRO(Astc12x12SrgbBlock)
              VKB_ENUM_MACRO(G8B8G8R8422Unorm)
              VKB_ENUM_MACRO(B8G8R8G8422Unorm)
              VKB_ENUM_MACRO(G8B8R83Plane420Unorm)
              VKB_ENUM_MACRO(G8B8R82Plane420Unorm)
              VKB_ENUM_MACRO(G8B8R83Plane422Unorm)
              VKB_ENUM_MACRO(G8B8R82Plane422Unorm)
              VKB_ENUM_MACRO(G8B8R83Plane444Unorm)
              VKB_ENUM_MACRO(R10X6UnormPack16)
              VKB_ENUM_MACRO(R10X6G10X6Unorm2Pack16)
              VKB_ENUM_MACRO(R10X6G10X6B10X6A10X6Unorm4Pack16)
              VKB_ENUM_MACRO(G10X6B10X6G10X6R10X6422Unorm4Pack16)
              VKB_ENUM_MACRO(B10X6G10X6R10X6G10X6422Unorm4Pack16)
              VKB_ENUM_MACRO(G10X6B10X6R10X63Plane420Unorm3Pack16)
              VKB_ENUM_MACRO(G10X6B10X6R10X62Plane420Unorm3Pack16)
              VKB_ENUM_MACRO(G10X6B10X6R10X63Plane422Unorm3Pack16)
              VKB_ENUM_MACRO(G10X6B10X6R10X62Plane422Unorm3Pack16)
              VKB_ENUM_MACRO(G10X6B10X6R10X63Plane444Unorm3Pack16)
              VKB_ENUM_MACRO(R12X4UnormPack16)
              VKB_ENUM_MACRO(R12X4G12X4Unorm2Pack16)
              VKB_ENUM_MACRO(R12X4G12X4B12X4A12X4Unorm4Pack16)
              VKB_ENUM_MACRO(G12X4B12X4G12X4R12X4422Unorm4Pack16)
              VKB_ENUM_MACRO(B12X4G12X4R12X4G12X4422Unorm4Pack16)
              VKB_ENUM_MACRO(G12X4B12X4R12X43Plane420Unorm3Pack16)
              VKB_ENUM_MACRO(G12X4B12X4R12X42Plane420Unorm3Pack16)
              VKB_ENUM_MACRO(G12X4B12X4R12X43Plane422Unorm3Pack16)
              VKB_ENUM_MACRO(G12X4B12X4R12X42Plane422Unorm3Pack16)
              VKB_ENUM_MACRO(G12X4B12X4R12X43Plane444Unorm3Pack16)
              VKB_ENUM_MACRO(G16B16G16R16422Unorm)
              VKB_ENUM_MACRO(B16G16R16G16422Unorm)
              VKB_ENUM_MACRO(G16B16R163Plane420Unorm)
              VKB_ENUM_MACRO(G16B16R162Plane420Unorm)
              VKB_ENUM_MACRO(G16B16R163Plane422Unorm)
              VKB_ENUM_MACRO(G16B16R162Plane422Unorm)
              VKB_ENUM_MACRO(G16B16R163Plane444Unorm)
              VKB_ENUM_MACRO(Pvrtc12BppUnormBlockIMG)
              VKB_ENUM_MACRO(Pvrtc14BppUnormBlockIMG)
              VKB_ENUM_MACRO(Pvrtc22BppUnormBlockIMG)
              VKB_ENUM_MACRO(Pvrtc24BppUnormBlockIMG)
              VKB_ENUM_MACRO(Pvrtc12BppSrgbBlockIMG)
              VKB_ENUM_MACRO(Pvrtc14BppSrgbBlockIMG)
              VKB_ENUM_MACRO(Pvrtc22BppSrgbBlockIMG)
              VKB_ENUM_MACRO(Pvrtc24BppSrgbBlockIMG)
              VKB_ENUM_MACRO(Astc4x4SfloatBlockEXT)
              VKB_ENUM_MACRO(Astc5x4SfloatBlockEXT)
              VKB_ENUM_MACRO(Astc5x5SfloatBlockEXT)
              VKB_ENUM_MACRO(Astc6x5SfloatBlockEXT)
              VKB_ENUM_MACRO(Astc6x6SfloatBlockEXT)
              VKB_ENUM_MACRO(Astc8x5SfloatBlockEXT)
              VKB_ENUM_MACRO(Astc8x6SfloatBlockEXT)
              VKB_ENUM_MACRO(Astc8x8SfloatBlockEXT)
              VKB_ENUM_MACRO(Astc10x5SfloatBlockEXT)
              VKB_ENUM_MACRO(Astc10x6SfloatBlockEXT)
              VKB_ENUM_MACRO(Astc10x8SfloatBlockEXT)
              VKB_ENUM_MACRO(Astc10x10SfloatBlockEXT)
              VKB_ENUM_MACRO(Astc12x10SfloatBlockEXT)
              VKB_ENUM_MACRO(Astc12x12SfloatBlockEXT)
              VKB_ENUM_MACRO(G8B8G8R8422UnormKHR)
              VKB_ENUM_MACRO(B8G8R8G8422UnormKHR)
              VKB_ENUM_MACRO(G8B8R83Plane420UnormKHR)
              VKB_ENUM_MACRO(G8B8R82Plane420UnormKHR)
              VKB_ENUM_MACRO(G8B8R83Plane422UnormKHR)
              VKB_ENUM_MACRO(G8B8R82Plane422UnormKHR)
              VKB_ENUM_MACRO(G8B8R83Plane444UnormKHR)
              VKB_ENUM_MACRO(R10X6UnormPack16KHR)
              VKB_ENUM_MACRO(R10X6G10X6Unorm2Pack16KHR)
              VKB_ENUM_MACRO(R10X6G10X6B10X6A10X6Unorm4Pack16KHR)
              VKB_ENUM_MACRO(G10X6B10X6G10X6R10X6422Unorm4Pack16KHR)
              VKB_ENUM_MACRO(B10X6G10X6R10X6G10X6422Unorm4Pack16KHR)
              VKB_ENUM_MACRO(G10X6B10X6R10X63Plane420Unorm3Pack16KHR)
              VKB_ENUM_MACRO(G10X6B10X6R10X62Plane420Unorm3Pack16KHR)
              VKB_ENUM_MACRO(G10X6B10X6R10X63Plane422Unorm3Pack16KHR)
              VKB_ENUM_MACRO(G10X6B10X6R10X62Plane422Unorm3Pack16KHR)
              VKB_ENUM_MACRO(G10X6B10X6R10X63Plane444Unorm3Pack16KHR)
              VKB_ENUM_MACRO(R12X4UnormPack16KHR)
              VKB_ENUM_MACRO(R12X4G12X4Unorm2Pack16KHR)
              VKB_ENUM_MACRO(R12X4G12X4B12X4A12X4Unorm4Pack16KHR)
              VKB_ENUM_MACRO(G12X4B12X4G12X4R12X4422Unorm4Pack16KHR)
              VKB_ENUM_MACRO(B12X4G12X4R12X4G12X4422Unorm4Pack16KHR)
              VKB_ENUM_MACRO(G12X4B12X4R12X43Plane420Unorm3Pack16KHR)
              VKB_ENUM_MACRO(G12X4B12X4R12X42Plane420Unorm3Pack16KHR)
              VKB_ENUM_MACRO(G12X4B12X4R12X43Plane422Unorm3Pack16KHR)
              VKB_ENUM_MACRO(G12X4B12X4R12X42Plane422Unorm3Pack16KHR)
              VKB_ENUM_MACRO(G12X4B12X4R12X43Plane444Unorm3Pack16KHR)
              VKB_ENUM_MACRO(G16B16G16R16422UnormKHR)
              VKB_ENUM_MACRO(B16G16R16G16422UnormKHR)
              VKB_ENUM_MACRO(G16B16R163Plane420UnormKHR)
              VKB_ENUM_MACRO(G16B16R162Plane420UnormKHR)
              VKB_ENUM_MACRO(G16B16R163Plane422UnormKHR)
              VKB_ENUM_MACRO(G16B16R162Plane422UnormKHR)
              VKB_ENUM_MACRO(G16B16R163Plane444UnormKHR)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, FormatFeatureFlagBits &p)
  {
      using enum_c_type = FormatFeatureFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(SampledImage)
              VKB_ENUM_MACRO(StorageImage)
              VKB_ENUM_MACRO(StorageImageAtomic)
              VKB_ENUM_MACRO(UniformTexelBuffer)
              VKB_ENUM_MACRO(StorageTexelBuffer)
              VKB_ENUM_MACRO(StorageTexelBufferAtomic)
              VKB_ENUM_MACRO(VertexBuffer)
              VKB_ENUM_MACRO(ColorAttachment)
              VKB_ENUM_MACRO(ColorAttachmentBlend)
              VKB_ENUM_MACRO(DepthStencilAttachment)
              VKB_ENUM_MACRO(BlitSrc)
              VKB_ENUM_MACRO(BlitDst)
              VKB_ENUM_MACRO(SampledImageFilterLinear)
              VKB_ENUM_MACRO(TransferSrc)
              VKB_ENUM_MACRO(TransferDst)
              VKB_ENUM_MACRO(MidpointChromaSamples)
              VKB_ENUM_MACRO(SampledImageYcbcrConversionLinearFilter)
              VKB_ENUM_MACRO(SampledImageYcbcrConversionSeparateReconstructionFilter)
              VKB_ENUM_MACRO(SampledImageYcbcrConversionChromaReconstructionExplicit)
              VKB_ENUM_MACRO(SampledImageYcbcrConversionChromaReconstructionExplicitForceable)
              VKB_ENUM_MACRO(Disjoint)
              VKB_ENUM_MACRO(CositedChromaSamples)
              VKB_ENUM_MACRO(SampledImageFilterMinmax)
              VKB_ENUM_MACRO(SampledImageFilterCubicIMG)
              VKB_ENUM_MACRO(FragmentDensityMapEXT)
              VKB_ENUM_MACRO(TransferSrcKHR)
              VKB_ENUM_MACRO(TransferDstKHR)
              VKB_ENUM_MACRO(SampledImageFilterMinmaxEXT)
              VKB_ENUM_MACRO(MidpointChromaSamplesKHR)
              VKB_ENUM_MACRO(SampledImageYcbcrConversionLinearFilterKHR)
              VKB_ENUM_MACRO(SampledImageYcbcrConversionSeparateReconstructionFilterKHR)
              VKB_ENUM_MACRO(SampledImageYcbcrConversionChromaReconstructionExplicitKHR)
              VKB_ENUM_MACRO(SampledImageYcbcrConversionChromaReconstructionExplicitForceableKHR)
              VKB_ENUM_MACRO(DisjointKHR)
              VKB_ENUM_MACRO(CositedChromaSamplesKHR)
              VKB_ENUM_MACRO(SampledImageFilterCubicEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, FramebufferCreateFlagBits &p)
  {
      using enum_c_type = FramebufferCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Imageless)
              VKB_ENUM_MACRO(ImagelessKHR)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, FrontFace &p)
  {
      using enum_c_type = FrontFace;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(CounterClockwise)
              VKB_ENUM_MACRO(Clockwise)
          VKB_END_ENUM_MACRO
  }
#if 0
  inline void from_json(nlohmann::json const & j, FullScreenExclusiveEXT &p)
  {
      using enum_c_type = FullScreenExclusiveEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Default)
              VKB_ENUM_MACRO(Allowed)
              VKB_ENUM_MACRO(Disallowed)
              VKB_ENUM_MACRO(ApplicationControlled)
          VKB_END_ENUM_MACRO
  }
#endif
  inline void from_json(nlohmann::json const & j, GeometryFlagBitsNV &p)
  {
      using enum_c_type = GeometryFlagBitsNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Opaque)
              VKB_ENUM_MACRO(NoDuplicateAnyHitInvocation)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, GeometryInstanceFlagBitsNV &p)
  {
      using enum_c_type = GeometryInstanceFlagBitsNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(TriangleCullDisable)
              VKB_ENUM_MACRO(TriangleFrontCounterclockwise)
              VKB_ENUM_MACRO(ForceOpaque)
              VKB_ENUM_MACRO(ForceNoOpaque)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, GeometryTypeNV &p)
  {
      using enum_c_type = GeometryTypeNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Triangles)
              VKB_ENUM_MACRO(Aabbs)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ImageAspectFlagBits &p)
  {
      using enum_c_type = ImageAspectFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Color)
              VKB_ENUM_MACRO(Depth)
              VKB_ENUM_MACRO(Stencil)
              VKB_ENUM_MACRO(Metadata)
              VKB_ENUM_MACRO(Plane0)
              VKB_ENUM_MACRO(Plane1)
              VKB_ENUM_MACRO(Plane2)
              VKB_ENUM_MACRO(MemoryPlane0EXT)
              VKB_ENUM_MACRO(MemoryPlane1EXT)
              VKB_ENUM_MACRO(MemoryPlane2EXT)
              VKB_ENUM_MACRO(MemoryPlane3EXT)
              VKB_ENUM_MACRO(Plane0KHR)
              VKB_ENUM_MACRO(Plane1KHR)
              VKB_ENUM_MACRO(Plane2KHR)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ImageCreateFlagBits &p)
  {
      using enum_c_type = ImageCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(SparseBinding)
              VKB_ENUM_MACRO(SparseResidency)
              VKB_ENUM_MACRO(SparseAliased)
              VKB_ENUM_MACRO(MutableFormat)
              VKB_ENUM_MACRO(CubeCompatible)
              VKB_ENUM_MACRO(Alias)
              VKB_ENUM_MACRO(SplitInstanceBindRegions)
              VKB_ENUM_MACRO(2DArrayCompatible)
              VKB_ENUM_MACRO(BlockTexelViewCompatible)
              VKB_ENUM_MACRO(ExtendedUsage)
              VKB_ENUM_MACRO(Protected)
              VKB_ENUM_MACRO(Disjoint)
              VKB_ENUM_MACRO(CornerSampledNV)
              VKB_ENUM_MACRO(SampleLocationsCompatibleDepthEXT)
              VKB_ENUM_MACRO(SubsampledEXT)
              VKB_ENUM_MACRO(SplitInstanceBindRegionsKHR)
              VKB_ENUM_MACRO(2DArrayCompatibleKHR)
              VKB_ENUM_MACRO(BlockTexelViewCompatibleKHR)
              VKB_ENUM_MACRO(ExtendedUsageKHR)
              VKB_ENUM_MACRO(DisjointKHR)
              VKB_ENUM_MACRO(AliasKHR)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ImageLayout &p)
  {
      using enum_c_type = ImageLayout;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Undefined)
              VKB_ENUM_MACRO(General)
              VKB_ENUM_MACRO(ColorAttachmentOptimal)
              VKB_ENUM_MACRO(DepthStencilAttachmentOptimal)
              VKB_ENUM_MACRO(DepthStencilReadOnlyOptimal)
              VKB_ENUM_MACRO(ShaderReadOnlyOptimal)
              VKB_ENUM_MACRO(TransferSrcOptimal)
              VKB_ENUM_MACRO(TransferDstOptimal)
              VKB_ENUM_MACRO(Preinitialized)
              VKB_ENUM_MACRO(DepthReadOnlyStencilAttachmentOptimal)
              VKB_ENUM_MACRO(DepthAttachmentStencilReadOnlyOptimal)
              VKB_ENUM_MACRO(DepthAttachmentOptimal)
              VKB_ENUM_MACRO(DepthReadOnlyOptimal)
              VKB_ENUM_MACRO(StencilAttachmentOptimal)
              VKB_ENUM_MACRO(StencilReadOnlyOptimal)
              VKB_ENUM_MACRO(PresentSrcKHR)
              VKB_ENUM_MACRO(SharedPresentKHR)
              VKB_ENUM_MACRO(ShadingRateOptimalNV)
              VKB_ENUM_MACRO(FragmentDensityMapOptimalEXT)
              VKB_ENUM_MACRO(DepthReadOnlyStencilAttachmentOptimalKHR)
              VKB_ENUM_MACRO(DepthAttachmentStencilReadOnlyOptimalKHR)
              VKB_ENUM_MACRO(DepthAttachmentOptimalKHR)
              VKB_ENUM_MACRO(DepthReadOnlyOptimalKHR)
              VKB_ENUM_MACRO(StencilAttachmentOptimalKHR)
              VKB_ENUM_MACRO(StencilReadOnlyOptimalKHR)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ImageTiling &p)
  {
      using enum_c_type = ImageTiling;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Optimal)
              VKB_ENUM_MACRO(Linear)
              VKB_ENUM_MACRO(DrmFormatModifierEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ImageType &p)
  {
      using enum_c_type = ImageType;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(1D)
              VKB_ENUM_MACRO(2D)
              VKB_ENUM_MACRO(3D)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ImageUsageFlagBits &p)
  {
      using enum_c_type = ImageUsageFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(TransferSrc)
              VKB_ENUM_MACRO(TransferDst)
              VKB_ENUM_MACRO(Sampled)
              VKB_ENUM_MACRO(Storage)
              VKB_ENUM_MACRO(ColorAttachment)
              VKB_ENUM_MACRO(DepthStencilAttachment)
              VKB_ENUM_MACRO(TransientAttachment)
              VKB_ENUM_MACRO(InputAttachment)
              VKB_ENUM_MACRO(ShadingRateImageNV)
              VKB_ENUM_MACRO(FragmentDensityMapEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ImageViewCreateFlagBits &p)
  {
      using enum_c_type = ImageViewCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(FragmentDensityMapDynamicEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ImageViewType &p)
  {
      using enum_c_type = ImageViewType;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(1D)
              VKB_ENUM_MACRO(2D)
              VKB_ENUM_MACRO(3D)
              VKB_ENUM_MACRO(Cube)
              VKB_ENUM_MACRO(1DArray)
              VKB_ENUM_MACRO(2DArray)
              VKB_ENUM_MACRO(CubeArray)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, IndexType &p)
  {
      using enum_c_type = IndexType;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Uint16)
              VKB_ENUM_MACRO(Uint32)
              VKB_ENUM_MACRO(NoneNV)
              VKB_ENUM_MACRO(Uint8EXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, IndirectCommandsLayoutUsageFlagBitsNVX &p)
  {
      using enum_c_type = IndirectCommandsLayoutUsageFlagBitsNVX;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(UnorderedSequences)
              VKB_ENUM_MACRO(SparseSequences)
              VKB_ENUM_MACRO(EmptyExecutions)
              VKB_ENUM_MACRO(IndexedSequences)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, IndirectCommandsTokenTypeNVX &p)
  {
      using enum_c_type = IndirectCommandsTokenTypeNVX;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Pipeline)
              VKB_ENUM_MACRO(DescriptorSet)
              VKB_ENUM_MACRO(IndexBuffer)
              VKB_ENUM_MACRO(VertexBuffer)
              VKB_ENUM_MACRO(PushConstant)
              VKB_ENUM_MACRO(DrawIndexed)
              VKB_ENUM_MACRO(Draw)
              VKB_ENUM_MACRO(Dispatch)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, InstanceCreateFlagBits &p)
  {
      using enum_c_type = InstanceCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, InternalAllocationType &p)
  {
      using enum_c_type = InternalAllocationType;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Executable)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, LineRasterizationModeEXT &p)
  {
      using enum_c_type = LineRasterizationModeEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Default)
              VKB_ENUM_MACRO(Rectangular)
              VKB_ENUM_MACRO(Bresenham)
              VKB_ENUM_MACRO(RectangularSmooth)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, LogicOp &p)
  {
      using enum_c_type = LogicOp;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Clear)
              VKB_ENUM_MACRO(And)
              VKB_ENUM_MACRO(AndReverse)
              VKB_ENUM_MACRO(Copy)
              VKB_ENUM_MACRO(AndInverted)
              VKB_ENUM_MACRO(NoOp)
              VKB_ENUM_MACRO(Xor)
              VKB_ENUM_MACRO(Or)
              VKB_ENUM_MACRO(Nor)
              VKB_ENUM_MACRO(Equivalent)
              VKB_ENUM_MACRO(Invert)
              VKB_ENUM_MACRO(OrReverse)
              VKB_ENUM_MACRO(CopyInverted)
              VKB_ENUM_MACRO(OrInverted)
              VKB_ENUM_MACRO(Nand)
              VKB_ENUM_MACRO(Set)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, MemoryAllocateFlagBits &p)
  {
      using enum_c_type = MemoryAllocateFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(DeviceMask)
              VKB_ENUM_MACRO(DeviceAddress)
              VKB_ENUM_MACRO(DeviceAddressCaptureReplay)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, MemoryHeapFlagBits &p)
  {
      using enum_c_type = MemoryHeapFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(DeviceLocal)
              VKB_ENUM_MACRO(MultiInstance)
              VKB_ENUM_MACRO(MultiInstanceKHR)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, MemoryOverallocationBehaviorAMD &p)
  {
      using enum_c_type = MemoryOverallocationBehaviorAMD;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Default)
              VKB_ENUM_MACRO(Allowed)
              VKB_ENUM_MACRO(Disallowed)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, MemoryPropertyFlagBits &p)
  {
      using enum_c_type = MemoryPropertyFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(DeviceLocal)
              VKB_ENUM_MACRO(HostVisible)
              VKB_ENUM_MACRO(HostCoherent)
              VKB_ENUM_MACRO(HostCached)
              VKB_ENUM_MACRO(LazilyAllocated)
              VKB_ENUM_MACRO(Protected)
              VKB_ENUM_MACRO(DeviceCoherentAMD)
              VKB_ENUM_MACRO(DeviceUncachedAMD)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ObjectEntryTypeNVX &p)
  {
      using enum_c_type = ObjectEntryTypeNVX;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(DescriptorSet)
              VKB_ENUM_MACRO(Pipeline)
              VKB_ENUM_MACRO(IndexBuffer)
              VKB_ENUM_MACRO(VertexBuffer)
              VKB_ENUM_MACRO(PushConstant)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ObjectEntryUsageFlagBitsNVX &p)
  {
      using enum_c_type = ObjectEntryUsageFlagBitsNVX;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Graphics)
              VKB_ENUM_MACRO(Compute)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ObjectType &p)
  {
      using enum_c_type = ObjectType;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Unknown)
              VKB_ENUM_MACRO(Instance)
              VKB_ENUM_MACRO(PhysicalDevice)
              VKB_ENUM_MACRO(Device)
              VKB_ENUM_MACRO(Queue)
              VKB_ENUM_MACRO(Semaphore)
              VKB_ENUM_MACRO(CommandBuffer)
              VKB_ENUM_MACRO(Fence)
              VKB_ENUM_MACRO(DeviceMemory)
              VKB_ENUM_MACRO(Buffer)
              VKB_ENUM_MACRO(Image)
              VKB_ENUM_MACRO(Event)
              VKB_ENUM_MACRO(QueryPool)
              VKB_ENUM_MACRO(BufferView)
              VKB_ENUM_MACRO(ImageView)
              VKB_ENUM_MACRO(ShaderModule)
              VKB_ENUM_MACRO(PipelineCache)
              VKB_ENUM_MACRO(PipelineLayout)
              VKB_ENUM_MACRO(RenderPass)
              VKB_ENUM_MACRO(Pipeline)
              VKB_ENUM_MACRO(DescriptorSetLayout)
              VKB_ENUM_MACRO(Sampler)
              VKB_ENUM_MACRO(DescriptorPool)
              VKB_ENUM_MACRO(DescriptorSet)
              VKB_ENUM_MACRO(Framebuffer)
              VKB_ENUM_MACRO(CommandPool)
              VKB_ENUM_MACRO(SamplerYcbcrConversion)
              VKB_ENUM_MACRO(DescriptorUpdateTemplate)
              VKB_ENUM_MACRO(SurfaceKHR)
              VKB_ENUM_MACRO(SwapchainKHR)
              VKB_ENUM_MACRO(DisplayKHR)
              VKB_ENUM_MACRO(DisplayModeKHR)
              VKB_ENUM_MACRO(DebugReportCallbackEXT)
              VKB_ENUM_MACRO(ObjectTableNVX)
              VKB_ENUM_MACRO(IndirectCommandsLayoutNVX)
              VKB_ENUM_MACRO(DebugUtilsMessengerEXT)
              VKB_ENUM_MACRO(ValidationCacheEXT)
              VKB_ENUM_MACRO(AccelerationStructureNV)
              VKB_ENUM_MACRO(PerformanceConfigurationINTEL)
              VKB_ENUM_MACRO(DescriptorUpdateTemplateKHR)
              VKB_ENUM_MACRO(SamplerYcbcrConversionKHR)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PeerMemoryFeatureFlagBits &p)
  {
      using enum_c_type = PeerMemoryFeatureFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(CopySrc)
              VKB_ENUM_MACRO(CopyDst)
              VKB_ENUM_MACRO(GenericSrc)
              VKB_ENUM_MACRO(GenericDst)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PerformanceConfigurationTypeINTEL &p)
  {
      using enum_c_type = PerformanceConfigurationTypeINTEL;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(CommandQueueMetricsDiscoveryActivated)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PerformanceCounterDescriptionFlagBitsKHR &p)
  {
      using enum_c_type = PerformanceCounterDescriptionFlagBitsKHR;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(PerformanceImpacting)
              VKB_ENUM_MACRO(ConcurrentlyImpacted)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PerformanceCounterScopeKHR &p)
  {
      using enum_c_type = PerformanceCounterScopeKHR;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(CommandBuffer)
              VKB_ENUM_MACRO(RenderPass)
              VKB_ENUM_MACRO(Command)
              VKB_ENUM_MACRO(VkQueryScopeCommandBuffer)
              VKB_ENUM_MACRO(VkQueryScopeRenderPass)
              VKB_ENUM_MACRO(VkQueryScopeCommand)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PerformanceCounterStorageKHR &p)
  {
      using enum_c_type = PerformanceCounterStorageKHR;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Int32)
              VKB_ENUM_MACRO(Int64)
              VKB_ENUM_MACRO(Uint32)
              VKB_ENUM_MACRO(Uint64)
              VKB_ENUM_MACRO(Float32)
              VKB_ENUM_MACRO(Float64)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PerformanceCounterUnitKHR &p)
  {
      using enum_c_type = PerformanceCounterUnitKHR;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Generic)
              VKB_ENUM_MACRO(Percentage)
              VKB_ENUM_MACRO(Nanoseconds)
              VKB_ENUM_MACRO(Bytes)
              VKB_ENUM_MACRO(BytesPerSecond)
              VKB_ENUM_MACRO(Kelvin)
              VKB_ENUM_MACRO(Watts)
              VKB_ENUM_MACRO(Volts)
              VKB_ENUM_MACRO(Amps)
              VKB_ENUM_MACRO(Hertz)
              VKB_ENUM_MACRO(Cycles)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PerformanceOverrideTypeINTEL &p)
  {
      using enum_c_type = PerformanceOverrideTypeINTEL;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(NullHardware)
              VKB_ENUM_MACRO(FlushGpuCaches)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PerformanceParameterTypeINTEL &p)
  {
      using enum_c_type = PerformanceParameterTypeINTEL;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(HwCountersSupported)
              VKB_ENUM_MACRO(StreamMarkerValidBits)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PerformanceValueTypeINTEL &p)
  {
      using enum_c_type = PerformanceValueTypeINTEL;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Uint32)
              VKB_ENUM_MACRO(Uint64)
              VKB_ENUM_MACRO(Float)
              VKB_ENUM_MACRO(Bool)
              VKB_ENUM_MACRO(String)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PhysicalDeviceType &p)
  {
      using enum_c_type = PhysicalDeviceType;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Other)
              VKB_ENUM_MACRO(IntegratedGpu)
              VKB_ENUM_MACRO(DiscreteGpu)
              VKB_ENUM_MACRO(VirtualGpu)
              VKB_ENUM_MACRO(Cpu)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineBindPoint &p)
  {
      using enum_c_type = PipelineBindPoint;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Graphics)
              VKB_ENUM_MACRO(Compute)
              VKB_ENUM_MACRO(RayTracingNV)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineCacheCreateFlagBits &p)
  {
      using enum_c_type = PipelineCacheCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineCacheHeaderVersion &p)
  {
      using enum_c_type = PipelineCacheHeaderVersion;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(One)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineColorBlendStateCreateFlagBits &p)
  {
      using enum_c_type = PipelineColorBlendStateCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineCompilerControlFlagBitsAMD &p)
  {
      using enum_c_type = PipelineCompilerControlFlagBitsAMD;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineCreateFlagBits &p)
  {
      using enum_c_type = PipelineCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(DisableOptimization)
              VKB_ENUM_MACRO(AllowDerivatives)
              VKB_ENUM_MACRO(Derivative)
              VKB_ENUM_MACRO(ViewIndexFromDeviceIndex)
              VKB_ENUM_MACRO(DispatchBase)
              VKB_ENUM_MACRO(DeferCompileNV)
              VKB_ENUM_MACRO(CaptureStatisticsKHR)
              VKB_ENUM_MACRO(CaptureInternalRepresentationsKHR)
              VKB_ENUM_MACRO(ViewIndexFromDeviceIndexKHR)
              VKB_ENUM_MACRO(DispatchBaseKHR)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineCreationFeedbackFlagBitsEXT &p)
  {
      using enum_c_type = PipelineCreationFeedbackFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Valid)
              VKB_ENUM_MACRO(ApplicationPipelineCacheHit)
              VKB_ENUM_MACRO(BasePipelineAcceleration)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineDepthStencilStateCreateFlagBits &p)
  {
      using enum_c_type = PipelineDepthStencilStateCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineDynamicStateCreateFlagBits &p)
  {
      using enum_c_type = PipelineDynamicStateCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineExecutableStatisticFormatKHR &p)
  {
      using enum_c_type = PipelineExecutableStatisticFormatKHR;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Bool32)
              VKB_ENUM_MACRO(Int64)
              VKB_ENUM_MACRO(Uint64)
              VKB_ENUM_MACRO(Float64)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineInputAssemblyStateCreateFlagBits &p)
  {
      using enum_c_type = PipelineInputAssemblyStateCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineLayoutCreateFlagBits &p)
  {
      using enum_c_type = PipelineLayoutCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineMultisampleStateCreateFlagBits &p)
  {
      using enum_c_type = PipelineMultisampleStateCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineRasterizationStateCreateFlagBits &p)
  {
      using enum_c_type = PipelineRasterizationStateCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineShaderStageCreateFlagBits &p)
  {
      using enum_c_type = PipelineShaderStageCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(AllowVaryingSubgroupSizeEXT)
              VKB_ENUM_MACRO(RequireFullSubgroupsEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineStageFlagBits &p)
  {
      using enum_c_type = PipelineStageFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(TopOfPipe)
              VKB_ENUM_MACRO(DrawIndirect)
              VKB_ENUM_MACRO(VertexInput)
              VKB_ENUM_MACRO(VertexShader)
              VKB_ENUM_MACRO(TessellationControlShader)
              VKB_ENUM_MACRO(TessellationEvaluationShader)
              VKB_ENUM_MACRO(GeometryShader)
              VKB_ENUM_MACRO(FragmentShader)
              VKB_ENUM_MACRO(EarlyFragmentTests)
              VKB_ENUM_MACRO(LateFragmentTests)
              VKB_ENUM_MACRO(ColorAttachmentOutput)
              VKB_ENUM_MACRO(ComputeShader)
              VKB_ENUM_MACRO(Transfer)
              VKB_ENUM_MACRO(BottomOfPipe)
              VKB_ENUM_MACRO(Host)
              VKB_ENUM_MACRO(AllGraphics)
              VKB_ENUM_MACRO(AllCommands)
              VKB_ENUM_MACRO(TransformFeedbackEXT)
              VKB_ENUM_MACRO(ConditionalRenderingEXT)
              VKB_ENUM_MACRO(CommandProcessNVX)
              VKB_ENUM_MACRO(ShadingRateImageNV)
              VKB_ENUM_MACRO(RayTracingShaderNV)
              VKB_ENUM_MACRO(AccelerationStructureBuildNV)
              VKB_ENUM_MACRO(TaskShaderNV)
              VKB_ENUM_MACRO(MeshShaderNV)
              VKB_ENUM_MACRO(FragmentDensityProcessEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineTessellationStateCreateFlagBits &p)
  {
      using enum_c_type = PipelineTessellationStateCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineVertexInputStateCreateFlagBits &p)
  {
      using enum_c_type = PipelineVertexInputStateCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineViewportStateCreateFlagBits &p)
  {
      using enum_c_type = PipelineViewportStateCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PointClippingBehavior &p)
  {
      using enum_c_type = PointClippingBehavior;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(AllClipPlanes)
              VKB_ENUM_MACRO(UserClipPlanesOnly)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PolygonMode &p)
  {
      using enum_c_type = PolygonMode;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Fill)
              VKB_ENUM_MACRO(Line)
              VKB_ENUM_MACRO(Point)
              VKB_ENUM_MACRO(FillRectangleNV)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PresentModeKHR &p)
  {
      using enum_c_type = PresentModeKHR;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Immediate)
              VKB_ENUM_MACRO(Mailbox)
              VKB_ENUM_MACRO(Fifo)
              VKB_ENUM_MACRO(FifoRelaxed)
              VKB_ENUM_MACRO(SharedDemandRefresh)
              VKB_ENUM_MACRO(SharedContinuousRefresh)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PrimitiveTopology &p)
  {
      using enum_c_type = PrimitiveTopology;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(PointList)
              VKB_ENUM_MACRO(LineList)
              VKB_ENUM_MACRO(LineStrip)
              VKB_ENUM_MACRO(TriangleList)
              VKB_ENUM_MACRO(TriangleStrip)
              VKB_ENUM_MACRO(TriangleFan)
              VKB_ENUM_MACRO(LineListWithAdjacency)
              VKB_ENUM_MACRO(LineStripWithAdjacency)
              VKB_ENUM_MACRO(TriangleListWithAdjacency)
              VKB_ENUM_MACRO(TriangleStripWithAdjacency)
              VKB_ENUM_MACRO(PatchList)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, QueryControlFlagBits &p)
  {
      using enum_c_type = QueryControlFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Precise)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, QueryPipelineStatisticFlagBits &p)
  {
      using enum_c_type = QueryPipelineStatisticFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(InputAssemblyVertices)
              VKB_ENUM_MACRO(InputAssemblyPrimitives)
              VKB_ENUM_MACRO(VertexShaderInvocations)
              VKB_ENUM_MACRO(GeometryShaderInvocations)
              VKB_ENUM_MACRO(GeometryShaderPrimitives)
              VKB_ENUM_MACRO(ClippingInvocations)
              VKB_ENUM_MACRO(ClippingPrimitives)
              VKB_ENUM_MACRO(FragmentShaderInvocations)
              VKB_ENUM_MACRO(TessellationControlShaderPatches)
              VKB_ENUM_MACRO(TessellationEvaluationShaderInvocations)
              VKB_ENUM_MACRO(ComputeShaderInvocations)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, QueryPoolCreateFlagBits &p)
  {
      using enum_c_type = QueryPoolCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, QueryPoolSamplingModeINTEL &p)
  {
      using enum_c_type = QueryPoolSamplingModeINTEL;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Manual)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, QueryResultFlagBits &p)
  {
      using enum_c_type = QueryResultFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(64)
              VKB_ENUM_MACRO(Wait)
              VKB_ENUM_MACRO(WithAvailability)
              VKB_ENUM_MACRO(Partial)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, QueryType &p)
  {
      using enum_c_type = QueryType;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Occlusion)
              VKB_ENUM_MACRO(PipelineStatistics)
              VKB_ENUM_MACRO(Timestamp)
              VKB_ENUM_MACRO(TransformFeedbackStreamEXT)
              VKB_ENUM_MACRO(PerformanceQueryKHR)
              VKB_ENUM_MACRO(AccelerationStructureCompactedSizeNV)
              VKB_ENUM_MACRO(PerformanceQueryINTEL)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, QueueFlagBits &p)
  {
      using enum_c_type = QueueFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Graphics)
              VKB_ENUM_MACRO(Compute)
              VKB_ENUM_MACRO(Transfer)
              VKB_ENUM_MACRO(SparseBinding)
              VKB_ENUM_MACRO(Protected)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, QueueGlobalPriorityEXT &p)
  {
      using enum_c_type = QueueGlobalPriorityEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Low)
              VKB_ENUM_MACRO(Medium)
              VKB_ENUM_MACRO(High)
              VKB_ENUM_MACRO(Realtime)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, RasterizationOrderAMD &p)
  {
      using enum_c_type = RasterizationOrderAMD;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Strict)
              VKB_ENUM_MACRO(Relaxed)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, RayTracingShaderGroupTypeNV &p)
  {
      using enum_c_type = RayTracingShaderGroupTypeNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(General)
              VKB_ENUM_MACRO(TrianglesHitGroup)
              VKB_ENUM_MACRO(ProceduralHitGroup)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, RenderPassCreateFlagBits &p)
  {
      using enum_c_type = RenderPassCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ResolveModeFlagBits &p)
  {
      using enum_c_type = ResolveModeFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(None)
              VKB_ENUM_MACRO(SampleZero)
              VKB_ENUM_MACRO(Average)
              VKB_ENUM_MACRO(Min)
              VKB_ENUM_MACRO(Max)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, Result &p)
  {
      using enum_c_type = Result;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Success)
              VKB_ENUM_MACRO(NotReady)
              VKB_ENUM_MACRO(Timeout)
              VKB_ENUM_MACRO(EventSet)
              VKB_ENUM_MACRO(EventReset)
              VKB_ENUM_MACRO(Incomplete)
              VKB_ENUM_MACRO(ErrorOutOfHostMemory)
              VKB_ENUM_MACRO(ErrorOutOfDeviceMemory)
              VKB_ENUM_MACRO(ErrorInitializationFailed)
              VKB_ENUM_MACRO(ErrorDeviceLost)
              VKB_ENUM_MACRO(ErrorMemoryMapFailed)
              VKB_ENUM_MACRO(ErrorLayerNotPresent)
              VKB_ENUM_MACRO(ErrorExtensionNotPresent)
              VKB_ENUM_MACRO(ErrorFeatureNotPresent)
              VKB_ENUM_MACRO(ErrorIncompatibleDriver)
              VKB_ENUM_MACRO(ErrorTooManyObjects)
              VKB_ENUM_MACRO(ErrorFormatNotSupported)
              VKB_ENUM_MACRO(ErrorFragmentedPool)
              VKB_ENUM_MACRO(ErrorUnknown)
              VKB_ENUM_MACRO(ErrorOutOfPoolMemory)
              VKB_ENUM_MACRO(ErrorInvalidExternalHandle)
              VKB_ENUM_MACRO(ErrorFragmentation)
              VKB_ENUM_MACRO(ErrorInvalidOpaqueCaptureAddress)
              VKB_ENUM_MACRO(ErrorSurfaceLostKHR)
              VKB_ENUM_MACRO(ErrorNativeWindowInUseKHR)
              VKB_ENUM_MACRO(SuboptimalKHR)
              VKB_ENUM_MACRO(ErrorOutOfDateKHR)
              VKB_ENUM_MACRO(ErrorIncompatibleDisplayKHR)
              VKB_ENUM_MACRO(ErrorValidationFailedEXT)
              VKB_ENUM_MACRO(ErrorInvalidShaderNV)
              VKB_ENUM_MACRO(ErrorInvalidDrmFormatModifierPlaneLayoutEXT)
              VKB_ENUM_MACRO(ErrorNotPermittedEXT)
              VKB_ENUM_MACRO(ErrorFullScreenExclusiveModeLostEXT)
              VKB_ENUM_MACRO(ErrorOutOfPoolMemoryKHR)
              VKB_ENUM_MACRO(ErrorInvalidExternalHandleKHR)
              VKB_ENUM_MACRO(ErrorFragmentationEXT)
              VKB_ENUM_MACRO(ErrorInvalidDeviceAddressEXT)
              VKB_ENUM_MACRO(ErrorInvalidOpaqueCaptureAddressKHR)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SampleCountFlagBits &p)
  {
      using enum_c_type = SampleCountFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(1)
              VKB_ENUM_MACRO(2)
              VKB_ENUM_MACRO(4)
              VKB_ENUM_MACRO(8)
              VKB_ENUM_MACRO(16)
              VKB_ENUM_MACRO(32)
              VKB_ENUM_MACRO(64)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SamplerAddressMode &p)
  {
      using enum_c_type = SamplerAddressMode;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Repeat)
              VKB_ENUM_MACRO(MirroredRepeat)
              VKB_ENUM_MACRO(ClampToEdge)
              VKB_ENUM_MACRO(ClampToBorder)
              VKB_ENUM_MACRO(MirrorClampToEdge)
              VKB_ENUM_MACRO(MirrorClampToEdgeKHR)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SamplerCreateFlagBits &p)
  {
      using enum_c_type = SamplerCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(SubsampledEXT)
              VKB_ENUM_MACRO(SubsampledCoarseReconstructionEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SamplerMipmapMode &p)
  {
      using enum_c_type = SamplerMipmapMode;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Nearest)
              VKB_ENUM_MACRO(Linear)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SamplerReductionMode &p)
  {
      using enum_c_type = SamplerReductionMode;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(WeightedAverage)
              VKB_ENUM_MACRO(Min)
              VKB_ENUM_MACRO(Max)
          VKB_END_ENUM_MACRO
  }



  inline void from_json(nlohmann::json const & j, SamplerYcbcrModelConversion &p)
  {
      using enum_c_type = SamplerYcbcrModelConversion;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(RgbIdentity)
              VKB_ENUM_MACRO(YcbcrIdentity)
              VKB_ENUM_MACRO(Ycbcr709)
              VKB_ENUM_MACRO(Ycbcr601)
              VKB_ENUM_MACRO(Ycbcr2020)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SamplerYcbcrRange &p)
  {
      using enum_c_type = SamplerYcbcrRange;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(ItuFull)
              VKB_ENUM_MACRO(ItuNarrow)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ScopeNV &p)
  {
      using enum_c_type = ScopeNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Device)
              VKB_ENUM_MACRO(Workgroup)
              VKB_ENUM_MACRO(Subgroup)
              VKB_ENUM_MACRO(QueueFamily)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SemaphoreCreateFlagBits &p)
  {
      using enum_c_type = SemaphoreCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SemaphoreImportFlagBits &p)
  {
      using enum_c_type = SemaphoreImportFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Temporary)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SemaphoreType &p)
  {
      using enum_c_type = SemaphoreType;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Binary)
              VKB_ENUM_MACRO(Timeline)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SemaphoreWaitFlagBits &p)
  {
      using enum_c_type = SemaphoreWaitFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Any)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ShaderCorePropertiesFlagBitsAMD &p)
  {
      using enum_c_type = ShaderCorePropertiesFlagBitsAMD;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ShaderFloatControlsIndependence &p)
  {
      using enum_c_type = ShaderFloatControlsIndependence;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(32BitOnly)
              VKB_ENUM_MACRO(All)
              VKB_ENUM_MACRO(None)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ShaderInfoTypeAMD &p)
  {
      using enum_c_type = ShaderInfoTypeAMD;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Statistics)
              VKB_ENUM_MACRO(Binary)
              VKB_ENUM_MACRO(Disassembly)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ShaderModuleCreateFlagBits &p)
  {
      using enum_c_type = ShaderModuleCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ShaderStageFlagBits &p)
  {
      using enum_c_type = ShaderStageFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Vertex)
              VKB_ENUM_MACRO(TessellationControl)
              VKB_ENUM_MACRO(TessellationEvaluation)
              VKB_ENUM_MACRO(Geometry)
              VKB_ENUM_MACRO(Fragment)
              VKB_ENUM_MACRO(Compute)
              VKB_ENUM_MACRO(AllGraphics)
              VKB_ENUM_MACRO(All)
              VKB_ENUM_MACRO(RaygenNV)
              VKB_ENUM_MACRO(AnyHitNV)
              VKB_ENUM_MACRO(ClosestHitNV)
              VKB_ENUM_MACRO(MissNV)
              VKB_ENUM_MACRO(IntersectionNV)
              VKB_ENUM_MACRO(CallableNV)
              VKB_ENUM_MACRO(TaskNV)
              VKB_ENUM_MACRO(MeshNV)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ShadingRatePaletteEntryNV &p)
  {
      using enum_c_type = ShadingRatePaletteEntryNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(NoInvocations)
              VKB_ENUM_MACRO(16InvocationsPerPixel)
              VKB_ENUM_MACRO(8InvocationsPerPixel)
              VKB_ENUM_MACRO(4InvocationsPerPixel)
              VKB_ENUM_MACRO(2InvocationsPerPixel)
              VKB_ENUM_MACRO(1InvocationPerPixel)
              VKB_ENUM_MACRO(1InvocationPer2X1Pixels)
              VKB_ENUM_MACRO(1InvocationPer1X2Pixels)
              VKB_ENUM_MACRO(1InvocationPer2X2Pixels)
              VKB_ENUM_MACRO(1InvocationPer4X2Pixels)
              VKB_ENUM_MACRO(1InvocationPer2X4Pixels)
              VKB_ENUM_MACRO(1InvocationPer4X4Pixels)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SharingMode &p)
  {
      using enum_c_type = SharingMode;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Exclusive)
              VKB_ENUM_MACRO(Concurrent)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SparseImageFormatFlagBits &p)
  {
      using enum_c_type = SparseImageFormatFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(SingleMiptail)
              VKB_ENUM_MACRO(AlignedMipSize)
              VKB_ENUM_MACRO(NonstandardBlockSize)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SparseMemoryBindFlagBits &p)
  {
      using enum_c_type = SparseMemoryBindFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Metadata)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, StencilFaceFlagBits &p)
  {
      using enum_c_type = StencilFaceFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Front)
              VKB_ENUM_MACRO(Back)
              VKB_ENUM_MACRO(FrontAndBack)
              VKB_ENUM_MACRO(VkStencilFrontAndBack)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, StencilOp &p)
  {
      using enum_c_type = StencilOp;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Keep)
              VKB_ENUM_MACRO(Zero)
              VKB_ENUM_MACRO(Replace)
              VKB_ENUM_MACRO(IncrementAndClamp)
              VKB_ENUM_MACRO(DecrementAndClamp)
              VKB_ENUM_MACRO(Invert)
              VKB_ENUM_MACRO(IncrementAndWrap)
              VKB_ENUM_MACRO(DecrementAndWrap)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, StructureType &p)
  {
      using enum_c_type = StructureType;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(ApplicationInfo)
              VKB_ENUM_MACRO(InstanceCreateInfo)
              VKB_ENUM_MACRO(DeviceQueueCreateInfo)
              VKB_ENUM_MACRO(DeviceCreateInfo)
              VKB_ENUM_MACRO(SubmitInfo)
              VKB_ENUM_MACRO(MemoryAllocateInfo)
              VKB_ENUM_MACRO(MappedMemoryRange)
              VKB_ENUM_MACRO(BindSparseInfo)
              VKB_ENUM_MACRO(FenceCreateInfo)
              VKB_ENUM_MACRO(SemaphoreCreateInfo)
              VKB_ENUM_MACRO(EventCreateInfo)
              VKB_ENUM_MACRO(QueryPoolCreateInfo)
              VKB_ENUM_MACRO(BufferCreateInfo)
              VKB_ENUM_MACRO(BufferViewCreateInfo)
              VKB_ENUM_MACRO(ImageCreateInfo)
              VKB_ENUM_MACRO(ImageViewCreateInfo)
              VKB_ENUM_MACRO(ShaderModuleCreateInfo)
              VKB_ENUM_MACRO(PipelineCacheCreateInfo)
              VKB_ENUM_MACRO(PipelineShaderStageCreateInfo)
              VKB_ENUM_MACRO(PipelineVertexInputStateCreateInfo)
              VKB_ENUM_MACRO(PipelineInputAssemblyStateCreateInfo)
              VKB_ENUM_MACRO(PipelineTessellationStateCreateInfo)
              VKB_ENUM_MACRO(PipelineViewportStateCreateInfo)
              VKB_ENUM_MACRO(PipelineRasterizationStateCreateInfo)
              VKB_ENUM_MACRO(PipelineMultisampleStateCreateInfo)
              VKB_ENUM_MACRO(PipelineDepthStencilStateCreateInfo)
              VKB_ENUM_MACRO(PipelineColorBlendStateCreateInfo)
              VKB_ENUM_MACRO(PipelineDynamicStateCreateInfo)
              VKB_ENUM_MACRO(GraphicsPipelineCreateInfo)
              VKB_ENUM_MACRO(ComputePipelineCreateInfo)
              VKB_ENUM_MACRO(PipelineLayoutCreateInfo)
              VKB_ENUM_MACRO(SamplerCreateInfo)
              VKB_ENUM_MACRO(DescriptorSetLayoutCreateInfo)
              VKB_ENUM_MACRO(DescriptorPoolCreateInfo)
              VKB_ENUM_MACRO(DescriptorSetAllocateInfo)
              VKB_ENUM_MACRO(WriteDescriptorSet)
              VKB_ENUM_MACRO(CopyDescriptorSet)
              VKB_ENUM_MACRO(FramebufferCreateInfo)
              VKB_ENUM_MACRO(RenderPassCreateInfo)
              VKB_ENUM_MACRO(CommandPoolCreateInfo)
              VKB_ENUM_MACRO(CommandBufferAllocateInfo)
              VKB_ENUM_MACRO(CommandBufferInheritanceInfo)
              VKB_ENUM_MACRO(CommandBufferBeginInfo)
              VKB_ENUM_MACRO(RenderPassBeginInfo)
              VKB_ENUM_MACRO(BufferMemoryBarrier)
              VKB_ENUM_MACRO(ImageMemoryBarrier)
              VKB_ENUM_MACRO(MemoryBarrier)
              VKB_ENUM_MACRO(LoaderInstanceCreateInfo)
              VKB_ENUM_MACRO(LoaderDeviceCreateInfo)
              VKB_ENUM_MACRO(PhysicalDeviceSubgroupProperties)
              VKB_ENUM_MACRO(BindBufferMemoryInfo)
              VKB_ENUM_MACRO(BindImageMemoryInfo)
              VKB_ENUM_MACRO(PhysicalDevice16BitStorageFeatures)
              VKB_ENUM_MACRO(MemoryDedicatedRequirements)
              VKB_ENUM_MACRO(MemoryDedicatedAllocateInfo)
              VKB_ENUM_MACRO(MemoryAllocateFlagsInfo)
              VKB_ENUM_MACRO(DeviceGroupRenderPassBeginInfo)
              VKB_ENUM_MACRO(DeviceGroupCommandBufferBeginInfo)
              VKB_ENUM_MACRO(DeviceGroupSubmitInfo)
              VKB_ENUM_MACRO(DeviceGroupBindSparseInfo)
              VKB_ENUM_MACRO(BindBufferMemoryDeviceGroupInfo)
              VKB_ENUM_MACRO(BindImageMemoryDeviceGroupInfo)
              VKB_ENUM_MACRO(PhysicalDeviceGroupProperties)
              VKB_ENUM_MACRO(DeviceGroupDeviceCreateInfo)
              VKB_ENUM_MACRO(BufferMemoryRequirementsInfo2)
              VKB_ENUM_MACRO(ImageMemoryRequirementsInfo2)
              VKB_ENUM_MACRO(ImageSparseMemoryRequirementsInfo2)
              VKB_ENUM_MACRO(MemoryRequirements2)
              VKB_ENUM_MACRO(SparseImageMemoryRequirements2)
              VKB_ENUM_MACRO(PhysicalDeviceFeatures2)
              VKB_ENUM_MACRO(PhysicalDeviceProperties2)
              VKB_ENUM_MACRO(FormatProperties2)
              VKB_ENUM_MACRO(ImageFormatProperties2)
              VKB_ENUM_MACRO(PhysicalDeviceImageFormatInfo2)
              VKB_ENUM_MACRO(QueueFamilyProperties2)
              VKB_ENUM_MACRO(PhysicalDeviceMemoryProperties2)
              VKB_ENUM_MACRO(SparseImageFormatProperties2)
              VKB_ENUM_MACRO(PhysicalDeviceSparseImageFormatInfo2)
              VKB_ENUM_MACRO(PhysicalDevicePointClippingProperties)
              VKB_ENUM_MACRO(RenderPassInputAttachmentAspectCreateInfo)
              VKB_ENUM_MACRO(ImageViewUsageCreateInfo)
              VKB_ENUM_MACRO(PipelineTessellationDomainOriginStateCreateInfo)
              VKB_ENUM_MACRO(RenderPassMultiviewCreateInfo)
              VKB_ENUM_MACRO(PhysicalDeviceMultiviewFeatures)
              VKB_ENUM_MACRO(PhysicalDeviceMultiviewProperties)
              VKB_ENUM_MACRO(PhysicalDeviceVariablePointersFeatures)
              VKB_ENUM_MACRO(ProtectedSubmitInfo)
              VKB_ENUM_MACRO(PhysicalDeviceProtectedMemoryFeatures)
              VKB_ENUM_MACRO(PhysicalDeviceProtectedMemoryProperties)
              VKB_ENUM_MACRO(DeviceQueueInfo2)
              VKB_ENUM_MACRO(SamplerYcbcrConversionCreateInfo)
              VKB_ENUM_MACRO(SamplerYcbcrConversionInfo)
              VKB_ENUM_MACRO(BindImagePlaneMemoryInfo)
              VKB_ENUM_MACRO(ImagePlaneMemoryRequirementsInfo)
              VKB_ENUM_MACRO(PhysicalDeviceSamplerYcbcrConversionFeatures)
              VKB_ENUM_MACRO(SamplerYcbcrConversionImageFormatProperties)
              VKB_ENUM_MACRO(DescriptorUpdateTemplateCreateInfo)
              VKB_ENUM_MACRO(PhysicalDeviceExternalImageFormatInfo)
              VKB_ENUM_MACRO(ExternalImageFormatProperties)
              VKB_ENUM_MACRO(PhysicalDeviceExternalBufferInfo)
              VKB_ENUM_MACRO(ExternalBufferProperties)
              VKB_ENUM_MACRO(PhysicalDeviceIdProperties)
              VKB_ENUM_MACRO(ExternalMemoryBufferCreateInfo)
              VKB_ENUM_MACRO(ExternalMemoryImageCreateInfo)
              VKB_ENUM_MACRO(ExportMemoryAllocateInfo)
              VKB_ENUM_MACRO(PhysicalDeviceExternalFenceInfo)
              VKB_ENUM_MACRO(ExternalFenceProperties)
              VKB_ENUM_MACRO(ExportFenceCreateInfo)
              VKB_ENUM_MACRO(ExportSemaphoreCreateInfo)
              VKB_ENUM_MACRO(PhysicalDeviceExternalSemaphoreInfo)
              VKB_ENUM_MACRO(ExternalSemaphoreProperties)
              VKB_ENUM_MACRO(PhysicalDeviceMaintenance3Properties)
              VKB_ENUM_MACRO(DescriptorSetLayoutSupport)
              VKB_ENUM_MACRO(PhysicalDeviceShaderDrawParametersFeatures)
              VKB_ENUM_MACRO(PhysicalDeviceVulkan11Features)
              VKB_ENUM_MACRO(PhysicalDeviceVulkan11Properties)
              VKB_ENUM_MACRO(PhysicalDeviceVulkan12Features)
              VKB_ENUM_MACRO(PhysicalDeviceVulkan12Properties)
              VKB_ENUM_MACRO(ImageFormatListCreateInfo)
              VKB_ENUM_MACRO(AttachmentDescription2)
              VKB_ENUM_MACRO(AttachmentReference2)
              VKB_ENUM_MACRO(SubpassDescription2)
              VKB_ENUM_MACRO(SubpassDependency2)
              VKB_ENUM_MACRO(RenderPassCreateInfo2)
              VKB_ENUM_MACRO(SubpassBeginInfo)
              VKB_ENUM_MACRO(SubpassEndInfo)
              VKB_ENUM_MACRO(PhysicalDevice8BitStorageFeatures)
              VKB_ENUM_MACRO(PhysicalDeviceDriverProperties)
              VKB_ENUM_MACRO(PhysicalDeviceShaderAtomicInt64Features)
              VKB_ENUM_MACRO(PhysicalDeviceShaderFloat16Int8Features)
              VKB_ENUM_MACRO(PhysicalDeviceFloatControlsProperties)
              VKB_ENUM_MACRO(DescriptorSetLayoutBindingFlagsCreateInfo)
              VKB_ENUM_MACRO(PhysicalDeviceDescriptorIndexingFeatures)
              VKB_ENUM_MACRO(PhysicalDeviceDescriptorIndexingProperties)
              VKB_ENUM_MACRO(DescriptorSetVariableDescriptorCountAllocateInfo)
              VKB_ENUM_MACRO(DescriptorSetVariableDescriptorCountLayoutSupport)
              VKB_ENUM_MACRO(PhysicalDeviceDepthStencilResolveProperties)
              VKB_ENUM_MACRO(SubpassDescriptionDepthStencilResolve)
              VKB_ENUM_MACRO(PhysicalDeviceScalarBlockLayoutFeatures)
              VKB_ENUM_MACRO(ImageStencilUsageCreateInfo)
              VKB_ENUM_MACRO(PhysicalDeviceSamplerFilterMinmaxProperties)
              VKB_ENUM_MACRO(SamplerReductionModeCreateInfo)
              VKB_ENUM_MACRO(PhysicalDeviceVulkanMemoryModelFeatures)
              VKB_ENUM_MACRO(PhysicalDeviceImagelessFramebufferFeatures)
              VKB_ENUM_MACRO(FramebufferAttachmentsCreateInfo)
              VKB_ENUM_MACRO(FramebufferAttachmentImageInfo)
              VKB_ENUM_MACRO(RenderPassAttachmentBeginInfo)
              VKB_ENUM_MACRO(PhysicalDeviceUniformBufferStandardLayoutFeatures)
              VKB_ENUM_MACRO(PhysicalDeviceShaderSubgroupExtendedTypesFeatures)
              VKB_ENUM_MACRO(PhysicalDeviceSeparateDepthStencilLayoutsFeatures)
              VKB_ENUM_MACRO(AttachmentReferenceStencilLayout)
              VKB_ENUM_MACRO(AttachmentDescriptionStencilLayout)
              VKB_ENUM_MACRO(PhysicalDeviceHostQueryResetFeatures)
              VKB_ENUM_MACRO(PhysicalDeviceTimelineSemaphoreFeatures)
              VKB_ENUM_MACRO(PhysicalDeviceTimelineSemaphoreProperties)
              VKB_ENUM_MACRO(SemaphoreTypeCreateInfo)
              VKB_ENUM_MACRO(TimelineSemaphoreSubmitInfo)
              VKB_ENUM_MACRO(SemaphoreWaitInfo)
              VKB_ENUM_MACRO(SemaphoreSignalInfo)
              VKB_ENUM_MACRO(PhysicalDeviceBufferDeviceAddressFeatures)
              VKB_ENUM_MACRO(BufferDeviceAddressInfo)
              VKB_ENUM_MACRO(BufferOpaqueCaptureAddressCreateInfo)
              VKB_ENUM_MACRO(MemoryOpaqueCaptureAddressAllocateInfo)
              VKB_ENUM_MACRO(DeviceMemoryOpaqueCaptureAddressInfo)
              VKB_ENUM_MACRO(SwapchainCreateInfoKHR)
              VKB_ENUM_MACRO(PresentInfoKHR)
              VKB_ENUM_MACRO(DeviceGroupPresentCapabilitiesKHR)
              VKB_ENUM_MACRO(ImageSwapchainCreateInfoKHR)
              VKB_ENUM_MACRO(BindImageMemorySwapchainInfoKHR)
              VKB_ENUM_MACRO(AcquireNextImageInfoKHR)
              VKB_ENUM_MACRO(DeviceGroupPresentInfoKHR)
              VKB_ENUM_MACRO(DeviceGroupSwapchainCreateInfoKHR)
              VKB_ENUM_MACRO(DisplayModeCreateInfoKHR)
              VKB_ENUM_MACRO(DisplaySurfaceCreateInfoKHR)
              VKB_ENUM_MACRO(DisplayPresentInfoKHR)
              VKB_ENUM_MACRO(XlibSurfaceCreateInfoKHR)
              VKB_ENUM_MACRO(XcbSurfaceCreateInfoKHR)
              VKB_ENUM_MACRO(WaylandSurfaceCreateInfoKHR)
              VKB_ENUM_MACRO(AndroidSurfaceCreateInfoKHR)
              VKB_ENUM_MACRO(Win32SurfaceCreateInfoKHR)
              VKB_ENUM_MACRO(DebugReportCallbackCreateInfoEXT)
              VKB_ENUM_MACRO(PipelineRasterizationStateRasterizationOrderAMD)
              VKB_ENUM_MACRO(DebugMarkerObjectNameInfoEXT)
              VKB_ENUM_MACRO(DebugMarkerObjectTagInfoEXT)
              VKB_ENUM_MACRO(DebugMarkerMarkerInfoEXT)
              VKB_ENUM_MACRO(DedicatedAllocationImageCreateInfoNV)
              VKB_ENUM_MACRO(DedicatedAllocationBufferCreateInfoNV)
              VKB_ENUM_MACRO(DedicatedAllocationMemoryAllocateInfoNV)
              VKB_ENUM_MACRO(PhysicalDeviceTransformFeedbackFeaturesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceTransformFeedbackPropertiesEXT)
              VKB_ENUM_MACRO(PipelineRasterizationStateStreamCreateInfoEXT)
              VKB_ENUM_MACRO(ImageViewHandleInfoNVX)
              VKB_ENUM_MACRO(TextureLodGatherFormatPropertiesAMD)
              VKB_ENUM_MACRO(StreamDescriptorSurfaceCreateInfoGGP)
              VKB_ENUM_MACRO(PhysicalDeviceCornerSampledImageFeaturesNV)
              VKB_ENUM_MACRO(ExternalMemoryImageCreateInfoNV)
              VKB_ENUM_MACRO(ExportMemoryAllocateInfoNV)
              VKB_ENUM_MACRO(ImportMemoryWin32HandleInfoNV)
              VKB_ENUM_MACRO(ExportMemoryWin32HandleInfoNV)
              VKB_ENUM_MACRO(Win32KeyedMutexAcquireReleaseInfoNV)
              VKB_ENUM_MACRO(ValidationFlagsEXT)
              VKB_ENUM_MACRO(ViSurfaceCreateInfoNN)
              VKB_ENUM_MACRO(PhysicalDeviceTextureCompressionAstcHdrFeaturesEXT)
              VKB_ENUM_MACRO(ImageViewAstcDecodeModeEXT)
              VKB_ENUM_MACRO(PhysicalDeviceAstcDecodeFeaturesEXT)
              VKB_ENUM_MACRO(ImportMemoryWin32HandleInfoKHR)
              VKB_ENUM_MACRO(ExportMemoryWin32HandleInfoKHR)
              VKB_ENUM_MACRO(MemoryWin32HandlePropertiesKHR)
              VKB_ENUM_MACRO(MemoryGetWin32HandleInfoKHR)
              VKB_ENUM_MACRO(ImportMemoryFdInfoKHR)
              VKB_ENUM_MACRO(MemoryFdPropertiesKHR)
              VKB_ENUM_MACRO(MemoryGetFdInfoKHR)
              VKB_ENUM_MACRO(Win32KeyedMutexAcquireReleaseInfoKHR)
              VKB_ENUM_MACRO(ImportSemaphoreWin32HandleInfoKHR)
              VKB_ENUM_MACRO(ExportSemaphoreWin32HandleInfoKHR)
              VKB_ENUM_MACRO(D3D12FenceSubmitInfoKHR)
              VKB_ENUM_MACRO(SemaphoreGetWin32HandleInfoKHR)
              VKB_ENUM_MACRO(ImportSemaphoreFdInfoKHR)
              VKB_ENUM_MACRO(SemaphoreGetFdInfoKHR)
              VKB_ENUM_MACRO(PhysicalDevicePushDescriptorPropertiesKHR)
              VKB_ENUM_MACRO(CommandBufferInheritanceConditionalRenderingInfoEXT)
              VKB_ENUM_MACRO(PhysicalDeviceConditionalRenderingFeaturesEXT)
              VKB_ENUM_MACRO(ConditionalRenderingBeginInfoEXT)
              VKB_ENUM_MACRO(PresentRegionsKHR)
              VKB_ENUM_MACRO(ObjectTableCreateInfoNVX)
              VKB_ENUM_MACRO(IndirectCommandsLayoutCreateInfoNVX)
              VKB_ENUM_MACRO(CmdProcessCommandsInfoNVX)
              VKB_ENUM_MACRO(CmdReserveSpaceForCommandsInfoNVX)
              VKB_ENUM_MACRO(DeviceGeneratedCommandsLimitsNVX)
              VKB_ENUM_MACRO(DeviceGeneratedCommandsFeaturesNVX)
              VKB_ENUM_MACRO(PipelineViewportWScalingStateCreateInfoNV)
              VKB_ENUM_MACRO(SurfaceCapabilities2EXT)
              VKB_ENUM_MACRO(DisplayPowerInfoEXT)
              VKB_ENUM_MACRO(DeviceEventInfoEXT)
              VKB_ENUM_MACRO(DisplayEventInfoEXT)
              VKB_ENUM_MACRO(SwapchainCounterCreateInfoEXT)
              VKB_ENUM_MACRO(PresentTimesInfoGOOGLE)
              VKB_ENUM_MACRO(PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX)
              VKB_ENUM_MACRO(PipelineViewportSwizzleStateCreateInfoNV)
              VKB_ENUM_MACRO(PhysicalDeviceDiscardRectanglePropertiesEXT)
              VKB_ENUM_MACRO(PipelineDiscardRectangleStateCreateInfoEXT)
              VKB_ENUM_MACRO(PhysicalDeviceConservativeRasterizationPropertiesEXT)
              VKB_ENUM_MACRO(PipelineRasterizationConservativeStateCreateInfoEXT)
              VKB_ENUM_MACRO(PhysicalDeviceDepthClipEnableFeaturesEXT)
              VKB_ENUM_MACRO(PipelineRasterizationDepthClipStateCreateInfoEXT)
              VKB_ENUM_MACRO(HdrMetadataEXT)
              VKB_ENUM_MACRO(SharedPresentSurfaceCapabilitiesKHR)
              VKB_ENUM_MACRO(ImportFenceWin32HandleInfoKHR)
              VKB_ENUM_MACRO(ExportFenceWin32HandleInfoKHR)
              VKB_ENUM_MACRO(FenceGetWin32HandleInfoKHR)
              VKB_ENUM_MACRO(ImportFenceFdInfoKHR)
              VKB_ENUM_MACRO(FenceGetFdInfoKHR)
              VKB_ENUM_MACRO(PhysicalDevicePerformanceQueryFeaturesKHR)
              VKB_ENUM_MACRO(PhysicalDevicePerformanceQueryPropertiesKHR)
              VKB_ENUM_MACRO(QueryPoolPerformanceCreateInfoKHR)
              VKB_ENUM_MACRO(PerformanceQuerySubmitInfoKHR)
              VKB_ENUM_MACRO(AcquireProfilingLockInfoKHR)
              VKB_ENUM_MACRO(PerformanceCounterKHR)
              VKB_ENUM_MACRO(PerformanceCounterDescriptionKHR)
              VKB_ENUM_MACRO(PhysicalDeviceSurfaceInfo2KHR)
              VKB_ENUM_MACRO(SurfaceCapabilities2KHR)
              VKB_ENUM_MACRO(SurfaceFormat2KHR)
              VKB_ENUM_MACRO(DisplayProperties2KHR)
              VKB_ENUM_MACRO(DisplayPlaneProperties2KHR)
              VKB_ENUM_MACRO(DisplayModeProperties2KHR)
              VKB_ENUM_MACRO(DisplayPlaneInfo2KHR)
              VKB_ENUM_MACRO(DisplayPlaneCapabilities2KHR)
              VKB_ENUM_MACRO(IosSurfaceCreateInfoMVK)
              VKB_ENUM_MACRO(MacosSurfaceCreateInfoMVK)
              VKB_ENUM_MACRO(DebugUtilsObjectNameInfoEXT)
              VKB_ENUM_MACRO(DebugUtilsObjectTagInfoEXT)
              VKB_ENUM_MACRO(DebugUtilsLabelEXT)
              VKB_ENUM_MACRO(DebugUtilsMessengerCallbackDataEXT)
              VKB_ENUM_MACRO(DebugUtilsMessengerCreateInfoEXT)
              VKB_ENUM_MACRO(AndroidHardwareBufferUsageANDROID)
              VKB_ENUM_MACRO(AndroidHardwareBufferPropertiesANDROID)
              VKB_ENUM_MACRO(AndroidHardwareBufferFormatPropertiesANDROID)
              VKB_ENUM_MACRO(ImportAndroidHardwareBufferInfoANDROID)
              VKB_ENUM_MACRO(MemoryGetAndroidHardwareBufferInfoANDROID)
              VKB_ENUM_MACRO(ExternalFormatANDROID)
              VKB_ENUM_MACRO(PhysicalDeviceInlineUniformBlockFeaturesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceInlineUniformBlockPropertiesEXT)
              VKB_ENUM_MACRO(WriteDescriptorSetInlineUniformBlockEXT)
              VKB_ENUM_MACRO(DescriptorPoolInlineUniformBlockCreateInfoEXT)
              VKB_ENUM_MACRO(SampleLocationsInfoEXT)
              VKB_ENUM_MACRO(RenderPassSampleLocationsBeginInfoEXT)
              VKB_ENUM_MACRO(PipelineSampleLocationsStateCreateInfoEXT)
              VKB_ENUM_MACRO(PhysicalDeviceSampleLocationsPropertiesEXT)
              VKB_ENUM_MACRO(MultisamplePropertiesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceBlendOperationAdvancedFeaturesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceBlendOperationAdvancedPropertiesEXT)
              VKB_ENUM_MACRO(PipelineColorBlendAdvancedStateCreateInfoEXT)
              VKB_ENUM_MACRO(PipelineCoverageToColorStateCreateInfoNV)
              VKB_ENUM_MACRO(PipelineCoverageModulationStateCreateInfoNV)
              VKB_ENUM_MACRO(PhysicalDeviceShaderSmBuiltinsFeaturesNV)
              VKB_ENUM_MACRO(PhysicalDeviceShaderSmBuiltinsPropertiesNV)
              VKB_ENUM_MACRO(DrmFormatModifierPropertiesListEXT)
              VKB_ENUM_MACRO(DrmFormatModifierPropertiesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceImageDrmFormatModifierInfoEXT)
              VKB_ENUM_MACRO(ImageDrmFormatModifierListCreateInfoEXT)
              VKB_ENUM_MACRO(ImageDrmFormatModifierExplicitCreateInfoEXT)
              VKB_ENUM_MACRO(ImageDrmFormatModifierPropertiesEXT)
              VKB_ENUM_MACRO(ValidationCacheCreateInfoEXT)
              VKB_ENUM_MACRO(ShaderModuleValidationCacheCreateInfoEXT)
              VKB_ENUM_MACRO(PipelineViewportShadingRateImageStateCreateInfoNV)
              VKB_ENUM_MACRO(PhysicalDeviceShadingRateImageFeaturesNV)
              VKB_ENUM_MACRO(PhysicalDeviceShadingRateImagePropertiesNV)
              VKB_ENUM_MACRO(PipelineViewportCoarseSampleOrderStateCreateInfoNV)
              VKB_ENUM_MACRO(RayTracingPipelineCreateInfoNV)
              VKB_ENUM_MACRO(AccelerationStructureCreateInfoNV)
              VKB_ENUM_MACRO(GeometryNV)
              VKB_ENUM_MACRO(GeometryTrianglesNV)
              VKB_ENUM_MACRO(GeometryAabbNV)
              VKB_ENUM_MACRO(BindAccelerationStructureMemoryInfoNV)
              VKB_ENUM_MACRO(WriteDescriptorSetAccelerationStructureNV)
              VKB_ENUM_MACRO(AccelerationStructureMemoryRequirementsInfoNV)
              VKB_ENUM_MACRO(PhysicalDeviceRayTracingPropertiesNV)
              VKB_ENUM_MACRO(RayTracingShaderGroupCreateInfoNV)
              VKB_ENUM_MACRO(AccelerationStructureInfoNV)
              VKB_ENUM_MACRO(PhysicalDeviceRepresentativeFragmentTestFeaturesNV)
              VKB_ENUM_MACRO(PipelineRepresentativeFragmentTestStateCreateInfoNV)
              VKB_ENUM_MACRO(PhysicalDeviceImageViewImageFormatInfoEXT)
              VKB_ENUM_MACRO(FilterCubicImageViewImageFormatPropertiesEXT)
              VKB_ENUM_MACRO(DeviceQueueGlobalPriorityCreateInfoEXT)
              VKB_ENUM_MACRO(ImportMemoryHostPointerInfoEXT)
              VKB_ENUM_MACRO(MemoryHostPointerPropertiesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceExternalMemoryHostPropertiesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceShaderClockFeaturesKHR)
              VKB_ENUM_MACRO(PipelineCompilerControlCreateInfoAMD)
              VKB_ENUM_MACRO(CalibratedTimestampInfoEXT)
              VKB_ENUM_MACRO(PhysicalDeviceShaderCorePropertiesAMD)
              VKB_ENUM_MACRO(DeviceMemoryOverallocationCreateInfoAMD)
              VKB_ENUM_MACRO(PhysicalDeviceVertexAttributeDivisorPropertiesEXT)
              VKB_ENUM_MACRO(PipelineVertexInputDivisorStateCreateInfoEXT)
              VKB_ENUM_MACRO(PhysicalDeviceVertexAttributeDivisorFeaturesEXT)
              VKB_ENUM_MACRO(PresentFrameTokenGGP)
              VKB_ENUM_MACRO(PipelineCreationFeedbackCreateInfoEXT)
              VKB_ENUM_MACRO(PhysicalDeviceComputeShaderDerivativesFeaturesNV)
              VKB_ENUM_MACRO(PhysicalDeviceMeshShaderFeaturesNV)
              VKB_ENUM_MACRO(PhysicalDeviceMeshShaderPropertiesNV)
              VKB_ENUM_MACRO(PhysicalDeviceFragmentShaderBarycentricFeaturesNV)
              VKB_ENUM_MACRO(PhysicalDeviceShaderImageFootprintFeaturesNV)
              VKB_ENUM_MACRO(PipelineViewportExclusiveScissorStateCreateInfoNV)
              VKB_ENUM_MACRO(PhysicalDeviceExclusiveScissorFeaturesNV)
              VKB_ENUM_MACRO(CheckpointDataNV)
              VKB_ENUM_MACRO(QueueFamilyCheckpointPropertiesNV)
              VKB_ENUM_MACRO(PhysicalDeviceShaderIntegerFunctions2FeaturesINTEL)
              VKB_ENUM_MACRO(QueryPoolCreateInfoINTEL)
              VKB_ENUM_MACRO(InitializePerformanceApiInfoINTEL)
              VKB_ENUM_MACRO(PerformanceMarkerInfoINTEL)
              VKB_ENUM_MACRO(PerformanceStreamMarkerInfoINTEL)
              VKB_ENUM_MACRO(PerformanceOverrideInfoINTEL)
              VKB_ENUM_MACRO(PerformanceConfigurationAcquireInfoINTEL)
              VKB_ENUM_MACRO(PhysicalDevicePciBusInfoPropertiesEXT)
              VKB_ENUM_MACRO(DisplayNativeHdrSurfaceCapabilitiesAMD)
              VKB_ENUM_MACRO(SwapchainDisplayNativeHdrCreateInfoAMD)
              VKB_ENUM_MACRO(ImagepipeSurfaceCreateInfoFUCHSIA)
              VKB_ENUM_MACRO(MetalSurfaceCreateInfoEXT)
              VKB_ENUM_MACRO(PhysicalDeviceFragmentDensityMapFeaturesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceFragmentDensityMapPropertiesEXT)
              VKB_ENUM_MACRO(RenderPassFragmentDensityMapCreateInfoEXT)
              VKB_ENUM_MACRO(PhysicalDeviceSubgroupSizeControlPropertiesEXT)
              VKB_ENUM_MACRO(PipelineShaderStageRequiredSubgroupSizeCreateInfoEXT)
              VKB_ENUM_MACRO(PhysicalDeviceSubgroupSizeControlFeaturesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceShaderCoreProperties2AMD)
              VKB_ENUM_MACRO(PhysicalDeviceCoherentMemoryFeaturesAMD)
              VKB_ENUM_MACRO(PhysicalDeviceMemoryBudgetPropertiesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceMemoryPriorityFeaturesEXT)
              VKB_ENUM_MACRO(MemoryPriorityAllocateInfoEXT)
              VKB_ENUM_MACRO(SurfaceProtectedCapabilitiesKHR)
              VKB_ENUM_MACRO(PhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV)
              VKB_ENUM_MACRO(PhysicalDeviceBufferDeviceAddressFeaturesEXT)
              VKB_ENUM_MACRO(BufferDeviceAddressCreateInfoEXT)
              VKB_ENUM_MACRO(PhysicalDeviceToolPropertiesEXT)
              VKB_ENUM_MACRO(ValidationFeaturesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceCooperativeMatrixFeaturesNV)
              VKB_ENUM_MACRO(CooperativeMatrixPropertiesNV)
              VKB_ENUM_MACRO(PhysicalDeviceCooperativeMatrixPropertiesNV)
              VKB_ENUM_MACRO(PhysicalDeviceCoverageReductionModeFeaturesNV)
              VKB_ENUM_MACRO(PipelineCoverageReductionStateCreateInfoNV)
              VKB_ENUM_MACRO(FramebufferMixedSamplesCombinationNV)
              VKB_ENUM_MACRO(PhysicalDeviceFragmentShaderInterlockFeaturesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceYcbcrImageArraysFeaturesEXT)
              VKB_ENUM_MACRO(SurfaceFullScreenExclusiveInfoEXT)
              VKB_ENUM_MACRO(SurfaceCapabilitiesFullScreenExclusiveEXT)
              VKB_ENUM_MACRO(SurfaceFullScreenExclusiveWin32InfoEXT)
              VKB_ENUM_MACRO(HeadlessSurfaceCreateInfoEXT)
              VKB_ENUM_MACRO(PhysicalDeviceLineRasterizationFeaturesEXT)
              VKB_ENUM_MACRO(PipelineRasterizationLineStateCreateInfoEXT)
              VKB_ENUM_MACRO(PhysicalDeviceLineRasterizationPropertiesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceIndexTypeUint8FeaturesEXT)
              VKB_ENUM_MACRO(PhysicalDevicePipelineExecutablePropertiesFeaturesKHR)
              VKB_ENUM_MACRO(PipelineInfoKHR)
              VKB_ENUM_MACRO(PipelineExecutablePropertiesKHR)
              VKB_ENUM_MACRO(PipelineExecutableInfoKHR)
              VKB_ENUM_MACRO(PipelineExecutableStatisticKHR)
              VKB_ENUM_MACRO(PipelineExecutableInternalRepresentationKHR)
              VKB_ENUM_MACRO(PhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceTexelBufferAlignmentFeaturesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceTexelBufferAlignmentPropertiesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceVariablePointerFeatures)
              VKB_ENUM_MACRO(PhysicalDeviceShaderDrawParameterFeatures)
              VKB_ENUM_MACRO(DebugReportCreateInfoEXT)
              VKB_ENUM_MACRO(RenderPassMultiviewCreateInfoKHR)
              VKB_ENUM_MACRO(PhysicalDeviceMultiviewFeaturesKHR)
              VKB_ENUM_MACRO(PhysicalDeviceMultiviewPropertiesKHR)
              VKB_ENUM_MACRO(PhysicalDeviceFeatures2KHR)
              VKB_ENUM_MACRO(PhysicalDeviceProperties2KHR)
              VKB_ENUM_MACRO(FormatProperties2KHR)
              VKB_ENUM_MACRO(ImageFormatProperties2KHR)
              VKB_ENUM_MACRO(PhysicalDeviceImageFormatInfo2KHR)
              VKB_ENUM_MACRO(QueueFamilyProperties2KHR)
              VKB_ENUM_MACRO(PhysicalDeviceMemoryProperties2KHR)
              VKB_ENUM_MACRO(SparseImageFormatProperties2KHR)
              VKB_ENUM_MACRO(PhysicalDeviceSparseImageFormatInfo2KHR)
              VKB_ENUM_MACRO(MemoryAllocateFlagsInfoKHR)
              VKB_ENUM_MACRO(DeviceGroupRenderPassBeginInfoKHR)
              VKB_ENUM_MACRO(DeviceGroupCommandBufferBeginInfoKHR)
              VKB_ENUM_MACRO(DeviceGroupSubmitInfoKHR)
              VKB_ENUM_MACRO(DeviceGroupBindSparseInfoKHR)
              VKB_ENUM_MACRO(BindBufferMemoryDeviceGroupInfoKHR)
              VKB_ENUM_MACRO(BindImageMemoryDeviceGroupInfoKHR)
              VKB_ENUM_MACRO(PhysicalDeviceGroupPropertiesKHR)
              VKB_ENUM_MACRO(DeviceGroupDeviceCreateInfoKHR)
              VKB_ENUM_MACRO(PhysicalDeviceExternalImageFormatInfoKHR)
              VKB_ENUM_MACRO(ExternalImageFormatPropertiesKHR)
              VKB_ENUM_MACRO(PhysicalDeviceExternalBufferInfoKHR)
              VKB_ENUM_MACRO(ExternalBufferPropertiesKHR)
              VKB_ENUM_MACRO(PhysicalDeviceIdPropertiesKHR)
              VKB_ENUM_MACRO(ExternalMemoryBufferCreateInfoKHR)
              VKB_ENUM_MACRO(ExternalMemoryImageCreateInfoKHR)
              VKB_ENUM_MACRO(ExportMemoryAllocateInfoKHR)
              VKB_ENUM_MACRO(PhysicalDeviceExternalSemaphoreInfoKHR)
              VKB_ENUM_MACRO(ExternalSemaphorePropertiesKHR)
              VKB_ENUM_MACRO(ExportSemaphoreCreateInfoKHR)
              VKB_ENUM_MACRO(PhysicalDeviceShaderFloat16Int8FeaturesKHR)
              VKB_ENUM_MACRO(PhysicalDeviceFloat16Int8FeaturesKHR)
              VKB_ENUM_MACRO(PhysicalDevice16BitStorageFeaturesKHR)
              VKB_ENUM_MACRO(DescriptorUpdateTemplateCreateInfoKHR)
              VKB_ENUM_MACRO(PhysicalDeviceImagelessFramebufferFeaturesKHR)
              VKB_ENUM_MACRO(FramebufferAttachmentsCreateInfoKHR)
              VKB_ENUM_MACRO(FramebufferAttachmentImageInfoKHR)
              VKB_ENUM_MACRO(RenderPassAttachmentBeginInfoKHR)
              VKB_ENUM_MACRO(AttachmentDescription2KHR)
              VKB_ENUM_MACRO(AttachmentReference2KHR)
              VKB_ENUM_MACRO(SubpassDescription2KHR)
              VKB_ENUM_MACRO(SubpassDependency2KHR)
              VKB_ENUM_MACRO(RenderPassCreateInfo2KHR)
              VKB_ENUM_MACRO(SubpassBeginInfoKHR)
              VKB_ENUM_MACRO(SubpassEndInfoKHR)
              VKB_ENUM_MACRO(PhysicalDeviceExternalFenceInfoKHR)
              VKB_ENUM_MACRO(ExternalFencePropertiesKHR)
              VKB_ENUM_MACRO(ExportFenceCreateInfoKHR)
              VKB_ENUM_MACRO(PhysicalDevicePointClippingPropertiesKHR)
              VKB_ENUM_MACRO(RenderPassInputAttachmentAspectCreateInfoKHR)
              VKB_ENUM_MACRO(ImageViewUsageCreateInfoKHR)
              VKB_ENUM_MACRO(PipelineTessellationDomainOriginStateCreateInfoKHR)
              VKB_ENUM_MACRO(PhysicalDeviceVariablePointerFeaturesKHR)
              VKB_ENUM_MACRO(PhysicalDeviceVariablePointersFeaturesKHR)
              VKB_ENUM_MACRO(MemoryDedicatedRequirementsKHR)
              VKB_ENUM_MACRO(MemoryDedicatedAllocateInfoKHR)
              VKB_ENUM_MACRO(PhysicalDeviceSamplerFilterMinmaxPropertiesEXT)
              VKB_ENUM_MACRO(SamplerReductionModeCreateInfoEXT)
              VKB_ENUM_MACRO(BufferMemoryRequirementsInfo2KHR)
              VKB_ENUM_MACRO(ImageMemoryRequirementsInfo2KHR)
              VKB_ENUM_MACRO(ImageSparseMemoryRequirementsInfo2KHR)
              VKB_ENUM_MACRO(MemoryRequirements2KHR)
              VKB_ENUM_MACRO(SparseImageMemoryRequirements2KHR)
              VKB_ENUM_MACRO(ImageFormatListCreateInfoKHR)
              VKB_ENUM_MACRO(SamplerYcbcrConversionCreateInfoKHR)
              VKB_ENUM_MACRO(SamplerYcbcrConversionInfoKHR)
              VKB_ENUM_MACRO(BindImagePlaneMemoryInfoKHR)
              VKB_ENUM_MACRO(ImagePlaneMemoryRequirementsInfoKHR)
              VKB_ENUM_MACRO(PhysicalDeviceSamplerYcbcrConversionFeaturesKHR)
              VKB_ENUM_MACRO(SamplerYcbcrConversionImageFormatPropertiesKHR)
              VKB_ENUM_MACRO(BindBufferMemoryInfoKHR)
              VKB_ENUM_MACRO(BindImageMemoryInfoKHR)
              VKB_ENUM_MACRO(DescriptorSetLayoutBindingFlagsCreateInfoEXT)
              VKB_ENUM_MACRO(PhysicalDeviceDescriptorIndexingFeaturesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceDescriptorIndexingPropertiesEXT)
              VKB_ENUM_MACRO(DescriptorSetVariableDescriptorCountAllocateInfoEXT)
              VKB_ENUM_MACRO(DescriptorSetVariableDescriptorCountLayoutSupportEXT)
              VKB_ENUM_MACRO(PhysicalDeviceMaintenance3PropertiesKHR)
              VKB_ENUM_MACRO(DescriptorSetLayoutSupportKHR)
              VKB_ENUM_MACRO(PhysicalDeviceShaderSubgroupExtendedTypesFeaturesKHR)
              VKB_ENUM_MACRO(PhysicalDevice8BitStorageFeaturesKHR)
              VKB_ENUM_MACRO(PhysicalDeviceShaderAtomicInt64FeaturesKHR)
              VKB_ENUM_MACRO(PhysicalDeviceDriverPropertiesKHR)
              VKB_ENUM_MACRO(PhysicalDeviceFloatControlsPropertiesKHR)
              VKB_ENUM_MACRO(PhysicalDeviceDepthStencilResolvePropertiesKHR)
              VKB_ENUM_MACRO(SubpassDescriptionDepthStencilResolveKHR)
              VKB_ENUM_MACRO(PhysicalDeviceTimelineSemaphoreFeaturesKHR)
              VKB_ENUM_MACRO(PhysicalDeviceTimelineSemaphorePropertiesKHR)
              VKB_ENUM_MACRO(SemaphoreTypeCreateInfoKHR)
              VKB_ENUM_MACRO(TimelineSemaphoreSubmitInfoKHR)
              VKB_ENUM_MACRO(SemaphoreWaitInfoKHR)
              VKB_ENUM_MACRO(SemaphoreSignalInfoKHR)
              VKB_ENUM_MACRO(PhysicalDeviceVulkanMemoryModelFeaturesKHR)
              VKB_ENUM_MACRO(PhysicalDeviceScalarBlockLayoutFeaturesEXT)
              VKB_ENUM_MACRO(PhysicalDeviceSeparateDepthStencilLayoutsFeaturesKHR)
              VKB_ENUM_MACRO(AttachmentReferenceStencilLayoutKHR)
              VKB_ENUM_MACRO(AttachmentDescriptionStencilLayoutKHR)
              VKB_ENUM_MACRO(PhysicalDeviceBufferAddressFeaturesEXT)
              VKB_ENUM_MACRO(BufferDeviceAddressInfoEXT)
              VKB_ENUM_MACRO(ImageStencilUsageCreateInfoEXT)
              VKB_ENUM_MACRO(PhysicalDeviceUniformBufferStandardLayoutFeaturesKHR)
              VKB_ENUM_MACRO(PhysicalDeviceBufferDeviceAddressFeaturesKHR)
              VKB_ENUM_MACRO(BufferDeviceAddressInfoKHR)
              VKB_ENUM_MACRO(BufferOpaqueCaptureAddressCreateInfoKHR)
              VKB_ENUM_MACRO(MemoryOpaqueCaptureAddressAllocateInfoKHR)
              VKB_ENUM_MACRO(DeviceMemoryOpaqueCaptureAddressInfoKHR)
              VKB_ENUM_MACRO(PhysicalDeviceHostQueryResetFeaturesEXT)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SubgroupFeatureFlagBits &p)
  {
      using enum_c_type = SubgroupFeatureFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Basic)
              VKB_ENUM_MACRO(Vote)
              VKB_ENUM_MACRO(Arithmetic)
              VKB_ENUM_MACRO(Ballot)
              VKB_ENUM_MACRO(Shuffle)
              VKB_ENUM_MACRO(ShuffleRelative)
              VKB_ENUM_MACRO(Clustered)
              VKB_ENUM_MACRO(Quad)
              VKB_ENUM_MACRO(PartitionedNV)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SubpassContents &p)
  {
      using enum_c_type = SubpassContents;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Inline)
              VKB_ENUM_MACRO(SecondaryCommandBuffers)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SubpassDescriptionFlagBits &p)
  {
      using enum_c_type = SubpassDescriptionFlagBits;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(PerViewAttributesNVX)
              VKB_ENUM_MACRO(PerViewPositionXOnlyNVX)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SurfaceCounterFlagBitsEXT &p)
  {
      using enum_c_type = SurfaceCounterFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Vblank)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SurfaceTransformFlagBitsKHR &p)
  {
      using enum_c_type = SurfaceTransformFlagBitsKHR;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Identity)
              VKB_ENUM_MACRO(Rotate90)
              VKB_ENUM_MACRO(Rotate180)
              VKB_ENUM_MACRO(Rotate270)
              VKB_ENUM_MACRO(HorizontalMirror)
              VKB_ENUM_MACRO(HorizontalMirrorRotate90)
              VKB_ENUM_MACRO(HorizontalMirrorRotate180)
              VKB_ENUM_MACRO(HorizontalMirrorRotate270)
              VKB_ENUM_MACRO(Inherit)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SwapchainCreateFlagBitsKHR &p)
  {
      using enum_c_type = SwapchainCreateFlagBitsKHR;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(SplitInstanceBindRegions)
              VKB_ENUM_MACRO(Protected)
              VKB_ENUM_MACRO(MutableFormat)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, SystemAllocationScope &p)
  {
      using enum_c_type = SystemAllocationScope;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Command)
              VKB_ENUM_MACRO(Object)
              VKB_ENUM_MACRO(Cache)
              VKB_ENUM_MACRO(Device)
              VKB_ENUM_MACRO(Instance)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, TessellationDomainOrigin &p)
  {
      using enum_c_type = TessellationDomainOrigin;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(UpperLeft)
              VKB_ENUM_MACRO(LowerLeft)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, TimeDomainEXT &p)
  {
      using enum_c_type = TimeDomainEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Device)
              VKB_ENUM_MACRO(ClockMonotonic)
              VKB_ENUM_MACRO(ClockMonotonicRaw)
              VKB_ENUM_MACRO(QueryPerformanceCounter)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ToolPurposeFlagBitsEXT &p)
  {
      using enum_c_type = ToolPurposeFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Validation)
              VKB_ENUM_MACRO(Profiling)
              VKB_ENUM_MACRO(Tracing)
              VKB_ENUM_MACRO(AdditionalFeatures)
              VKB_ENUM_MACRO(ModifyingFeatures)
              VKB_ENUM_MACRO(DebugReporting)
              VKB_ENUM_MACRO(DebugMarkers)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ValidationCacheHeaderVersionEXT &p)
  {
      using enum_c_type = ValidationCacheHeaderVersionEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(One)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ValidationCheckEXT &p)
  {
      using enum_c_type = ValidationCheckEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(All)
              VKB_ENUM_MACRO(Shaders)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ValidationFeatureDisableEXT &p)
  {
      using enum_c_type = ValidationFeatureDisableEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(All)
              VKB_ENUM_MACRO(Shaders)
              VKB_ENUM_MACRO(ThreadSafety)
              VKB_ENUM_MACRO(ApiParameters)
              VKB_ENUM_MACRO(ObjectLifetimes)
              VKB_ENUM_MACRO(CoreChecks)
              VKB_ENUM_MACRO(UniqueHandles)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ValidationFeatureEnableEXT &p)
  {
      using enum_c_type = ValidationFeatureEnableEXT;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(GpuAssisted)
              VKB_ENUM_MACRO(GpuAssistedReserveBindingSlot)
              VKB_ENUM_MACRO(BestPractices)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, VendorId &p)
  {
      using enum_c_type = VendorId;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(VIV)
              VKB_ENUM_MACRO(VSI)
              VKB_ENUM_MACRO(Kazan)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, VertexInputRate &p)
  {
      using enum_c_type = VertexInputRate;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(Vertex)
              VKB_ENUM_MACRO(Instance)
          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, ViewportCoordinateSwizzleNV &p)
  {
      using enum_c_type = ViewportCoordinateSwizzleNV;


          VKB_BEGIN_ENUM_MACRO


              VKB_ENUM_MACRO(PositiveX)
              VKB_ENUM_MACRO(NegativeX)
              VKB_ENUM_MACRO(PositiveY)
              VKB_ENUM_MACRO(NegativeY)
              VKB_ENUM_MACRO(PositiveZ)
              VKB_ENUM_MACRO(NegativeZ)
              VKB_ENUM_MACRO(PositiveW)
              VKB_ENUM_MACRO(NegativeW)
          VKB_END_ENUM_MACRO
  }
#ifdef VK_USE_PLATFORM_ANDROID_KHR
  inline void from_json(nlohmann::json const & j, AndroidSurfaceCreateFlagBitsKHR &p)
  {
      using enum_c_type = AndroidSurfaceCreateFlagBitsKHR;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }
#endif
  inline void from_json(nlohmann::json const & j, CommandPoolTrimFlagBits &p)
  {
      using enum_c_type = CommandPoolTrimFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DebugUtilsMessengerCallbackDataFlagBitsEXT &p)
  {
      using enum_c_type = DebugUtilsMessengerCallbackDataFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DebugUtilsMessengerCreateFlagBitsEXT &p)
  {
      using enum_c_type = DebugUtilsMessengerCreateFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DescriptorPoolResetFlagBits &p)
  {
      using enum_c_type = DescriptorPoolResetFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DescriptorUpdateTemplateCreateFlagBits &p)
  {
      using enum_c_type = DescriptorUpdateTemplateCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DisplayModeCreateFlagBitsKHR &p)
  {
      using enum_c_type = DisplayModeCreateFlagBitsKHR;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, DisplaySurfaceCreateFlagBitsKHR &p)
  {
      using enum_c_type = DisplaySurfaceCreateFlagBitsKHR;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, EventCreateFlagBits &p)
  {
      using enum_c_type = EventCreateFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, HeadlessSurfaceCreateFlagBitsEXT &p)
  {
      using enum_c_type = HeadlessSurfaceCreateFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }
#ifdef VK_USE_PLATFORM_IOS_MVK
  inline void from_json(nlohmann::json const & j, IOSSurfaceCreateFlagBitsMVK &p)
  {
      using enum_c_type = IOSSurfaceCreateFlagBitsMVK;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }
#endif

#ifdef VK_USE_PLATFORM_FUCHSIA
  inline void from_json(nlohmann::json const & j, ImagePipeSurfaceCreateFlagBitsFUCHSIA &p)
  {
      using enum_c_type = ImagePipeSurfaceCreateFlagBitsFUCHSIA;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }
#endif

#ifdef VK_USE_PLATFORM_MACOS_MVK
  inline void from_json(nlohmann::json const & j, MacOSSurfaceCreateFlagBitsMVK &p)
  {
      using enum_c_type = MacOSSurfaceCreateFlagBitsMVK;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }
#endif
  inline void from_json(nlohmann::json const & j, MemoryMapFlagBits &p)
  {
      using enum_c_type = MemoryMapFlagBits;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }
#ifdef VK_USE_PLATFORM_METAL_EXT
  inline void from_json(nlohmann::json const & j, MetalSurfaceCreateFlagBitsEXT &p)
  {
      using enum_c_type = MetalSurfaceCreateFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }
#endif
  inline void from_json(nlohmann::json const & j, PipelineCoverageModulationStateCreateFlagBitsNV &p)
  {
      using enum_c_type = PipelineCoverageModulationStateCreateFlagBitsNV;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineCoverageReductionStateCreateFlagBitsNV &p)
  {
      using enum_c_type = PipelineCoverageReductionStateCreateFlagBitsNV;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineCoverageToColorStateCreateFlagBitsNV &p)
  {
      using enum_c_type = PipelineCoverageToColorStateCreateFlagBitsNV;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineDiscardRectangleStateCreateFlagBitsEXT &p)
  {
      using enum_c_type = PipelineDiscardRectangleStateCreateFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineRasterizationConservativeStateCreateFlagBitsEXT &p)
  {
      using enum_c_type = PipelineRasterizationConservativeStateCreateFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineRasterizationDepthClipStateCreateFlagBitsEXT &p)
  {
      using enum_c_type = PipelineRasterizationDepthClipStateCreateFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineRasterizationStateStreamCreateFlagBitsEXT &p)
  {
      using enum_c_type = PipelineRasterizationStateStreamCreateFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, PipelineViewportSwizzleStateCreateFlagBitsNV &p)
  {
      using enum_c_type = PipelineViewportSwizzleStateCreateFlagBitsNV;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }
#ifdef VK_USE_PLATFORM_GG
  inline void from_json(nlohmann::json const & j, StreamDescriptorSurfaceCreateFlagBitsGGP &p)
  {
      using enum_c_type = StreamDescriptorSurfaceCreateFlagBitsGGP;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }
#endif
  inline void from_json(nlohmann::json const & j, ValidationCacheCreateFlagBitsEXT &p)
  {
      using enum_c_type = ValidationCacheCreateFlagBitsEXT;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }
#ifdef VK_USE_PLATFORM_VI_NN
  inline void from_json(nlohmann::json const & j, ViSurfaceCreateFlagBitsNN &p)
  {
      using enum_c_type = ViSurfaceCreateFlagBitsNN;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, WaylandSurfaceCreateFlagBitsKHR &p)
  {
      using enum_c_type = WaylandSurfaceCreateFlagBitsKHR;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, Win32SurfaceCreateFlagBitsKHR &p)
  {
      using enum_c_type = Win32SurfaceCreateFlagBitsKHR;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, XcbSurfaceCreateFlagBitsKHR &p)
  {
      using enum_c_type = XcbSurfaceCreateFlagBitsKHR;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }

  inline void from_json(nlohmann::json const & j, XlibSurfaceCreateFlagBitsKHR &p)
  {
      using enum_c_type = XlibSurfaceCreateFlagBitsKHR;


          VKB_BEGIN_ENUM_MACRO

          VKB_END_ENUM_MACRO
  }
#endif


}

#undef VKB_BEGIN_ENUM_MACRO
#undef VKB_ENUM_MACRO
#undef VKB_END_ENUM_MACRO

#endif
