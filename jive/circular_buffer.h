/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <cstdint>
#include <type_traits>
#include <ostream>
#include <iomanip>
#include "jive/circular_index.h"

namespace jive
{

namespace detail
{
    template<typename T>
    auto AsNumber(T value)
    {
        using type = std::remove_cv_t<std::remove_reference_t<T>>;

        if constexpr (sizeof(type) > 1)
        {
            return value;
        }
        else if constexpr (std::is_signed_v<type>)
        {
            return static_cast<int16_t>(value);
        }
        else
        {
            return static_cast<uint16_t>(value);
        }
    }
} // end namespace detail


template<typename T>
struct PromoteByte
{
    static_assert(sizeof(T) == 1);

    PromoteByte(T value_): value(value_) {}
    T value; 
};


template<typename T>
std::ostream & operator<<(
        std::ostream &outputStream,
        const PromoteByte<T> &asByte)
{
    if constexpr (sizeof(T) == 1)
    {
        return outputStream << std::setw(2) << detail::AsNumber(asByte.value);
    }
    else
    {
        return outputStream << asByte.value;
    }
}


template<typename T, size_t N>
class CircularBuffer
{
public:
    CircularBuffer()
        :
        writeIndex_(),
        readIndex_()
    {

    }

    void Reset()
    {
        this->writeIndex_.Reset();
        this->readIndex_.Reset();
    }

    bool IsEmpty()
    {
        return (this->readIndex_ == this->writeIndex_);
    }

    size_t GetSize() const
    {
        return this->writeIndex_ - this->readIndex_;
    }

    size_t GetAvailable() const
    {
        return N - this->GetSize();
    }

    T FrontElement()
    {
        if (this->IsEmpty())
        {
            throw std::out_of_range("Buffer is empty");
        }

        return this->elements_[static_cast<size_t>(this->readIndex_)];
    }

    T BackElement()
    {
        if (this->IsEmpty())
        {
            throw std::out_of_range("Buffer is empty");
        }

        return this->elements_[this->writeIndex_ - CircularIndex<N>(1)];
    }

    std::ostream & PrintElements(std::ostream &outputStream)
    {
        auto index = this->readIndex_;
        auto endIndex = this->writeIndex_;
        
        while (index != endIndex)
        {
            if constexpr (std::is_integral_v<T> && sizeof(T) == 1)
            {
                outputStream << PromoteByte(
                    this->elements_[static_cast<size_t>(index)]) << " ";
            }
            else
            {
                outputStream << this->elements_[static_cast<size_t>(index)] << " ";
            }

            ++index;
        }

        return outputStream;
    }

    std::ostream & PrintContents(std::ostream &outputStream)
    {
        auto index = 0;
        auto endIndex = N;
        
        while (index != endIndex)
        {
            if constexpr (std::is_integral_v<T> && sizeof(T) == 1)
            {
                outputStream << PromoteByte(this->elements_[index]) << " ";
            }
            else
            {
                outputStream << this->elements_[index] << " ";
            }

            ++index;
        }

        return outputStream;
    }

    bool Write(const T *source, size_t count)
    {
        if (count > this->GetAvailable())
        {
            return false;
        }

        // Because we have already checked that count is less than the free
        // space, we do not need to consider the position of the readIndex_ in
        // this check. If the readIndex_ is after the write index, then count
        // places us before the readIndex_.
        auto countToEnd = N - this->writeIndex_;
        auto tailCount = std::min(count, countToEnd);

        std::memcpy(
            &this->elements_[static_cast<size_t>(this->writeIndex_)],
            source,
            sizeof(T) * tailCount);

        if (count > tailCount)
        {
            // Write the remaining data at the start of the buffer
            auto remainder = count - tailCount;

            std::memcpy(
                &this->elements_[0],
                source + tailCount,
                sizeof(T) * remainder);
        }
        
        this->writeIndex_ += CircularIndex<N>(count);

        return true;
    }

    bool Peek(T * target, size_t count)
    {
        if (count > this->GetSize())
        {
            return false;
        }

        auto countToEnd = N - this->readIndex_;
        auto tailCount = std::min(count, countToEnd);

        std::memcpy(
            target,
            &this->elements_[static_cast<size_t>(this->readIndex_)],
            sizeof(T) * tailCount);

        if (count > tailCount)
        {
            // Read from the start of the buffer
            auto remainder = count - tailCount;

            std::memcpy(
                target + tailCount,
                &this->elements_[0],
                sizeof(T) * remainder);
        }

        return true;
    }

    bool Read(T * target, size_t count)
    {
        if (this->Peek(target, count))
        {
            // The Peek copied to target.
            // Increment the readIndex_ to consume the data.
            this->readIndex_ += CircularIndex<N>(count);
            return true;
        }

        return false;
    }

    size_t GetWriteIndex() const
    {
        return static_cast<size_t>(this->writeIndex_);
    }

    size_t GetReadIndex() const
    {
        return static_cast<size_t>(this->readIndex_);
    }
 
    void Remove(size_t count)
    {
        // Increment the readIndex_ to consume the data.
        this->readIndex_ += CircularIndex<N>(count);
    }

    template<typename, size_t>
    friend class AsPointer;

private:
    /**
     ** @return The count of bytes that can be written without overwriting
     ** either the end of the buffer or the read index.
     **/
    size_t GetWritableSize() const
    {
        auto countToEnd = N - static_cast<size_t>(this->writeIndex_);

        return std::min(this->GetAvailable(), countToEnd);
    }

private:
    CircularIndex<N> writeIndex_;
    CircularIndex<N> readIndex_;
    T elements_[N];
};


template<typename T, size_t N>
class AsPointer
{
public:
    AsPointer(CircularBuffer<T, N> &targetBuffer)
        :
        targetBuffer_(targetBuffer),
        writeCount_(0)
    {

    }

    size_t GetWritableSize() const
    {
        return this->targetBuffer_.GetWritableSize();
    }

    T * Get()
    {
        return &this->targetBuffer_.elements_[
            static_cast<size_t>(this->targetBuffer_.writeIndex_)];
    }

    void SetWriteCount(size_t count)
    {
        assert(count <= this->GetWritableSize());
        this->writeCount_ = CircularIndex<N>(count);
    }

    ~AsPointer()
    {
        if (this->writeCount_ > 0)
        {
            this->targetBuffer_.writeIndex_ += this->writeCount_;
        }
    }

private:
    CircularBuffer<T, N> &targetBuffer_;
    CircularIndex<N> writeCount_;
};


} // end namespace jive
