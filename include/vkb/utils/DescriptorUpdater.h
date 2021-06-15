#ifndef VKB_DESCRIPTORSET_UPDATER_2_H
#define VKB_DESCRIPTORSET_UPDATER_2_H

#include <vulkan/vulkan.hpp>
#include <any>

namespace vkb
{

struct DescriptorSetUpdater2
{
protected:
    std::vector< vk::WriteDescriptorSet > write_sets;
    std::vector< std::any > vectorStorage;

    template<typename T>
    std::vector<T>& getDescriptorInfo()
    {
        auto & x = vectorStorage.emplace_back();
        return x.emplace< std::vector<T> >();
    }

public:
    /**
     * @brief updateImageDescriptor
     * @param set
     * @param binding
     * @param arrayIndex
     * @param imageDescriptorType
     * @param imgInfo
     *
     * Update a single image array element in a descriptor binding.
     *
     * eg:
     *   updateImageDescriptor( set, 0, 0, vk::DescriptorType::eCombinedImageSampler, { sampler, view} );
     *
     * note: that imgInfo contains 3 elements: sampler, view and image layout. if image layout is set
     * to its default value, it will be converted into shaderReadOnlyOptimal
     */
    void updateImageDescriptor( vk::DescriptorSet       set,
                                uint32_t                binding,
                                uint32_t                arrayIndex,
                                vk::DescriptorType      imageDescriptorType,
                                vk::DescriptorImageInfo imgInfo )
    {
        auto & ifn =  getDescriptorInfo<vk::DescriptorImageInfo>();

        if(imgInfo.imageLayout == vk::ImageLayout::eUndefined)
            imgInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

        ifn.emplace_back( imgInfo);


        write_sets.emplace_back().setDstSet(set)
                                 .setDstBinding(binding)
                                 .setPImageInfo( &ifn.back())
                                 .setDescriptorType( imageDescriptorType )
                                 .setDescriptorCount( 1 )
                                 .setDstArrayElement(arrayIndex);
    }
    void updateImageDescriptor( vk::DescriptorSet       set,
                                uint32_t                binding,
                                uint32_t                arrayIndex,
                                vk::DescriptorType      imageDescriptorType,
                                vk::ArrayProxy<vk::DescriptorImageInfo const> imgInfo_v )
    {
        auto & ifn =  getDescriptorInfo<vk::DescriptorImageInfo>();

        for(auto & v : imgInfo_v)
        {
            auto & imgInfo = ifn.emplace_back(v);
            if(imgInfo.imageLayout == vk::ImageLayout::eUndefined)
                imgInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        }

        write_sets.emplace_back().setDstSet(set)
                                 .setDstBinding(binding)
                                 .setPImageInfo(ifn.data())
                                 .setDescriptorType( imageDescriptorType )
                                 .setDescriptorCount( static_cast<uint32_t>(imgInfo_v.size()) )
                                 .setDstArrayElement(arrayIndex);
    }


    void updateBufferDescriptor( vk::DescriptorSet  set,
                                 uint32_t           binding,
                                 uint32_t           arrayIndex,
                                 vk::DescriptorType bufferDescriptorType,
                                 vk::ArrayProxy< const vk::DescriptorBufferInfo > buffer_offset_range )
    {
        auto & ifn =  getDescriptorInfo<vk::DescriptorBufferInfo>();

        for(auto & v : buffer_offset_range)
        {
            auto & imgInfo = ifn.emplace_back(v);
            if(imgInfo.range == 0)
                imgInfo.range = VK_WHOLE_SIZE;
        }

        write_sets.emplace_back().setDstSet(set)
                                 .setDstBinding(binding)
                                 .setPBufferInfo(ifn.data())
                                 .setDescriptorType(bufferDescriptorType)
                                 .setDescriptorCount( static_cast<uint32_t>(ifn.size()) )
                                 .setDstArrayElement(arrayIndex);

    }

    template<typename Callable_t>
    void create_t(Callable_t && CC) const
    {
        CC(write_sets);
    }

