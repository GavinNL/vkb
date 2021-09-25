#include <iostream>
#include <cassert>

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


class MyApplication : public vkw::Application
{
    // Application interface
public:
    void initResources() override
    {
        // The following can be used here
        // getDevice();
        // getPhysicalDevice();
        // getInstance();
        std::cout << "initResources() " << std::endl;
    }
    void releaseResources() override
    {
        // The following can be used here
        // getDevice();
        // getPhysicalDevice();
        // getInstance();
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
    void render( vkw::Frame &frame) override
    {
        assert( frame.depthImage != VK_NULL_HANDLE);

        frame.clearColor.float32[0] = 0.0f;
        //frame.clearColor.float32[1] = 1.0f;
        //frame.clearColor.float32[2] = 1.0f;
        //frame.clearColor.float32[3] = 1.0f;
        frame.beginRenderPass( frame.commandBuffer );

        frame.endRenderPass(frame.commandBuffer);

        // request the next frame
        // so that this function will be called again
        requestNextFrame();
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

    c.surfaceInfo.depthFormat    = VK_FORMAT_D32_SFLOAT_S8_UINT;
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
