# VKB - Vulkan Builder

A small helper library to make building Vulkan object a little easier. This
library provides a few alternatives to the standard `vk::XXXXCreateInfo`
structs. These are in the `vkb` namespace.

The following new structs are created:

* vkb::DescriptorSetLayoutCreateInfo2
* vkb::PipelineLayoutCreateInfo2
* vkb::RenderPassCreateInfo2
* vkb::ShaderModuleCreateInfo2
* vkb::GraphicsPipelineCreateInfo2
* vkb::DescriptorPoolCreateInfo2
* vkb::DescriptorUpdater

Each of these structs are fully self-contained, that is, they hold all the data
required to construct the object. The pointer/size pairs have been replaced with
`std::vectors` so they can be appended to in-place, and you don't have to deal
with setting up the pointers and keeping track of the additional objects.

Additionally, each of these structs provide a `hash()` method so that it can be
placed into a map.

## Constructing Vulkan Objects

There are usually 3 method to create the object.

  * Provide a lambda function which has a single `const &` parameter of the original CreateInfo struct
  * Simply Provide it with the vk::Device
  * Provide the vk::Device and the `vkb::Storage&` struct.

```C++

vk::Device device = ....

// use the new DescriptorSetLayoutCreateInfo2.

vkb::DescriptorSetLayoutCreateInfo2 dsl;


dsl.addDescriptor( 0, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eVertex);
dsl.addDescriptor( 1, vk::DescriptorType::eStorageBuffer, 1, vk::ShaderStageFlagBits::eFragment);

// Use the create_t method, along with a lambda to get the original CreateInfo struct
auto layout = dsl.create_t( [device](vk::DescriptorSetLayoutCreateInfo & d
{
    auto l = device.createDescriptorSetLayout(d);

    // do something else with l if you want

    return l;
});


// Use the create(vk::Device) method to to do the same as the above

auto layout2 = dsl.create( device );


```

## No need to Manage DescriptorSetLayouts for Pipeline/PipelineLayouts

A pipeline requires knowledge of the DescriptorSets its going to use via the
PipelineLayout. This can be cumbersome to manage both the PipelineLayout as
well as the DescriptorSetLayouts because these are external objects.

Because the CreateInfo structs can now be hashed, we can keep track of objects
already created with the same CreateInfo structs and retrieve those instead of
creating new ones.

When creating a PipelineLayout, instead of creating the DescriptorSetLayouts
first, we can simply give the PipelineLayoutCreateInfo2 struct the
DescriptorSetLayoutCreateInfo2 objects and it will create the layouts for you.

To use this method, we need to use the `vkb::Storage` objects. This is simply a
container of maps which store the objects created. When we pass this storage
object into the create( ) method, it will look up the hash in the storage
container first before creating a new one.

```C++

vk::Device device ...;


// a storage container to store created objects by their hash
vkb::Storage storage;

vkb::PipelineLayoutCreateInfo2 PLC;

// 4 samplers for the first set on binding 0
PLC.newDescriptorSet().addDescriptor( 0, vk::DescriptorType::eCombinedImageSampler, 4, vk::ShaderStageFlagBits::eVertex);

// 1 storage buffer for the second set on binding 0
PLC.newDescriptorSet().addDescriptor( 0, vk::DescriptorType::eStorageBuffer, 4, vk::ShaderStageFlagBits::eVertex);

// Create the actual pipeline layout
auto pipelineLayout = PLC.create(storage, device);

assert( storage.descriptorSetLayouts.size() == 2);
assert( storage.pipelineLayouts.size() == 1);



// Creating the pipeline layout again, will not return a new object
// but instead return the same one! Same with the descriptor sets.
auto pipelineLayout2 = PLC.create(storage, device);

assert( pipelineLayout2 == pipelineLayout);

assert( storage.descriptorSetLayouts.size() == 2);
assert( storage.pipelineLayouts.size() == 2);

// Destroy the pipelineLayout and remove
// the data from the storage area. This does not
// destroy the descriptorSetLayouts
storage.destroy( pipelineLayout2, device);

// Destroy all objects currently held in the storage container.
storage.destroyAll(device);

```


