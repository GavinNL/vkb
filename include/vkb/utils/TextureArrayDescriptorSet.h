#ifndef VKB_TEXTURE_ARRAY_DESCRIPTOR_SET
#define VKB_TEXTURE_ARRAY_DESCRIPTOR_SET

#include "../detail/DescriptorUpdater.h"
#include "../detail/Storage.h"
#include "../detail/DescriptorSetLayoutCreateInfo2.h"
#include "../detail/DescriptorPoolCreateInfo2.h"
#include <set>

namespace vkb
{

struct TextureArrayDescriptorSetCreateInfo
{
    size_t             textureCount;
    vk::DescriptorSet  descriptorSet;
    uint32_t           binding;

    // on creation, all textures in the set
    // will point to this value. It MUST always be valid
    // a simple white texture would sufffice.
    vk::Sampler     defaultSampler;
    vk::ImageView   defaultImageView;
    vk::ImageLayout defaultImageLayout;
};

/**
 * @brief The TextureArrayDescriptorSet struct
 *
 * This container is basically an array of textures, which
 * you can use to update.
 *
 * Since an array-of-textures descriptor must have all
 * textures bound, it requires a default image that
 * must initially be bound to every element.
 * A simple blank white texture would suffice.
 *
 * This container was meant to be used to create a
 * global texture array. That is to keep all images
 * that the frame is going to use bound to the
 * set.
 */
struct TextureArrayDescriptorSet
{
    struct image_t
    {
        vk::Sampler     sampler;
        vk::ImageView   view;
        vk::ImageLayout layout;
    };

    vk::DescriptorSet    m_DescriptorSet;
    uint32_t             m_binding;
    std::vector<image_t> m_images;
    std::set<size_t>     m_dirty;
    image_t              m_default;

    struct hash_
    {
        size_t operator()(vk::ImageView v) const
        {
            std::hash<void*> H;
            return H(static_cast<void*>(v) );
        }
    };

    std::unordered_map<vk::ImageView, size_t, hash_> m_viewToIndex;
    std::vector<size_t>     m_available;

    void create(TextureArrayDescriptorSetCreateInfo const & C)
    {
        m_binding = C.binding;
        m_DescriptorSet = C.descriptorSet;

        m_default = { C.defaultSampler, C.defaultImageView, C.defaultImageLayout };
        m_images.insert( m_images.end(), C.textureCount, m_default);
        for(size_t i=0;i<m_images.size();i++)
        {
            m_available.push_back(i);
        }
        setDirty();
    }

    /**
     * @brief operator []
     * @param i
     * @return
     *
     * Access a particular element, you can update the values
     * but you must set that index to be dirty for it to
     * actually update.
     *
     * arr[3].view = ...;
     * arr.setDirty(3);
     */
    image_t& operator[](size_t i)
    {
        return m_images[i];
    }
    /**
     * @brief setDirty
     * @param i
     *
     * Sets a particular index in the array to be dirty.
     * This index will be updated when the updater is called.
     */
    void setDirty(size_t i)
    {
        m_dirty.insert(i);
    }
    /**
     * @brief setDirty
     * Sets the entire array to be dirty
     */
    void setDirty()
    {
        for(size_t i=0;i<m_images.size();i++)
        {
            m_dirty.insert(i);
        }
    }

    bool isDirty(size_t i) const
    {
        return m_dirty.count(i);
    }
    bool isDirty() const
    {
        return m_dirty.size()>0;
    }
    size_t dirtyCount() const
    {
        return m_dirty.size();
    }
    /**
     * @brief updateDirty
     * @param updater
     *
     * Update all the textures that have been marked dirty.
     */
    void update(vkb::DescriptorSetUpdater & updater )
    {
        for(auto & u : m_dirty)
        {
            std::tuple<vk::Sampler, vk::ImageView, vk::ImageLayout>  up= {m_images[u].sampler, m_images[u].view, m_images[u].layout};
            updater.updateImageDescriptor( m_DescriptorSet, m_binding, static_cast<uint32_t>(u), vk::DescriptorType::eCombinedImageSampler, up );
        }
        m_dirty.clear();
    }

    int32_t findTexture(vk::ImageView v) const
    {
        auto f = m_viewToIndex.find(v);
        if( f == m_viewToIndex.end() )
            return -1;
        return static_cast<int32_t>(f->second);
    }

    size_t insertTexture(vk::ImageView v)
    {
        if( m_viewToIndex.count(v) == 0)
        {
            size_t index = _nextIndex();
            if( index > m_images.size() )
                return index;

            m_images.at(index).view = v;
            m_viewToIndex[v] = index;
            setDirty(index);
            return index;
        }
        else
        {
            return m_viewToIndex[v];
        }
    }

