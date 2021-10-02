#include <iostream>
#include <cassert>
#include <fstream>
#include <vkb/manager/BindlessTextureManager.h>
#include <vkb/manager/PipelineManager.h>
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
        Ci.totalTextureCube = 1024;

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

        {
            auto id = m_TManager.allocateTextureCube(1024);
            m_TManager.uploadImageData(id,img);
            m_TManager.generateMipMaps(id,0,99999);
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
        vp.x          = 0.0f;
        vp.y          = 0.0f;
        vp.width      = ( float ) swapchainImageSize().width;
        vp.height     = ( float ) swapchainImageSize().height;
        vp.minDepth   = 0.0f;
        vp.maxDepth   = 1.0f;

        VkRect2D sc = {};
        sc.extent = swapchainImageSize();
        vkCmdSetViewport(frame.commandBuffer, 0, 1, &vp);
        vkCmdSetScissor(frame.commandBuffer,0,1,&sc);

        // You should call update once per frame
        // to ensure that all descriptors are updated
        // appropriately
        m_TManager.nextChain();
        m_TManager.update();

        // Bind the texture manager to set 0
        m_TManager.bind(frame.commandBuffer, 0, m_pipeline.layout);



        struct pushC
        {
            int32_t texture0 = 1;
            int32_t texture1 = 2;
        };

        {
            static int jj=0;
            jj++;

            static pushC p = pushC();
            if( jj > 100)
            {
                jj = 0;
                std::swap(p.texture0,p.texture1);
            }

            vkCmdPushConstants(frame.commandBuffer, m_pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT|VK_SHADER_STAGE_FRAGMENT_BIT,0, sizeof(pushC), &p);
        }

        {
            //PFN_vkCreateDebugUtilsMessengerEXT myvkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>( vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" ) );
            PFN_vkCmdSetVertexInputEXT func = reinterpret_cast<PFN_vkCmdSetVertexInputEXT>( vkGetInstanceProcAddr( getInstance(), "vkCmdSetVertexInputEXT" ) );
            func(frame.commandBuffer, 0, nullptr, 0, nullptr);
        }
        vkCmdBindPipeline(frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.pipeline);

        vkCmdDraw(frame.commandBuffer, 3,1,0,0);

        frame.endRenderPass(frame.commandBuffer);

        // request the next frame
        // so that this function will be called again
        requestNextFrame();
    }


    Pipeline_s createGraphicsPipeline2(VkRenderPass renderPass)
    {
        Pipeline_s pp;

        vkb::GraphicsPipelineCreateInfo ci;

        ci.renderPass     = renderPass;
        ci.vertexShader   = createShaderModule(CMAKE_SOURCE_DIR "/share/shaders/BindLessTexture.vert.spv");
        ci.fragmentShader = createShaderModule(CMAKE_SOURCE_DIR "/share/shaders/BindLessTexture.frag.spv");

        ci.dynamicStates = {VK_DYNAMIC_STATE_SCISSOR,
                           VK_DYNAMIC_STATE_VIEWPORT,
                           VK_DYNAMIC_STATE_VERTEX_INPUT_EXT
                           };

        ci.outputTargetCount = 1; // writing to only one output target.
                                  // this needs to match the renderpass

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

            vkCreatePipelineLayout(getDevice(), &lci, nullptr, &ci.pipelineLayout);
            pp.layout = ci.pipelineLayout;
        }

        pp.shaders.push_back(ci.vertexShader);
        pp.shaders.push_back(ci.fragmentShader);
        pp.pipeline = ci.create([&](auto & info)
        {
            // we can modify the values in info if you need
            // more control

            VkPipeline pipeline;
            if (vkCreateGraphicsPipelines(getDevice(), VK_NULL_HANDLE, 1, &info, nullptr, &pipeline) != VK_SUCCESS) {
                throw std::runtime_error("failed to create graphics pipeline!");
            }
            return pipeline;
        });

        return pp;
    }

    Pipeline_s createPipeline(VkRenderPass renderPass)
    {
        return createGraphicsPipeline2(renderPass);
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

    c.instanceInfo.enabledLayers     = { "VK_LAYER_KHRONOS_validation"};
    c.instanceInfo.enabledExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

    c.deviceInfo.deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    c.deviceInfo.deviceExtensions.push_back(VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME);
    c.deviceInfo.deviceExtensions.push_back(VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME);
    c.deviceInfo.deviceExtensions.push_back(VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME);

    // enable a new extended feature
    VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT dynamicVertexState = {};
    dynamicVertexState.sType                   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_INPUT_DYNAMIC_STATE_FEATURES_EXT;
    dynamicVertexState.vertexInputDynamicState = true;
    c.deviceInfo.enabledFeatures12.pNext       = &dynamicVertexState;

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
