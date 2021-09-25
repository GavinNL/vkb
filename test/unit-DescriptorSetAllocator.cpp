#include "catch.hpp"
#include <fstream>

#include "unit_helpers.h"
#include <vkb/vkb.h>
#include <vkb/utils/DescriptorSetAllocator.h>

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


    vkb::DescriptorSetAllocator dSetAllocator;

    vkb::DescriptorPoolCreateInfo2 basicPool;
    basicPool.setMaxSets(10)
         .setPoolSize(vk::DescriptorType::eCombinedImageSampler, 10)
         .setPoolSize(vk::DescriptorType::eUniformBuffer, 10)
         .setFlags(   vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet
                    | vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind);

    dSetAllocator.init(&S, window->getDevice(), basicPool );


    // Allocate 7 combined image samplers
    auto set = dSetAllocator.allocateDescriptorSet( {{ 0, vk::DescriptorType::eCombinedImageSampler, 7, vk::ShaderStageFlagBits::eFragment, {}}});

    auto parentPool = dSetAllocator.getParentPool(set);
    {
        auto & poolInfo = dSetAllocator.getPoolInfo(parentPool);

        // there are 9 sets remaining
        REQUIRE( poolInfo.remainingSets == 9);

        // and 3 combined image samplers
        REQUIRE( poolInfo.remaining.at(vk::DescriptorType::eCombinedImageSampler) == 3);
    }

    // free the set
    dSetAllocator.freeDescriptorSet(set);

    {

        auto & poolInfo = dSetAllocator.getPoolInfo(parentPool);

        // we are now back to 10 sets remaining
        REQUIRE( poolInfo.remainingSets == 10);

        // and 10 combined image samplers
        REQUIRE( poolInfo.remaining.at(vk::DescriptorType::eCombinedImageSampler) == 10);
    }


    // allocate two sets
    {
        auto set1 = dSetAllocator.allocateDescriptorSet( {
                                                             { 0, vk::DescriptorType::eCombinedImageSampler, 4, vk::ShaderStageFlagBits::eFragment, {}},
                                                             { 1, vk::DescriptorType::eCombinedImageSampler, 3, vk::ShaderStageFlagBits::eVertex  , {}}
                                                         });

        auto set2 = dSetAllocator.allocateDescriptorSet( {{ 0, vk::DescriptorType::eCombinedImageSampler, 7, vk::ShaderStageFlagBits::eFragment, {}}});

        // they come from different pools because
        // each pool only has 10 combined image samplers
        REQUIRE( dSetAllocator.getParentPool(set1) != dSetAllocator.getParentPool(set2) );
    }

    // destroy the pools
    dSetAllocator.destroy();


    S.destroyAll(window->getDevice());

    window->destroy();
    window.reset();

    SDL_Quit();

}