    int32_t removeTexture(vk::ImageView v)
    {
        if( v == m_default.view) // cannot remove the default
            return -1;

        int32_t out=-1;
        auto it = m_viewToIndex.find(v);
        if( it != m_viewToIndex.end() )
        {
            m_images[ it->second ] = m_default;
            setDirty( it->second );
            out = static_cast<int32_t>(it->second);
            _freeIndex( it->second );
            m_viewToIndex.erase(it);
        }
        return out;
    }

protected:
    size_t _nextIndex()
    {
        if( m_available.size() ==0) return std::numeric_limits<size_t>::max();
        size_t index = m_available.back();
        m_available.pop_back();
        return index;
    }
    void _freeIndex(size_t i)
    {
        m_available.push_back(i);
    }
};


struct TextureArrayDescriptorSetChainCreateInfo
{
    uint32_t chainSize = 0;

    // on creation, all textures in the set
    // will point to this value. It MUST always be valid
    // a simple white texture would sufffice.
    uint32_t textureBinding = 0;
    uint32_t textureCount   = 0;
    vk::Sampler     defaultSampler;
    vk::ImageView   defaultImageView;
    vk::ImageLayout defaultImageLayout;

    //uint32_t textureCubeBinding = 0;
    //uint32_t textureCubeCount   = 0;
    //vk::Sampler     defaultImageCubeSampler;
    //vk::ImageView   defaultImageCubeView;
    //vk::ImageLayout defaultImageCubeLayout;
};

/**
 * @brief The TextureArrayDescriptorSetChain struct
 *
 * This class acts similarly to a Swapchain, but
 * for descriptor sets of TextureArrays.
 */
struct TextureArrayDescriptorSetChain
{
    vk::DescriptorSetLayout                     m_layout;
    vk::DescriptorPool                          m_pool;
    std::vector<vkb::TextureArrayDescriptorSet> m_TextureArrayChain;
    std::vector<vkb::TextureArrayDescriptorSet> m_TextureCubeArrayChain;

    void create( vk::Device device,
                 vkb::Storage & storage,
                 TextureArrayDescriptorSetChainCreateInfo const & C)
    {
        auto textureCount     = C.textureCount;

        //=================================================================
        // Create a single descriptor set layout which will
        // it will host MAX_TEXTURES_BOUND images
        vkb::DescriptorSetLayoutCreateInfo2 L;
        L.addDescriptor(C.textureBinding,
                        vk::DescriptorType::eCombinedImageSampler,
                        textureCount,
                        vk::ShaderStageFlagBits::eFragment);

        m_layout = L.create(storage, device);
        //=================================================================

        {
            uint32_t maxSets = C.chainSize;
            // Initialize the descriptor pool
            vkb::DescriptorPoolCreateInfo2 pI;

            pI.setPoolSize( vk::DescriptorType::eCombinedImageSampler, (textureCount) * maxSets );
            pI.setMaxSets(maxSets);

            m_pool = pI.create( storage, device);
        }

        m_TextureArrayChain.resize(C.chainSize);
        for(auto & t : m_TextureArrayChain)
        {
            vk::DescriptorSetAllocateInfo a;
            a.setPSetLayouts( &m_layout);
            a.setDescriptorSetCount(1);
            a.setDescriptorPool(m_pool);

            vkb::TextureArrayDescriptorSetCreateInfo c;
            c.textureCount       = textureCount;
            c.descriptorSet      = device.allocateDescriptorSets(a).front();
            c.binding            = C.textureBinding;
            c.defaultImageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
            c.defaultSampler     = C.defaultSampler;
            c.defaultImageView   = C.defaultImageView;
            t.create(c);
        }
    }


    void destroy( vk::Device device ,vkb::Storage & storage)
    {
        storage.destroy(m_pool, device);
    }
    int32_t removeTexture( vk::ImageView v, vkb::DescriptorSetUpdater & updater)
    {
        int32_t s = -1;
        for(auto & t : m_TextureArrayChain)
        {
            s = t.removeTexture( v );
            t.update(updater);
        }
        return s;
    }
    int32_t insertTexture( vk::ImageView v)
    {
        int32_t s =0;
        for(auto & t : m_TextureArrayChain)
        {
            s = static_cast<int32_t>( t.insertTexture( v ) );
        }
        return s;
    }

    size_t update( DescriptorSetUpdater & updater)
    {
        auto c = currentArray().dirtyCount();
        currentArray().update(updater);
        return c;
    }
    void nextArray()
    {
        m_currentIndex++;
        m_currentIndex = m_currentIndex % m_TextureArrayChain.size();
    }

    vk::DescriptorSet currentDescriptorSet() const
    {
        return currentArray().m_DescriptorSet;
    }
    vkb::TextureArrayDescriptorSet & currentArray()
    {
        return m_TextureArrayChain.at(m_currentIndex);
    }
    vkb::TextureArrayDescriptorSet const & currentArray() const
    {
        return m_TextureArrayChain.at(m_currentIndex);
    }

    int32_t findTexture(vk::ImageView v) const
    {
        return currentArray().findTexture(v);
    }
private:
    size_t m_currentIndex=0;
};

}

#endif