This also works the vk::GraphicsPipelineCreateInfo2, You can provide in the
RenderPassCreateInfo2 and the PipelineLayoutCreateInfo2 structs and it will
create the RenderPass and the PipelineLayouts/DescriptorSetLayouts for you (if
they haven't already been created in the storage area).

Additionally, when providing the individual shader stages. You can just provide
it the location to the spv files, and it will create the shader modules.

The shader module's source code is hashed as well, so if you use the same code
twice, it will not create a new module.

```C++

    vk::Device device ...;

    // a storage container to store created objects by their hash
    vkb::Storage storage;


    vkb::GraphicsPipelineCreateInfo2 PCI;

    // Viewports
    PCI.viewportState.viewports.emplace_back( vk::Viewport(0,0,1024,768,0,1.0f));
    PCI.viewportState.scissors.emplace_back( vk::Rect2D( {0,0}, {1024,768}));

    // vertex inputs
    uint32_t stride=0+12+24;
    PCI.setVertexInputAttribute(0,0,vk::Format::eR32G32B32Sfloat,0 );
    PCI.setVertexInputAttribute(1,1,vk::Format::eR32G32B32Sfloat,12);
    PCI.setVertexInputAttribute(2,2,vk::Format::eR8G8B8A8Unorm  ,24);

    PCI.setVertexInputBinding(0,stride, vk::VertexInputRate::eVertex);
    PCI.setVertexInputBinding(1,stride, vk::VertexInputRate::eVertex);
    PCI.setVertexInputBinding(2,stride, vk::VertexInputRate::eVertex);

    // shader stages
    PCI.addStage( vk::ShaderStageFlagBits::eVertex,   "main", CMAKE_SOURCE_DIR "/share/shaders/vert.spv");
    PCI.addStage( vk::ShaderStageFlagBits::eFragment, "main", CMAKE_SOURCE_DIR "/share/shaders/frag.spv");

    // Render Pass - we will initialize the pipeline by providing the RenderPassCreateInfo2 struct
    //               and have it auto generate the renderpass for us.
    //               PCI.rendeRenderPass is a variant, you can either provide it
    //               with a vk::RenderPass or a vkb::RenderPassCreateInfo2
    PCI.renderPass = vkb::RenderPassCreateInfo2::defaultSwapchainRenderPass( {{ swapchainFormat, vk::ImageLayout::ePresentSrcKHR}});

    // Descriptor sets/Push constants
    PCI.addPushConstantRange(vk::ShaderStageFlagBits::eVertex, 0, 128);

    // single attachment (Swapchain), use default values
    // setColorBlendOp and setBlendEnabled are just
    /// being added for show, these are teh default values
    PCI.addBlendStateAttachment().setColorBlendOp(vk::BlendOp::eAdd)
                                 .setBlendEnable(true);


    auto pipeline = PCI.create( storage, device );




    // Destroy the pipeline and remove
    // the data from the storage area. This does not
    // destroy the pipelineLayouts/descriptorSetLayouts/renderPasses/ShaderModules
    storage.destroy( pipeline, device);

    // Destroy all objects currently held in the storage container.
    storage.destroyAll(device);


```


## Storage

Any objects created using the `.create(vkb::Storage&, vulkanObject)`  method,
will also store its CreateInfo struct in the storage.  That way you can query
how the object was created. For example, you can use this to determine what the
vertex input data is for a particular pipeline.

```C++
vkb::Storage S;

vkb::XXXXCreateInfo2 C;

auto l = C.create(S, device);

auto & S.getCreateInfo(l);

```

Note that the the following equality is not always true.
`S.getCreateInfo(l).hash() == C.hash()`. This is because certain objects which
depend on other vulkan objects, such as PipelineLayouts, which depend on
DescriptorSetLayouts, can store the construction information either as a vector
of `vk::DescriptorSetLayout`, or a vector of
`vkb::DescriptorSetLayoutCreateInfo2`. When the .create(...) method is called,
if the struct contains DescriptorSetLayoutCreateInfo2, those will be converted
into vk::DescriptorSetLayouts and then hashed in the Storage.
