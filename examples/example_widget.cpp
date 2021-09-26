#include <iostream>
#include <cassert>
#include <fstream>
#include <vkb/manager/BindlessTextureManager.h>
#include <gul/Image.h>

#define VKW_WINDOW_LIB 1
#if VKW_WINDOW_LIB == 1
#include <vkw/SDLWidget.h>
#elif VKW_WINDOW_LIB == 2
#include <vkw/GLFWWidget.h>
#endif

// callback function for validation layers
static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanReportFunc(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t obj,
    size_t location,
    int32_t code,
    const char* layerPrefix,
    const char* msg,
    void* userData)
{
    std::cout << "VULKAN VALIDATION: " << layerPrefix << " :: " <<  msg << std::endl;;

    return VK_FALSE;
}




namespace vkb
{
template<>
struct ImgQuery<gul::Image>
{
    gul::Image const & I;
    ImgQuery(gul::Image const & i) : I(i)
    {

    }

    void const * data() const
    {
        return I.data();
    }
    uint32_t width() const
    {
        return I.getWidth();
    }
    uint32_t height() const
    {
        return I.getHeight();
    }
    uint32_t depth() const
    {
        return 1;
    }
    uint32_t bytesPerPixel() const
    {
        return 4;
    }
    VkFormat format() const
    {
        return VK_FORMAT_R8G8B8A8_UNORM;
    }
};
}



class MyApplication : public vkw::Application
{
    // Application interface
    vkb::BindlessTextureManager m_TManager;
public:

    struct Pipeline_s
    {
        VkPipeline                  pipeline;
        std::vector<VkShaderModule> shaders;
        VkPipelineLayout            layout;
    };
    Pipeline_s m_pipeline;

    void initResources() override
    {
        // The following can be used here
        // getDevice();
        // getPhysicalDevice();
        // getInstance();

        //=======================================================
        // Set up the BindlessTextureManager with 1024 maximum
        // images
        //=======================================================
        vkb::BindlessTextureManager::CreateInfo Ci;
        Ci.instance       = getInstance();
        Ci.physicalDevice = getPhysicalDevice();
        Ci.device         = getDevice();
        Ci.allocator      = nullptr;// create it for us
        Ci.graphicsQueue  = getGraphicsQueue();
        Ci.totalTexture2D = 1024;

        Ci.allocator = nullptr; // have the texture manager create
                                // the vma Allocator for us.

        m_TManager.create(Ci);
        //=======================================================

        gul::Image img(1024,1024);
        {
            img.r = 255;
            img.a = 255;
            auto id = m_TManager.allocateTexture(img);
            assert(id.index != -1);
        }

        {
            img.r = 0;
            img.g = 255;
            auto id = m_TManager.allocateTexture(img);
            assert(id.index != -1);
        }
        std::cout << "initResources() " << std::endl;
    }
    void releaseResources() override
    {
        // The following can be used here
        // getDevice();
        // getPhysicalDevice();
        // getInstance();
        m_TManager.destroy();
        {
            vkDestroyPipeline(getDevice(), m_pipeline.pipeline, nullptr);
            vkDestroyPipelineLayout(getDevice(), m_pipeline.layout,nullptr);
            for(auto & m : m_pipeline.shaders)
                vkDestroyShaderModule(getDevice(), m, nullptr);
        }
        std::cout << "releaseResources() " << std::endl;
    }
    void initSwapChainResources() override
    {
        // The following can be used here
        // swapchainImageCount();
        // swapchainImage( index );
        // colorFormat();
        // depthStencilFormat();
        // swapchainImageSize();
        // swapchainImageView();
        std::cout << "initSwapchainResources() " << std::endl;
    }
    void releaseSwapChainResources() override
    {
        std::cout << "releaseSwapChainResources() " << std::endl;
    }

