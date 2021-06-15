#ifndef VKB_WRITE_DESCRIPTOR_SET2_H
#define VKB_WRITE_DESCRIPTOR_SET2_H

#include <vulkan/vulkan.hpp>

namespace vkb
{

struct WriteDescriptorSet2
{
    VkDescriptorSet                     dstSet = VK_NULL_HANDLE;
    uint32_t                            dstBinding = 0;
    uint32_t                            dstArrayElement = 0;
    VkDescriptorType                    descriptorType;
    std::vector<VkDescriptorImageInfo>  ImageInfo;
    std::vector<VkDescriptorBufferInfo> BufferInfo;
    std::vector<VkBufferView>           TexelBufferView;

    WriteDescriptorSet2& setBinding(uint32_t b)
    {
        dstBinding = b;
        return *this;
    }
    WriteDescriptorSet2& setDescriptorType(VkDescriptorType t)
    {
        descriptorType = t;
        return *this;
    }
    WriteDescriptorSet2& setDescriptorSet(VkDescriptorSet s)
    {
        dstSet = s;
        return *this;
    }
    WriteDescriptorSet2& setArrayElement(uint32_t s)
    {
        dstArrayElement = s;
        return *this;
    }
    WriteDescriptorSet2& setImageInfo(std::initializer_list<VkDescriptorImageInfo> img)
    {
        ImageInfo = img;
        return *this;
    }
    WriteDescriptorSet2& setImageInfo(std::vector<VkDescriptorImageInfo> img)
    {
        ImageInfo = img;
        return *this;
    }

    WriteDescriptorSet2& setBufferInfo(std::initializer_list<VkDescriptorBufferInfo> img)
    {
        BufferInfo = img;
        return *this;
    }
    WriteDescriptorSet2& setBufferInfo(std::vector<VkDescriptorBufferInfo> img)
    {
        BufferInfo = img;
        return *this;
    }
    void update(VkDevice device)
    {
        VkWriteDescriptorSet write = {};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet          = dstSet;
        write.dstBinding      = dstBinding;
        write.dstArrayElement = dstArrayElement;
        write.descriptorType  = descriptorType;

        write.pImageInfo       = nullptr;
        write.pBufferInfo      = nullptr;
        write.pTexelBufferView = nullptr;

        if( ImageInfo.size() )
        {
            write.descriptorCount = std::max( write.descriptorCount, static_cast<uint32_t>( ImageInfo.size() ) );
            write.pImageInfo       = ImageInfo.data();
        }
        else if( BufferInfo.size() )
        {
            write.descriptorCount = std::max( write.descriptorCount, static_cast<uint32_t>( BufferInfo.size() ) );
            write.pBufferInfo      = BufferInfo.data();
        }
        else if( TexelBufferView.size()  )
        {
            write.descriptorCount = std::max( write.descriptorCount, static_cast<uint32_t>( TexelBufferView.size() ) );
            write.pTexelBufferView = TexelBufferView.data();
        }

        vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);
    }

    VkWriteDescriptorSet generate() const
    {
        VkWriteDescriptorSet write = {};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet          = dstSet;
        write.dstBinding      = dstBinding;
        write.dstArrayElement = dstArrayElement;
        write.descriptorType  = descriptorType;

        write.pImageInfo       = nullptr;
        write.pBufferInfo      = nullptr;
        write.pTexelBufferView = nullptr;

        if( ImageInfo.size() )
        {
            write.descriptorCount = std::max( write.descriptorCount, static_cast<uint32_t>( ImageInfo.size() ) );
            write.pImageInfo       = ImageInfo.data();
        }
        else if( BufferInfo.size() )
        {
            write.descriptorCount = std::max( write.descriptorCount, static_cast<uint32_t>( BufferInfo.size() ) );
            write.pBufferInfo      = BufferInfo.data();
        }
        else if( TexelBufferView.size()  )
        {
            write.descriptorCount = std::max( write.descriptorCount, static_cast<uint32_t>( TexelBufferView.size() ) );
            write.pTexelBufferView = TexelBufferView.data();
        }
        return write;
    }
};


struct DescriptorSetWriter
{
    std::vector<WriteDescriptorSet2> writers;

    WriteDescriptorSet2& updateSet(VkDescriptorSet set, uint32_t binding, VkDescriptorType type)
    {
        auto & d = writers.emplace_back();
        d.dstSet = set;
        d.dstBinding = binding;
        d.descriptorType = type;
        return d;
    }

    DescriptorSetWriter& insertImageInfo(VkDescriptorImageInfo img)
    {
        writers.back().ImageInfo.push_back(img);
        return *this;
    }
    DescriptorSetWriter& insertBufferInfo(VkDescriptorBufferInfo img)
    {
        writers.back().BufferInfo.push_back(img);
        return *this;
    }
    void update(VkDevice device)
    {
        std::vector<VkWriteDescriptorSet> out;
        for(auto & w : writers)
        {
            out.push_back(w.generate());
        }
        vkUpdateDescriptorSets(device, static_cast<uint32_t>(out.size()), out.data(), 0, nullptr);
    }
};

}

#endif
