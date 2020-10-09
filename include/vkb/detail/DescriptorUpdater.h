#ifndef VKJSON_WRITEDESCRIPTORSET2_H
#define VKJSON_WRITEDESCRIPTORSET2_H

#include <vulkan/vulkan.hpp>
#include <functional>
#include <variant>
#include "HashFunctions.h"


namespace vkb
{


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
    using info_type =     std::variant< std::vector <vk::DescriptorBufferInfo>, std::vector< vk::DescriptorImageInfo> >;

    std::vector< vk::WriteDescriptorSet > write_sets;
    std::vector< info_type > infos2;


    void updateCombinedImageSampler( vk::DescriptorSet set,
                      uint32_t binding,
                      uint32_t arrayIndex,
                      std::vector< std::tuple<vk::Sampler, vk::ImageView, vk::ImageLayout> > l )
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
                                 .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                                 .setDescriptorCount( static_cast<uint32_t>(b.size()) )
                                 .setDstArrayElement(arrayIndex);

    }

    void updateUniformbuffer( vk::DescriptorSet set,
                      uint32_t binding,
                      uint32_t arrayIndex,
                      std::vector< std::tuple<vk::Buffer, vk::DeviceSize, vk::DeviceSize> > buffer_offset_range )
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
                                 .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                                 .setDescriptorCount( static_cast<uint32_t>(b.size()) )
                                 .setDstArrayElement(arrayIndex);

    }

    void updateStorageBuffer( vk::DescriptorSet set,
                      uint32_t binding,
                      uint32_t arrayIndex,
                      std::vector< std::tuple<vk::Buffer, vk::DeviceSize, vk::DeviceSize> > buffer_offset_range )
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
                                 .setDescriptorType(vk::DescriptorType::eStorageBuffer)
                                 .setDescriptorCount( static_cast<uint32_t>(b.size()) )
                                 .setDstArrayElement(arrayIndex);

    }

    template<typename Callable_t>
    void create_t(Callable_t && CC) const
    {
        CC(write_sets);
    }

};
}

#endif
