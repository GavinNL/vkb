#include "catch.hpp"
#include <fstream>

#include "unit_helpers.h"

#include <vkb/vkb.h>



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

    {
        vkb::DescriptorSetLayoutCreateInfo2 v;

        v.addDescriptor(0, vk::DescriptorType::eCombinedImageSampler, 4, vk::ShaderStageFlagBits::eFragment);
        REQUIRE( v.bindings.size() == 1);
        REQUIRE( v.bindings[0].binding         == 0);
        REQUIRE( v.bindings[0].descriptorType  == vk::DescriptorType::eCombinedImageSampler);
        REQUIRE( v.bindings[0].descriptorCount == 4);
        REQUIRE( v.bindings[0].stageFlags      == vk::ShaderStageFlagBits::eFragment);

        v.flags = {};


        vkb::Storage S;

        // Create the pool
        auto d = v.create( S, window->getDevice() );
        auto d2 = v.create( S, window->getDevice() );

        REQUIRE( d == d2);

        REQUIRE( d != vk::DescriptorSetLayout() );


        S.destroy(d, window->getDevice());

        auto & Ci = S.getCreateInfo<vkb::DescriptorSetLayoutCreateInfo2>(d);

        REQUIRE( Ci.bindings.size()             == v.bindings.size()            );
        REQUIRE( Ci.bindings[0].binding         == v.bindings[0].binding        );
        REQUIRE( Ci.bindings[0].descriptorType  == v.bindings[0].descriptorType );
        REQUIRE( Ci.bindings[0].descriptorCount == v.bindings[0].descriptorCount);
        REQUIRE( Ci.bindings[0].stageFlags      == v.bindings[0].stageFlags     );

        REQUIRE( Ci.hash() == v.hash() );

    }

    window->destroy();
    window.reset();

    SDL_Quit();

}

