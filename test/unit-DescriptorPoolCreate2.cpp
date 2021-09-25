#include "catch.hpp"
#include <fstream>

#include "unit_helpers.h"


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

    vkb::DescriptorPoolCreateInfo2 pC;
    pC.setPoolSize( vk::DescriptorType::eCombinedImageSampler, 10);
    pC.setPoolSize( vk::DescriptorType::eUniformBuffer, 10);
    pC.setMaxSets(10);

    auto dPool = pC.create(S, window->getDevice());

    REQUIRE( dPool != vk::DescriptorPool() );

    {
        auto & ci = S.getCreateInfo<vkb::DescriptorPoolCreateInfo2>(dPool);

        REQUIRE( ci.hash() == pC.hash() );
    }

    {
        vkb::DescriptorSetUpdater bb;
        bb.updateTexelBufferDescriptor( {},{},{}, vk::DescriptorType::eStorageTexelBuffer, { vk::BufferView(), vk::BufferView(), vk::BufferView()});
    }
    {
        vkb::DescriptorSetLayoutCreateInfo2 v;

        v.addDescriptor(0, vk::DescriptorType::eCombinedImageSampler, 4, vk::ShaderStageFlagBits::eFragment);
        REQUIRE( v.bindings.size() == 1);
        REQUIRE( v.bindings[0].binding         == 0);
        REQUIRE( v.bindings[0].descriptorType  == vk::DescriptorType::eCombinedImageSampler);
        REQUIRE( v.bindings[0].descriptorCount == 4);
        REQUIRE( v.bindings[0].stageFlags      == vk::ShaderStageFlagBits::eFragment);

        v.flags = {};




        auto dSet = v.allocateFromPool(S, dPool, window->getDevice());

        REQUIRE( dSet != vk::DescriptorSet()) ;

    }
    S.destroyAll( window->getDevice());


    vk::Device(window->getDevice()).destroyDescriptorPool( dPool );

    window->destroy();
    window.reset();

    SDL_Quit();

}


