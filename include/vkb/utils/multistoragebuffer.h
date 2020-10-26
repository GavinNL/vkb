#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iterator>

namespace vkb
{
/**
 * @brief The StorageDescriptor_t struct
 *
 * The StorageDescriptor is mean to be used
 * to store large amounts of similar data, for example
 * matrices. For example, In your shader you can
 * have something like this:
 *
 *
 * layout(set = 0, binding = 1) buffer STORAGE_MATRIX_t
 * {
 *     mat4 transform[];
 * } STORAGE_MATRIX;
 *
 * You would then create this on the host by
 *
 * StorageDescriptor_t<glm::mat4> storageDescriptor;
 *
 * storageDescriptor( mySubbuffer, 0 ,mySubbuffer.getSize() );
 *
 * This storageDescriptor can then be passed to the various
 * renderers
 */
struct MultiStorageBuffer
{
protected:
    uint8_t *  m_first = nullptr;
    uint8_t *  m_last  = nullptr;
    uint8_t *  m_end  = nullptr;
public:
    MultiStorageBuffer()
    {
    }

    void init(void * data, size_t byteSize)
    {
        m_first = static_cast<uint8_t*>(data);
        m_end   = m_first;
        m_last  = m_first + byteSize;
    }

    void reset()
    {
        m_end = m_first;
    }

    // returns the number of bytes available until the buffer
    // rolls over.
    size_t available() const
    {
        return static_cast<size_t>( std::distance(m_end, m_last) );
    }
    size_t capacity() const
    {
        return static_cast<size_t>( std::distance(m_first, m_last) );
    }
    size_t size() const
    {
        return static_cast<size_t>( std::distance(m_first, m_end) );
    }
    /**
     * @brief push
     * @param v
     * @return
     *
     * Push data into the buffer. The value will always be aligned to
     * sizeof(T).
     *
     * Ths function runs the array offset if the buffer was
     * representing a an array of T[x].
     *
     *  For example, given an empty storage, M:
     *
     * M.push(glm::mat4() ) == 0 (since it is the first element)
     * M.push(glm::mat4() ) == 1 (since it is now the second element in mat4[]
     * M.push( float() )    == 32 (since the float was pushed after the 2 mat4(), it exists at
     *                             byte offset 2*sizeof(mat4)==128.
     *                             If the underlying buffer was an array of float[], then byte 128
     *                             would be at array index 32.
     *
     *
     */
    template<typename T>
    int32_t push(T const & v)
    {
        static_assert(  !std::is_pointer<T>::value, "Cannot use a pointer");
        return push(&v, 1);
    }
    template<typename T>
    int32_t push(T const * v, size_t count)
    {
        auto alignment = sizeof(T);
        auto totalBytes= sizeof(T)*count;

        return push(v, totalBytes, alignment);
    }

    int32_t push(void const * v, size_t totalBytes, size_t alignment)
    {
        if( available() < totalBytes )
        {
            reset();
        }
        // move the current end point to a byte offset that aligns with alignment
        auto i = roundUp(alignment);

        std::memcpy( m_end, v, totalBytes );
        m_end += totalBytes;

        return static_cast<int32_t>( i / alignment );
    }

private:
    // round up the current offset to the next byte alignment
    size_t roundUp(size_t alignment)
    {
        auto _roundUp = [](auto N, auto S)
        {
            return ((((N) + (S) - 1) / (S)) * (S));
        };
        m_end = m_first + _roundUp( size(), alignment);

        if(m_end >= m_last)
        {
            reset();
        }

        return size();
    }

};

}
