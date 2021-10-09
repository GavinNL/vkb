#include "catch.hpp"
#include <fstream>

#include "unit_helpers.h"

#include <vkb/vkb.h>
#include <vkb/utils/DynamicPipeline.h>
#include <vkb/utils/DescriptorSetAllocator.h>
#include <vkb/manager/BindlessTextureManager.h>


struct SimpleImage
{
    SimpleImage(uint32_t w, uint32_t h) : width(w), height(h)
    {
        data.resize(w*h);
    }
    uint32_t              width;
    uint32_t              height;
    std::vector<uint32_t> data;
};

namespace vkb
{
template<>
struct ImgQuery<SimpleImage>
{
    SimpleImage const & I;
    ImgQuery(SimpleImage const & i) : I(i)
    {

    }

    void const * data() const
    {
        return I.data.data();
    }
    uint32_t width() const
    {
        return I.width;
    }
    uint32_t height() const
    {
        return I.width;
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


SCENARIO( " Scenario 1: Create a DescriptorSetLayout" )
{
    // create a default window and initialize all vulkan
    // objects.
    auto window = createWindow(1024,768);


    // resize the framegraph to the size of the
    // swapchain. This will allocate any internal
    // images which depend on the size of the swapchain (eg: gBuffers)
    auto e = window->getSwapchainExtent();
    (void)e;

    vkb::Storage S;

    vkb::BindlessTextureManager::CreateInfo Ci;
    Ci.instance       = window->getInstance();
    Ci.physicalDevice = window->getPhysicalDevice();
    Ci.device         = window->getDevice();
    Ci.allocator      = nullptr;// create it for us
    Ci.graphicsQueue  = window->getGraphicsQueue();
    Ci.totalTexture2D = 1024;
    vkb::BindlessTextureManager tManager;

    tManager.create(Ci);

    auto id = tManager.allocateTexture({256,256});

    SimpleImage img(256,256);

    tManager.uploadImageData(id, img);

    tManager.generateMipMaps(id,0,1);

    auto id2 = tManager.allocateTexture(img);
    // frame 1
    tManager.update();
        // tManager.bind(0); // bind to descriptor set 0
        // draw

    // frame 2
    tManager.update();

        // draw
    // frame 3
    tManager.update();
        // draw

    // frame 4
    tManager.update();
        // draw

    tManager.destroy();


    S.destroyAll(window->getDevice());

    window->destroy();
    window.reset();

    SDL_Quit();

}

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
