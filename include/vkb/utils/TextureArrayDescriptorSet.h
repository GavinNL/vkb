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
    // on creation, all textures in the set
    // will point to this value. It MUST always be valid
    // a simple white texture would sufffice.
    size_t          textureCount;
    uint32_t        binding;
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
    size_t update(vk::DescriptorSet set, vkb::DescriptorSetUpdater & updater)
    {
        auto c = m_dirty.size();
        for(auto & u : m_dirty)
        {
            std::tuple<vk::Sampler, vk::ImageView, vk::ImageLayout>  up= {m_images[u].sampler, m_images[u].view, m_images[u].layout};
            updater.updateImageDescriptor( set, m_binding, static_cast<uint32_t>(u), vk::DescriptorType::eCombinedImageSampler, up );
        }
        m_dirty.clear();
        return c;
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
    // the total number of descriptor sets
    // you should probably aim to have more than
    // your current swapchain count.
    uint32_t chainSize = 0;

    struct ArrayInfo
    {
        uint32_t        textureCount = 0;
        uint32_t        descriptorSetBinding = 0;
        vk::Sampler     defaultSampler;
        vk::ImageView   defaultImageView;
        vk::ImageLayout defaultImageLayout;
    };

    std::vector< ArrayInfo > bindings;
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
 *
 * This class keeps its own descriptor pool and
 * descriptor set layout.
 */
struct TextureArrayDescriptorSetChain
{

    std::vector< std::vector<vkb::TextureArrayDescriptorSet> > m_TextureArrayChain;
    std::vector<vk::DescriptorSet>              m_descriptorSets;

    void create( vk::Device device,
                 vkb::Storage & storage,
                 TextureArrayDescriptorSetChainCreateInfo const & C)
    {
        auto textureCount     = 0;//C.textureCount;

        //=================================================================
        // Create a single descriptor set layout which will
        // it will host MAX_TEXTURES_BOUND images
        vkb::DescriptorSetLayoutCreateInfo2 L;
        for(auto & T : C.bindings)
        {
            L.addDescriptor(T.descriptorSetBinding,
                            vk::DescriptorType::eCombinedImageSampler,
                            T.textureCount,
                            vk::ShaderStageFlagBits::eFragment);
            textureCount += T.textureCount;
        }

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


        m_TextureArrayChain.resize( C.chainSize );

        // for each chain.

        for(auto & T : m_TextureArrayChain)
        {
            vk::DescriptorSetAllocateInfo a;
            a.setPSetLayouts( &m_layout);
            a.setDescriptorSetCount( 1);
            a.setDescriptorPool(m_pool);

            m_descriptorSets.push_back( device.allocateDescriptorSets(a).front() );

            T.resize( C.bindings.size() );
            size_t i=0;
            for(auto & t : C.bindings)
            {
                vkb::TextureArrayDescriptorSetCreateInfo c;
                c.binding            = t.descriptorSetBinding;
                c.textureCount       = t.textureCount;
                c.defaultImageLayout = t.defaultImageLayout;
                c.defaultSampler     = t.defaultSampler;
                c.defaultImageView   = t.defaultImageView;

                T.at(i).create(c);
                ++i;
            }
        }

    }


    void destroy( vk::Device device ,vkb::Storage & storage)
    {
        storage.destroy(m_pool, device);
    }
    int32_t removeTexture( vk::ImageView v, uint32_t binding, vkb::DescriptorSetUpdater & updater)
    {
        int32_t s = -1;
        for(size_t i=0;i < m_TextureArrayChain.size();i++ )
        {
            s =  m_TextureArrayChain[i][binding].removeTexture(v);
            m_TextureArrayChain[i][binding].update(m_descriptorSets[i], updater);
        }
        return s;
    }
    int32_t insertTexture( vk::ImageView v, uint32_t binding)
    {
        int32_t s =0;
        for(auto & t : m_TextureArrayChain)
        {
            s = static_cast<int32_t>( t[binding].insertTexture( v ) );
        }
        return s;
    }

    size_t update( DescriptorSetUpdater & updater)
    {
        size_t c=0;

        // for all bindings
        for(auto & b : getCurrentArray())
        {
            c += b.update( getCurrentDescriptorSet(), updater);
        }
        return c;
    }

    void nextArray()
    {
        m_currentIndex++;
        m_currentIndex = m_currentIndex % m_TextureArrayChain.size();
    }

    vk::DescriptorSet getCurrentDescriptorSet() const
    {
        return m_descriptorSets[m_currentIndex];
    }
    std::vector< vkb::TextureArrayDescriptorSet> & getCurrentArray()
    {
        return m_TextureArrayChain.at(m_currentIndex);
    }
    std::vector< vkb::TextureArrayDescriptorSet> const & getCurrentArray() const
    {
        return m_TextureArrayChain.at(m_currentIndex);
    }

    int32_t findTexture(vk::ImageView v, uint32_t binding) const
    {
        return getCurrentArray()[binding].findTexture(v);
    }

    vk::DescriptorSetLayout getDescriptorSetLayout() const
    {
        return m_layout;
    }
private:
    vk::DescriptorSetLayout                     m_layout;
    vk::DescriptorPool                          m_pool;
    size_t m_currentIndex=0;
};

}

#endif
