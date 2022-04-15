#include "catch.hpp"
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <vkw/Adapters/SDLVulkanWindowAdapter.h>
#include <vkw/VKWVulkanWindow.inl>


#include <vulkan/vulkan.hpp>

#include <vkb/vkb.h>

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
    (void)obj;
    (void)flags;
    (void)objType;
    (void)location;
    (void)code;
    (void)userData;
    printf("VULKAN VALIDATION: [%s] %s\n", layerPrefix, msg);
    //throw std::runtime_error( msg );
    return VK_FALSE;
}


std::shared_ptr<vkw::VKWVulkanWindow> createWindow(uint32_t width, uint32_t height)
{
    // create a default window and initialize all vulkan
    // objects.
    auto window     = new vkw::VKWVulkanWindow();
    auto sdl_window = new vkw::SDLVulkanWindowAdapter();

    // 1. create the window and set the adapater
    sdl_window->createWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024,768);
    window->setWindowAdapater(sdl_window);

    // 2. Create the Instance
    vkw::VKWVulkanWindow::InstanceInitilizationInfo2 instanceInfo;
    instanceInfo.debugCallback = &VulkanReportFunc;
    instanceInfo.vulkanVersion = VK_MAKE_VERSION(1, 0, 0);
    instanceInfo.enabledExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

    window->createVulkanInstance(instanceInfo);

    // 3. Create the surface
    vkw::VKWVulkanWindow::SurfaceInitilizationInfo2 surfaceInfo;
    surfaceInfo.depthFormat          = VkFormat::VK_FORMAT_D32_SFLOAT_S8_UINT;
    surfaceInfo.presentMode          = VK_PRESENT_MODE_FIFO_KHR;
    surfaceInfo.additionalImageCount = 1;// how many additional swapchain images should we create ( total = min_images + additionalImageCount
    window->createVulkanSurface(surfaceInfo);


    // 4. Create the device
    //    and add additional extensions that we want to enable
    vkw::VKWVulkanWindow::DeviceInitilizationInfo2 deviceInfo;
    deviceInfo.deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    deviceInfo.deviceExtensions.push_back(VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME);

    // enable a new extended feature
    VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT dynamicVertexState = {};
    dynamicVertexState.sType                    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_INPUT_DYNAMIC_STATE_FEATURES_EXT;
    dynamicVertexState.vertexInputDynamicState  = true;
    deviceInfo.enabledFeatures12.pNext          = &dynamicVertexState;

    window->createVulkanDevice(deviceInfo);

    return std::shared_ptr<vkw::VKWVulkanWindow>(window, [](vkw::VKWVulkanWindow * w)
    {
       delete w->getWindowAdapter();
       delete w;
    });
}


