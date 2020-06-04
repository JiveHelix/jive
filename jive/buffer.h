/**
  * @file buffer.h
  *
  * @brief A sized region of memory that is never default initialized.
  *
  * Useful to create a managed and sized buffer when you don't want to pay for
  * default initialization. This is similar to a std::vector with a custom
  * allocator that implements a call to construct like;
  *
    template<typename U>
    void construct(U *p)
    {
        // p will remain in uninitialized state.
        ::new (p) U;

        // The default behavior is equivalent to:
        // ::new (p) U();
        // where p is zero-initialized.
    }
  *
  * The advantage of Buffer's approach is there is no overhead to call
  * construct for each member of the buffer.
  *
  * For non-trivial types, this class offers no advantages, so these have been
  * disabled.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 07 May 2020
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <type_traits>

namespace jive
{

namespace detail
{

template<typename T, size_t align>
size_t GetAlignedByteCount(size_t count)
{
    auto byteCount = count * sizeof(T);
    auto error = byteCount % align;

    return (error == 0)
        ? byteCount
        : byteCount + (align - error);
}

} // end namespace detail


inline constexpr auto minimumAlign = sizeof(void *);


template<
    typename T,
    size_t align_ = (alignof(T) < minimumAlign) ? minimumAlign : alignof(T)>
class Buffer
{
public:
    static_assert(std::is_trivial_v<T>);
    static_assert(align_ > 0);

    // align_ must be a multiple of the pointer size
    static_assert(align_ % sizeof(void *) == 0);

    // align_ must be a power of 2.
    static_assert((align_ & (align_ - 1)) == 0);

    static constexpr auto align = align_;

    using type = T;

    Buffer(size_t elementCount)
        :
        byteCount_(detail::GetAlignedByteCount<T, align>(elementCount)),
        elementCount_(elementCount),
        data_(
            static_cast<T *>(
                ::operator new(this->byteCount_, std::align_val_t{align})))
    {

    }

    template<typename InputStream>
    static Buffer FromStream(size_t elementCount, InputStream &inputStream)
    {
        Buffer buffer(elementCount);

        inputStream.read(
            reinterpret_cast<char *>(buffer.Get()),
            static_cast<std::streamsize>(elementCount * sizeof(T)));

        return buffer;
    }

    ~Buffer()
    {
        std::free(this->data_);
    }

    Buffer(const Buffer &) = delete;

    Buffer(Buffer &&other) noexcept
        :
        elementCount_(other.elementCount_),
        data_(other.data_)
    {
        other.data_ = nullptr;
    }

    size_t GetElementCount() const { return this->elementCount_; }

    size_t GetByteCount() const { return this->elementCount_ * sizeof(T); }

    size_t GetAllocatedByteCount() const { return this->byteCount_; }

    const T * Get() const { return this->data_; }

    T * Get() { return this->data_; }

private:
    size_t byteCount_;
    size_t elementCount_;
    T *data_;
};


} // end namespace jive
