#include "catch.hpp"
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <vkw/SDLVulkanWindow.h>
#include <vkw/SDLVulkanWindow_INIT.inl>
#include <vkw/SDLVulkanWindow_USAGE.inl>

using namespace vkw;

#include <vulkan/vulkan.hpp>

#include <vkb/vkb.h>

#include <vkb/utils/SPIRV_DescriptorSetLayoutGenerator.h>

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


SCENARIO( " Scenario 1: Create a DescriptorSetLayout" )
{
    SDL_Init(SDL_INIT_EVERYTHING);
    auto window = new SDLVulkanWindow();

    // 1. create the window
    window->createWindow("Simple Deferred", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 1024,768);

    // 2. initialize the vulkan instance
    SDLVulkanWindow::InitilizationInfo info;
    info.callback = VulkanReportFunc;
    window->createVulkanInstance( info);

    // 3. Create the following objects:
    //    instance, physical device, device, graphics/present queues,
    //    swap chain, depth buffer, render pass and framebuffers
    window->initSurface(SDLVulkanWindow::SurfaceInitilizationInfo());

    // resize the framegraph to the size of the
    // swapchain. This will allocate any internal
    // images which depend on the size of the swapchain (eg: gBuffers)
    auto e = window->getSwapchainExtent();
    (void)e;

#if 1
    GIVEN("Some shader source code")
    {
        auto vertexS   =  readSPV(CMAKE_SOURCE_DIR "/share/shaders/pbr_vert.spv");
        auto fragmentS =  readSPV(CMAKE_SOURCE_DIR "/share/shaders/pbr_frag.spv");

        vkb::SPIRV_PipelineReflector reflector;
        reflector.addSPIRVCode( vertexS, VK_SHADER_STAGE_VERTEX_BIT);
        reflector.addSPIRVCode( fragmentS, VK_SHADER_STAGE_FRAGMENT_BIT);

        THEN("We can get the vertex input descriptions")
        {
            std::vector< VkVertexInputAttributeDescription > vertexInput = reflector.generateVertexInputDescriptions();
            auto                                             names       = reflector.generateVertexInputNames();

            REQUIRE( vertexInput.size() == 8);
            REQUIRE( vertexInput[0].location == 0);
            REQUIRE( vertexInput[1].location == 1);
            REQUIRE( vertexInput[2].location == 2);
            REQUIRE( vertexInput[3].location == 3);
            REQUIRE( vertexInput[4].location == 4);
            REQUIRE( vertexInput[5].location == 5);
            REQUIRE( vertexInput[6].location == 6);
            REQUIRE( vertexInput[7].location == 7);

            REQUIRE( vertexInput[0].format == VK_FORMAT_R32G32B32_SFLOAT);
            REQUIRE( vertexInput[1].format == VK_FORMAT_R32G32B32_SFLOAT);
            REQUIRE( vertexInput[2].format == VK_FORMAT_R32G32B32_SFLOAT);
            REQUIRE( vertexInput[3].format == VK_FORMAT_R32G32_SFLOAT);
            REQUIRE( vertexInput[4].format == VK_FORMAT_R32G32_SFLOAT);
            REQUIRE( vertexInput[5].format == VK_FORMAT_R32G32B32A32_SFLOAT); // meant for colour, but no way to determine if u8 or int
            REQUIRE( vertexInput[6].format == VK_FORMAT_R32G32B32A32_UINT);
            REQUIRE( vertexInput[7].format == VK_FORMAT_R32G32B32A32_SFLOAT);

            REQUIRE( names[0] == "in_POSITION"   );
            REQUIRE( names[1] == "in_NORMAL"     );
            REQUIRE( names[2] == "in_TANGENT"    );
            REQUIRE( names[3] == "in_TEXCOORD_0" );
            REQUIRE( names[4] == "in_TEXCOORD_1" );
            REQUIRE( names[5] == "in_COLOR_0"    );
            REQUIRE( names[6] == "in_JOINTS_0"   );
            REQUIRE( names[7] == "in_WEIGHTS_0"  );


            // get all the Descriptor Set Layout bindings
            auto bindingLayouts = reflector.generateDescriptorSetLayoutBindings();

            // There are 3 descriptor sets
            REQUIRE(bindingLayouts.size() == 3);

            REQUIRE(bindingLayouts[0].size() == 3);
            REQUIRE(bindingLayouts[1].size() == 2);
            REQUIRE(bindingLayouts[2].size() == 2);

            REQUIRE(bindingLayouts[0][0].binding == 0);
            REQUIRE(bindingLayouts[0][0].descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
            REQUIRE(bindingLayouts[0][0].descriptorCount == 1);
            REQUIRE(bindingLayouts[0][0].stageFlags == VkShaderStageFlags(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT ));

            REQUIRE(bindingLayouts[0][1].binding == 1);
            REQUIRE(bindingLayouts[0][1].descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
            REQUIRE(bindingLayouts[0][1].descriptorCount == 1);
            REQUIRE(bindingLayouts[0][1].stageFlags == VkShaderStageFlags(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT ));

            REQUIRE(bindingLayouts[0][2].binding == 2);
            REQUIRE(bindingLayouts[0][2].descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
            REQUIRE(bindingLayouts[0][2].descriptorCount == 1);
            REQUIRE(bindingLayouts[0][2].stageFlags == VkShaderStageFlags(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT ));


            REQUIRE(bindingLayouts[1][0].binding == 0);
            REQUIRE(bindingLayouts[1][0].descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
            REQUIRE(bindingLayouts[1][0].descriptorCount == 32);
            REQUIRE(bindingLayouts[1][0].stageFlags == VkShaderStageFlags( VK_SHADER_STAGE_FRAGMENT_BIT ));

            REQUIRE(bindingLayouts[1][1].binding == 1);
            REQUIRE(bindingLayouts[1][1].descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
            REQUIRE(bindingLayouts[1][1].descriptorCount == 32);
            REQUIRE(bindingLayouts[1][1].stageFlags == VkShaderStageFlags( VK_SHADER_STAGE_FRAGMENT_BIT ));


            REQUIRE(bindingLayouts[2][0].binding == 3);
            REQUIRE(bindingLayouts[2][0].descriptorType == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);
            REQUIRE(bindingLayouts[2][0].descriptorCount == 64);
            REQUIRE(bindingLayouts[2][0].stageFlags == VkShaderStageFlags( VK_SHADER_STAGE_FRAGMENT_BIT ));

            REQUIRE(bindingLayouts[2][1].binding == 4);
            REQUIRE(bindingLayouts[2][1].descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER);
            REQUIRE(bindingLayouts[2][1].descriptorCount == 64);
            REQUIRE(bindingLayouts[2][1].stageFlags == VkShaderStageFlags( VK_SHADER_STAGE_FRAGMENT_BIT ));


            // Generate the push conststs
            auto pushConstsRange = reflector.generatePushConstantRange();

            REQUIRE( pushConstsRange.size() == 2);

            REQUIRE( pushConstsRange[0].offset == 0);
            REQUIRE( pushConstsRange[0].size   == 32);
            REQUIRE( pushConstsRange[0].stageFlags == VkShaderStageFlags( VK_SHADER_STAGE_VERTEX_BIT ));

            REQUIRE( pushConstsRange[1].offset == 20);
            REQUIRE( pushConstsRange[1].size   == 24);
            REQUIRE( pushConstsRange[1].stageFlags == VkShaderStageFlags( VK_SHADER_STAGE_FRAGMENT_BIT ));


            // generate the blend attachment states
            // these values will all be the same
            // you will need to set them up appropriately
            auto blend = reflector.generateBlendStateAttachments();

            REQUIRE( blend.size() == 1);
        }
    }
#endif
    {
        vkb::SPIRV_PipelineReflector reflector;

        reflector.addSPIRVCode( readSPV(CMAKE_SOURCE_DIR "/share/shaders/pbr_vert.spv"), VK_SHADER_STAGE_VERTEX_BIT);
        reflector.addSPIRVCode( readSPV(CMAKE_SOURCE_DIR "/share/shaders/pbr_frag.spv"), VK_SHADER_STAGE_FRAGMENT_BIT);


        {
            for(auto & x : reflector.generateVertexInputDescriptions() )
            {
                std::cout << "Location: " << x.location<< std::endl;
                std::cout << "Binding : " << x.binding << std::endl;
                std::cout << "Format  : " << vk::to_string( static_cast<vk::Format>(x.format)) << std::endl;
                std::cout << "Offset  : " << x.offset << std::endl;
            }
        }
        auto bindingLayouts = reflector.generateDescriptorSetLayoutBindings();

        vk::Device Dev(window->getDevice());

        VkPipelineLayoutCreateInfo plci = {};
        plci.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

        std::vector<VkDescriptorSetLayout> setLayouts;
        for(auto & set : bindingLayouts)
        {
            std::cout << "Set: " << set.first << std::endl;

            VkDescriptorSetLayoutCreateInfo dci;
            dci.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            dci.pBindings    = set.second.data();
            dci.bindingCount = static_cast<uint32_t>(set.second.size());
            dci.flags        = 0;
            dci.pNext        = nullptr;

            for(auto & b : set.second)
            {
                std::cout << "  Binding        : " << b.binding << std::endl;
                std::cout << "  Type           : " << vk::to_string(static_cast<vk::DescriptorType>(b.descriptorType)) << std::endl;
                std::cout << "  descriptorCount: " << b.descriptorCount << std::endl;
                std::cout << "  stage          : " << vk::to_string(static_cast<vk::ShaderStageFlags>(b.stageFlags)) << std::endl;
                std::cout << std::endl;
            }

            auto layout      = Dev.createDescriptorSetLayout(dci);

            REQUIRE( layout );
            setLayouts.push_back(layout);
        }



        auto pushConstsRanges = reflector.generatePushConstantRange();
        for(auto & r : pushConstsRanges)
        {
            std::cout << "  Stage: " << to_string(static_cast<vk::ShaderStageFlags>(r.stageFlags)) << std::endl;
            std::cout << "  Offset: " << r.offset << std::endl;
            std::cout << "  Size  : " << r.size << std::endl;
        }
        plci.setLayoutCount         = static_cast<uint32_t>(setLayouts.size());
        plci.pSetLayouts            = setLayouts.data();
        plci.pPushConstantRanges    = pushConstsRanges.data();
        plci.pushConstantRangeCount = static_cast<uint32_t>(pushConstsRanges.size());
        plci.pNext                  = nullptr;
        auto pipelineLayout = Dev.createPipelineLayout(plci);
        REQUIRE( pipelineLayout);

        Dev.destroyPipelineLayout(pipelineLayout);

        for(auto & layout : setLayouts)
        {
            Dev.destroyDescriptorSetLayout(layout);
        }
    }

    delete window;
    SDL_Quit();

}