    // This function is called whenever
    // requestNextFrame() is called. You can
    // call requestNextFrame() as many times as you want
    // but render() will be called only once.
    void render( vkw::Frame &frame) override
    {
        static bool once=true;
        if( once )
        {
            m_pipeline = createPipeline(frame.renderPass);
            once = false;
        }
        assert( frame.depthImage != VK_NULL_HANDLE);

        frame.clearColor.float32[0] = 0.0f;
        //frame.clearColor.float32[1] = 1.0f;
        //frame.clearColor.float32[2] = 1.0f;
        //frame.clearColor.float32[3] = 1.0f;
        frame.beginRenderPass( frame.commandBuffer );

        VkViewport vp = {};
        vp.x = 0.0f;
        vp.y = 0.0f;
        vp.width  = (float)swapchainImageSize().width;
        vp.height = (float)swapchainImageSize().height;
        vp.minDepth = 0.0f;
        vp.maxDepth = 1.0f;

        VkRect2D sc = {};
        sc.extent = swapchainImageSize();
        vkCmdSetViewport(frame.commandBuffer, 0, 1, &vp);
        vkCmdSetScissor(frame.commandBuffer,0,1,&sc);

        // You should call update once per frame
        // to ensure that all descriptors are updated
        // appropriately
        m_TManager.update();

        // Bind the texture manager to set 0
        m_TManager.bind(frame.commandBuffer, 0, m_pipeline.layout);

        m_TManager.nextChain();

        struct pushC
        {
            int32_t texture0 = 1;
            int32_t texture1 = 1;
        };

        {
            pushC p;
            vkCmdPushConstants(frame.commandBuffer, m_pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT|VK_SHADER_STAGE_FRAGMENT_BIT,0, sizeof(pushC), &p);
        }
        vkCmdBindPipeline(frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.pipeline);

        vkCmdDraw(frame.commandBuffer, 3,1,0,0);

        frame.endRenderPass(frame.commandBuffer);

        // request the next frame
        // so that this function will be called again
        requestNextFrame();
    }


    Pipeline_s createGraphicsPipeline(VkRenderPass renderPass)
    {
        Pipeline_s pp;

        auto vertShaderModule = createShaderModule( CMAKE_SOURCE_DIR "/share/shaders/BindLessTexture.vert.spv" );
        auto fragShaderModule = createShaderModule( CMAKE_SOURCE_DIR "/share/shaders/BindLessTexture.frag.spv" );

        pp.shaders = {vertShaderModule, fragShaderModule};

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) 1024.0f;
        viewport.height = (float) 768.0f;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = {1024,768};

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports    = &viewport;
        viewportState.scissorCount  = 1;
        viewportState.pScissors     = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineLayout pipelineLayout;
        {
            VkPipelineLayoutCreateInfo lci = {};

            auto dlayout       = m_TManager.getDescriptorSetLayout();
            lci.pSetLayouts    = &dlayout;
            lci.setLayoutCount = 1;

            VkPushConstantRange range;
            lci.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            range.offset               = 0;
            range.size                 = 128;
            range.stageFlags           = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            lci.pushConstantRangeCount = 1;
            lci.pPushConstantRanges    = &range;

            vkCreatePipelineLayout(getDevice(), &lci, nullptr, &pipelineLayout);
            pp.layout = pipelineLayout;
        }

        VkPipelineDepthStencilStateCreateInfo dsInfo = {};
        dsInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDepthStencilState = &dsInfo;
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pp.pipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        //vkDestroyShaderModule(getDevice(), fragShaderModule, nullptr);
        //vkDestroyShaderModule(getDevice(), vertShaderModule, nullptr);

