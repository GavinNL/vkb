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
        vkb::PipelineLayoutCreateInfo2 v;// = J.get< vkb::PipelineLayoutCreateInfo2 >();

        // set 0
        v.newDescriptorSet().addDescriptor(0, vk::DescriptorType::eCombinedImageSampler, 4, vk::ShaderStageFlagBits::eFragment);

        // set 1
        v.newDescriptorSet().addDescriptor(0, vk::DescriptorType::eStorageBuffer, 1, vk::ShaderStageFlagBits::eVertex);

        // push constants
        v.addPushConstantRange( vk::ShaderStageFlagBits::eFragment, 0, 64);

        REQUIRE( v.pushConstantRanges.size() == 1);
        REQUIRE( v.setLayoutsDescriptions.size() == 2);
        REQUIRE( v.setLayouts.size() == 0);

        REQUIRE( v.setLayoutsDescriptions[0].bindings.size() == 1);
        REQUIRE( v.setLayoutsDescriptions[0].bindings[0].binding         == 0);
        REQUIRE( v.setLayoutsDescriptions[0].bindings[0].descriptorType  == vk::DescriptorType::eCombinedImageSampler);
        REQUIRE( v.setLayoutsDescriptions[0].bindings[0].descriptorCount == 4);
        REQUIRE( v.setLayoutsDescriptions[0].bindings[0].stageFlags      == vk::ShaderStageFlagBits::eFragment);

        REQUIRE( v.setLayoutsDescriptions[1].bindings.size() == 1);
        REQUIRE( v.setLayoutsDescriptions[1].bindings[0].binding         == 0);
        REQUIRE( v.setLayoutsDescriptions[1].bindings[0].descriptorType  == vk::DescriptorType::eStorageBuffer);
        REQUIRE( v.setLayoutsDescriptions[1].bindings[0].descriptorCount == 1);
        REQUIRE( v.setLayoutsDescriptions[1].bindings[0].stageFlags      == vk::ShaderStageFlagBits::eVertex);
        vkb::Storage S;

        // Create the pool
        auto d  = v.create( S, window->getDevice() );
        auto d2 = v.create( S, window->getDevice() );

        REQUIRE( d == d2);

        REQUIRE( d != vk::PipelineLayout() );

        auto & Ci = S.getCreateInfo<vkb::PipelineLayoutCreateInfo2>(d);

        REQUIRE( Ci.setLayouts.size() == v.setLayoutsDescriptions.size() );
        S.destroy(d, window->getDevice());

        S.destroyAll(window->getDevice());
    }


    window->destroy();
    window.reset();

    SDL_Quit();

}

