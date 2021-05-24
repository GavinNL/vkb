#ifndef VKB_WRITEDESCRIPTORSET2_H
#define VKB_WRITEDESCRIPTORSET2_H

#include <vulkan/vulkan.hpp>
#include <functional>
#include <variant>
#include <any>
#include "HashFunctions.h"


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
                                 .setPImageInfo(ifn.data())
                                 .setDescriptorType( imageDescriptorType )
                                 .setDescriptorCount( static_cast<uint32_t>(ifn.size()) )
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
                                 .setDescriptorCount( static_cast<uint32_t>(ifn.size()) )
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

/**
 * @brief The DescriptorSetUpdater struct
 *
 * Update multiple descriptor sets/bindings using a single
 * struct.
 *
 *  DescriptorSetUpdater updater;
 *
 * updater.updateCombinedImageSampler( set1,
 *                                     binding1,
 *                                     0,
 *                                     {
 *                                          { sampler1, imageview1, layout1},
 *                                          { sampler2, imageview2, layout2},
 *                                          { sampler3, imageview3, layout3},
 *                                          { sampler4, imageview4, layout4}
 *                                     } );
 *
 * updater.updateUniformbuffer( set2,
 *                                     binding2,
 *                                     1,
 *                                     {
 *                                          { buffer, 0, size}
 *                                     } );
 *
 * // updates the following:
 * // set1, binding1, array indices: 0,1,2,3
 * // set2, binding2, array indices: 0
 * updater.create_t([](auto & vectorOfWrites)
 * {
 *      device.updateDescriptorSets(vectorOfWrites, nullptr);
 * });
 */
struct DescriptorSetUpdater
{
    using info_type =     std::variant< std::vector <vk::DescriptorBufferInfo>, std::vector< vk::DescriptorImageInfo> , std::vector< vk::BufferView>>;

    std::vector< vk::WriteDescriptorSet > write_sets;
    std::vector< info_type > infos2;

    std::vector< std::any > vectorStorage;

    template<typename T>
    std::vector<T>& getDescriptorInfo()
    {
        auto & x = vectorStorage.emplace_back();
        return x.emplace< std::vector<T> >();
    }

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
    void updateImageDescriptor( vk::DescriptorSet  set,
                                uint32_t           binding,
                                uint32_t           arrayIndex,
                                vk::DescriptorType imageDescriptorType,
                                vk::DescriptorImageInfo imgInfo )
    {
        //std::vector< vk::DescriptorImageInfo> ifn;
        auto & ifn =  getDescriptorInfo<vk::DescriptorImageInfo>();//std::get< std::vector< vk::DescriptorImageInfo> >( infos2.emplace_back( std::move(ifn)) );

        if(imgInfo.imageLayout == vk::ImageLayout::eUndefined)
            imgInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

        ifn.emplace_back( imgInfo);


        write_sets.emplace_back().setDstSet(set)
                                 .setDstBinding(binding)
                                 .setPImageInfo(ifn.data())
                                 .setDescriptorType( imageDescriptorType )
                                 .setDescriptorCount( static_cast<uint32_t>(ifn.size()) )
                                 .setDstArrayElement(arrayIndex);
    }

    void updateImageDescriptor( vk::DescriptorSet set,
                      uint32_t binding,
                      uint32_t arrayIndex,
                      vk::DescriptorType imageDescriptorType,
                      vk::ArrayProxy< const std::tuple<vk::Sampler, vk::ImageView, vk::ImageLayout> > l )
    {
        std::vector< vk::DescriptorImageInfo> ifn;

        for(auto & i : l)
        {
            ifn.emplace_back( std::get<0>(i), std::get<1>(i), std::get<2>(i));
        }

        auto & b = std::get< std::vector< vk::DescriptorImageInfo> >( infos2.emplace_back( std::move(ifn)) );

        write_sets.emplace_back().setDstSet(set)
                                 .setDstBinding(binding)
                                 .setPImageInfo(b.data())
                                 .setDescriptorType( imageDescriptorType )
                                 .setDescriptorCount( static_cast<uint32_t>(b.size()) )
                                 .setDstArrayElement(arrayIndex);

    }

    void updateBufferDescriptor( vk::DescriptorSet set,
                      uint32_t binding,
                      uint32_t arrayIndex,
                      vk::DescriptorType bufferDescriptorType,
                      vk::ArrayProxy< const std::tuple<vk::Buffer, vk::DeviceSize, vk::DeviceSize> > buffer_offset_range )
    {
        std::vector< vk::DescriptorBufferInfo> ifn;

        for(auto & i : buffer_offset_range)
        {
            ifn.emplace_back( std::get<0>(i), std::get<1>(i), std::get<2>(i));
        }

        auto & b = std::get< std::vector< vk::DescriptorBufferInfo> >( infos2.emplace_back( std::move(ifn)) );

        write_sets.emplace_back().setDstSet(set)
                                 .setDstBinding(binding)
                                 .setPBufferInfo(b.data())
                                 .setDescriptorType(bufferDescriptorType)
                                 .setDescriptorCount( static_cast<uint32_t>(b.size()) )
                                 .setDstArrayElement(arrayIndex);

    }

    void updateTexelBufferDescriptor( vk::DescriptorSet set,
                      uint32_t binding,
                      uint32_t arrayIndex,
                      vk::DescriptorType bufferDescriptorType,
                      vk::ArrayProxy< const vk::BufferView> texelBufferViews )
    {
        std::vector< vk::BufferView> ifn;

        for(auto & i : texelBufferViews)
        {
            ifn.emplace_back( i);
        }

        auto & b = std::get< std::vector< vk::BufferView> >( infos2.emplace_back( std::move(ifn)) );

        write_sets.emplace_back().setDstSet(set)
                                 .setDstBinding(binding)
                                 .setPTexelBufferView(b.data())
                                 .setDescriptorType(bufferDescriptorType)
                                 .setDescriptorCount( static_cast<uint32_t>(b.size()) )
                                 .setDstArrayElement(arrayIndex);

    }

    template<typename Callable_t>
    void create_t(Callable_t && CC) const
    {
        CC(write_sets);
    }

    void update(vk::Device d) const
    {
        create_t( [d](auto & t)
        {
            d.updateDescriptorSets(t, nullptr);
        });
    }

};
}

#endif