        return pp;
    }

    Pipeline_s createPipeline(VkRenderPass renderPass)
    {
        return createGraphicsPipeline(renderPass);
        Pipeline_s pp;

        auto m_vertShader = createShaderModule( CMAKE_SOURCE_DIR "/share/shaders/BindLessTexture.vert.spv" );
        auto m_fragShader = createShaderModule( CMAKE_SOURCE_DIR "/share/shaders/BindLessTexture.frag.spv" );

        pp.shaders = {m_vertShader, m_fragShader};

        VkPipelineShaderStageCreateInfo shaderStageCreateInfo[2] = {};

        shaderStageCreateInfo[0].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageCreateInfo[0].stage  = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStageCreateInfo[0].module = m_vertShader;
        shaderStageCreateInfo[0].pName  = "main";

        shaderStageCreateInfo[1].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageCreateInfo[1].stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStageCreateInfo[1].module = m_fragShader;
        shaderStageCreateInfo[1].pName  = "main";

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        VkPipelineInputAssemblyStateCreateInfo pipelineIACreateInfo = {};
        pipelineIACreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        pipelineIACreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        VkPipelineRasterizationStateCreateInfo rastCreateInfo = {};

        rastCreateInfo.sType       = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rastCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
        rastCreateInfo.cullMode    = VK_CULL_MODE_NONE;
        rastCreateInfo.frontFace   = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rastCreateInfo.lineWidth   = 1.0f;

        VkPipelineMultisampleStateCreateInfo pipelineMSCreateInfo = {};
        pipelineMSCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        pipelineMSCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;


        VkPipelineColorBlendAttachmentState blendAttachState{};
        blendAttachState.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        blendAttachState.blendEnable         = VK_FALSE;
        blendAttachState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        blendAttachState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;// Optional
        blendAttachState.colorBlendOp        = VK_BLEND_OP_ADD;     // Optional
        blendAttachState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        blendAttachState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;// Optional
        blendAttachState.alphaBlendOp        = VK_BLEND_OP_ADD;     // Optional


        VkPipelineColorBlendStateCreateInfo blendCreateInfo = {};
        blendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        blendCreateInfo.logicOpEnable = VK_FALSE;
        blendCreateInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
        blendCreateInfo.attachmentCount = 1;
        blendCreateInfo.pAttachments = &blendAttachState;
        blendCreateInfo.blendConstants[0] = 0.0f; // Optional
        blendCreateInfo.blendConstants[1] = 0.0f; // Optional
        blendCreateInfo.blendConstants[2] = 0.0f; // Optional
        blendCreateInfo.blendConstants[3] = 0.0f; // Optional;

        VkPipelineDynamicStateCreateInfo dynamicStates = {};
        VkDynamicState dynamic[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

        dynamicStates.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStates.dynamicStateCount = 2;
        dynamicStates.pDynamicStates    = dynamic;

        VkPipelineDepthStencilStateCreateInfo dsInfo = {};
        dsInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

        VkPipelineLayout layout;
        {
            VkPipelineLayoutCreateInfo lci = {};

            auto dlayout = m_TManager.getDescriptorSetLayout();
            lci.pSetLayouts    = &dlayout;
            lci.setLayoutCount = 1;

            VkPushConstantRange range;
            lci.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            range.offset               = 0;
            range.size                 = 128;
            range.stageFlags           = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            lci.pushConstantRangeCount = 1;
            lci.pPushConstantRanges    = &range;

            vkCreatePipelineLayout(getDevice(), &lci, nullptr, &layout);
            pp.layout = layout;
        }

        VkViewport vp = {};
        vp.x = 0.0f;
        vp.y = 0.0f;
        vp.width  = (float)swapchainImageSize().width;
        vp.height = (float)swapchainImageSize().height;
        vp.minDepth = 0.0f;
        vp.maxDepth = 1.0f;

        VkRect2D sc = {};
        sc.extent = {1024,768};


        VkPipelineViewportStateCreateInfo vpCreateInfo = {};
        vpCreateInfo.sType                             = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        vpCreateInfo.viewportCount                     = 1;
        vpCreateInfo.pViewports                        = &vp;
        vpCreateInfo.scissorCount                      = 1;
        vpCreateInfo.pScissors                         = &sc;

        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount                   = 1;
        pipelineInfo.pStages                      = &shaderStageCreateInfo[0];
        pipelineInfo.pVertexInputState            = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState          = &pipelineIACreateInfo;
        pipelineInfo.pViewportState               = &vpCreateInfo;
        pipelineInfo.pDepthStencilState           = &dsInfo;
        pipelineInfo.pRasterizationState          = &rastCreateInfo;
        pipelineInfo.pMultisampleState            = &pipelineMSCreateInfo;
        pipelineInfo.pColorBlendState             = &blendCreateInfo;
        pipelineInfo.pDynamicState                = &dynamicStates;
        pipelineInfo.renderPass                   = renderPass;
        pipelineInfo.layout                       = layout;
        pipelineInfo.basePipelineIndex            = -1;

        VkPipeline pipeline;
        VK_CHECK_RESULT(vkCreateGraphicsPipelines( getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &pipeline) );
        pp.pipeline = pipeline;
        return pp;
    }

    VkShaderModule createShaderModule(const char* pFileName)
    {
        auto ShaderCode = readResourceBIN(pFileName);

        VkShaderModuleCreateInfo shaderCreateInfo = {};
        shaderCreateInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderCreateInfo.codeSize = ShaderCode.size();
        shaderCreateInfo.pCode    = static_cast<const uint32_t *>( static_cast<void*>(ShaderCode.data()) );

        VkShaderModule shaderModule;
        VK_CHECK_RESULT(vkCreateShaderModule(getDevice(), &shaderCreateInfo, NULL, &shaderModule) );

        return shaderModule;
    }

    std::vector<uint8_t> readResourceBIN(const std::string & path) const
    {
        std::ifstream stream(path, std::ios::in | std::ios::binary);
        if( !stream )
        {
            throw  std::runtime_error("Invalid file");
        }
        std::vector<uint8_t> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        return contents;
    }
};