    void update(vk::Device d)
    {
        d.updateDescriptorSets(write_sets, nullptr);
        vectorStorage.clear();
    }
};


struct DescriptorSetUpdater3
{
protected:
    std::vector< vk::WriteDescriptorSet > write_sets;
    std::vector< std::any > vectorStorage;

    template<typename T>
    std::vector<T>& getDescriptorInfo()
    {
        auto & x = vectorStorage.emplace_back();
        return x.emplace< std::vector<T> >();
    }

public:
    /**
     * @brief updateImageDescriptor
     * @param set
     * @param binding
     * @param arrayIndex
     * @param imageDescriptorType
     * @param imgInfo
     *
     * Update a single image array element in a descriptor binding.
     *
     * eg:
     *   updateImageDescriptor( set, 0, 0, vk::DescriptorType::eCombinedImageSampler, { sampler, view} );
     *
     * note: that imgInfo contains 3 elements: sampler, view and image layout. if image layout is set
     * to its default value, it will be converted into shaderReadOnlyOptimal
     */
    void updateImageDescriptor( vk::DescriptorSet       set,
                                uint32_t                binding,
                                uint32_t                arrayIndex,
                                vk::DescriptorType      imageDescriptorType,
                                vk::DescriptorImageInfo imgInfo )
    {
        auto & ifn =  getDescriptorInfo<vk::DescriptorImageInfo>();

        if(imgInfo.imageLayout == vk::ImageLayout::eUndefined)
            imgInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

        ifn.emplace_back( imgInfo);


        write_sets.emplace_back().setDstSet(set)
                                 .setDstBinding(binding)
                                 .setPImageInfo( &ifn.back())
                                 .setDescriptorType( imageDescriptorType )
                                 .setDescriptorCount( 1 )
                                 .setDstArrayElement(arrayIndex);
    }
    void updateImageDescriptor( vk::DescriptorSet       set,
                                uint32_t                binding,
                                uint32_t                arrayIndex,
                                vk::DescriptorType      imageDescriptorType,
                                vk::ArrayProxy<vk::DescriptorImageInfo const> imgInfo_v )
    {
        auto & ifn =  getDescriptorInfo<vk::DescriptorImageInfo>();

        for(auto & v : imgInfo_v)
        {
            auto & imgInfo = ifn.emplace_back(v);
            if(imgInfo.imageLayout == vk::ImageLayout::eUndefined)
                imgInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        }

        write_sets.emplace_back().setDstSet(set)
                                 .setDstBinding(binding)
                                 .setPImageInfo(ifn.data())
                                 .setDescriptorType( imageDescriptorType )
                                 .setDescriptorCount( static_cast<uint32_t>(imgInfo_v.size()) )
                                 .setDstArrayElement(arrayIndex);
    }


    void updateBufferDescriptor( vk::DescriptorSet  set,
                                 uint32_t           binding,
                                 uint32_t           arrayIndex,
                                 vk::DescriptorType bufferDescriptorType,
                                 vk::ArrayProxy< const vk::DescriptorBufferInfo > buffer_offset_range )
    {
        auto & ifn =  getDescriptorInfo<vk::DescriptorBufferInfo>();

        for(auto & v : buffer_offset_range)
        {
            auto & imgInfo = ifn.emplace_back(v);
            if(imgInfo.range == 0)
                imgInfo.range = VK_WHOLE_SIZE;
        }

        write_sets.emplace_back().setDstSet(set)
                                 .setDstBinding(binding)
                                 .setPBufferInfo(ifn.data())
                                 .setDescriptorType(bufferDescriptorType)
                                 .setDescriptorCount( static_cast<uint32_t>(ifn.size()) )
                                 .setDstArrayElement(arrayIndex);

    }

    template<typename Callable_t>
    void create_t(Callable_t && CC) const
    {


        CC(write_sets);
    }

    void update(vk::Device d)
    {
        d.updateDescriptorSets(write_sets, nullptr);
        vectorStorage.clear();
    }
};


}

#endif
