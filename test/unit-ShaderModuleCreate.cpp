#include "catch.hpp"
#include <fstream>

#include "unit_helpers.h"

#include <vkb/vkb.h>


std::vector<uint32_t> readSPV(std::string path)
{
    std::ifstream stream(path, std::ios::in | std::ios::binary);
    std::vector<char> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

    assert( contents.size() % sizeof(uint32_t) == 0);
    std::vector<uint32_t> code;
    code.resize( contents.size() / sizeof(uint32_t));
    std::memcpy( code.data(), contents.data(), contents.size());
    return code;
}

SCENARIO( " Scenario 1: Create a DescriptorSetLayout" )
{
    // create a default window and initialize all vulkan
    // objects.
    auto window = createWindow(1024,768);

    // Resize the framegraph to the size of the
    // swapchain. This will allocate any internal
    // images which depend on the size of the swapchain (eg: gBuffers)
    auto e = window->getSwapchainExtent();


    vkb::Storage S;
    {

        vkb::ShaderModuleCreateInfo2 smci;
        smci.code = readSPV( CMAKE_SOURCE_DIR "/share/shaders/vert.spv" );

        auto module  = smci.create(S, window->getDevice());
        auto module2 = smci.create(S, window->getDevice());
        REQUIRE(module == module2);
        REQUIRE( module != vk::ShaderModule()) ;

        S.destroy(module, window->getDevice());
    }

    S.destroyAll(window->getDevice());

    window->destroy();
    window.reset();

    SDL_Quit();

}