template<typename Widget_t>
int startApp( MyApplication & app,
              Widget_t & vulkanWindow,
              typename Widget_t::CreateInfo const &ci)
{
#if VKW_WINDOW_LIB == 1
    // This needs to be called first to initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);
#elif VKW_WINDOW_LIB == 2
    glfwInit();
#endif

    vulkanWindow.create(ci);

#if VKW_WINDOW_LIB == 1
    // put the window in the main loop
    // and provide a callback function for the SDL events
    vulkanWindow.exec(&app,
                      [&app](SDL_Event const & evt)
    {
        if( evt.type == SDL_QUIT)
            app.quit();
    });
#elif VKW_WINDOW_LIB == 2
    // put the window in the main loop
    // GLFW requires you to register callbacks
    // for input events. you will have to do these yourself
    vulkanWindow.exec(&app);
#endif

    vulkanWindow.destroy();


#if VKW_WINDOW_LIB == 1
    SDL_Quit();
#elif VKW_WINDOW_LIB == 2
    glfwTerminate();
#endif

    return 0;
}


#if defined(__WIN32__)
int SDL_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    (void)argc;
    (void)argv;

    // create a vulkan window widget
#if VKW_WINDOW_LIB == 1
    using WidgetType = vkw::SDLVulkanWidget;
#elif VKW_WINDOW_LIB == 2
    using WidgetType = vkw::GLFWVulkanWidget;
#endif

    WidgetType vulkanWindow;

    // set the initial properties of the
    // window. Also specify that we want
    // a depth stencil attachment
    WidgetType::CreateInfo c;
    c.width       = 1024;
    c.height      = 768;
    c.windowTitle = "My Vulkan Application Window";

//    c.surfaceInfo.depthFormat    = VK_FORMAT_D32_SFLOAT_S8_UINT;
    c.instanceInfo.debugCallback = &VulkanReportFunc;

    c.deviceInfo.deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    c.deviceInfo.deviceExtensions.push_back(VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME);

    // enable a new extended feature
    //VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT dynamicVertexState = {};
    //dynamicVertexState.sType                    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_INPUT_DYNAMIC_STATE_FEATURES_EXT;
    //dynamicVertexState.vertexInputDynamicState  = true;
    //c.deviceInfo.enabledFeatures12.pNext         = &dynamicVertexState;

    // create the window and initialize
    //vulkanWindow.create(c);

    // Here is the actual vulkan application that does
    // all the rendering.
    MyApplication app;

    // Send our application to the window
    // manager
    return startApp( app, vulkanWindow, c);
}




#include <vkw/SDLVulkanWindow_INIT.inl>
#include <vkw/SDLVulkanWindow_USAGE.inl>


#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
