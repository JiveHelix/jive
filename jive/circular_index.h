/**
  * @file circular_index.h
  *
  * @brief CircularIndex behaves like size_t, except that it cannot be modified
  * except by pre/post increment. wrapCount determines the value at which the
  * index wraps back to the start.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 27 Feb 2018
  * @copyright Jive Helix
  *
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <cstddef>

namespace jive
{


/*
 * @tparam wrapCount defaults to 0, meaning the wrapCount must be specified at
 * runtime in the constructor.
 */
template<size_t wrapCount>
class CircularIndex
{
public:
    CircularIndex()
        :
        wrapCount_(wrapCount),
        index_(0)
    {
        static_assert(
            wrapCount != 0,
            "Runtime wrapCount must be specified");
    }

    explicit CircularIndex(size_t index)
        :
        wrapCount_(wrapCount),
        index_(index % wrapCount)
    {
        static_assert(
            wrapCount != 0,
            "Run-time wrapCount must be specified");
    }

    explicit CircularIndex(size_t runtimeWrapCount, size_t index)
        :
        wrapCount_(runtimeWrapCount)
    {
        if constexpr (wrapCount != 0)
        {
            static_assert(wrapCount == runtimeWrapCount);
        }

        if (runtimeWrapCount == 0)
        {
            throw std::invalid_argument("runtimeWrapCount cannot be zero");
        }

        this->index_ = index % this->wrapCount_;
    }

    static CircularIndex Create(size_t runtimeWrapCount)
    {
        if (runtimeWrapCount == 0)
        {
            throw std::invalid_argument("runtimeWrapCount cannot be zero");
        }

        if constexpr (wrapCount != 0)
        {
            static_assert(wrapCount == runtimeWrapCount);
        }

        return CircularIndex(runtimeWrapCount, 0);
    }

    CircularIndex(const CircularIndex &other)
        :
        wrapCount_(other.wrapCount_),
        index_(other.index_)
    {

    }

    CircularIndex & operator=(const CircularIndex &other)
    {
        this->index_ = other.index_;
        return *this;
    }

    // Post-increment
    CircularIndex operator++(int)
    {
        CircularIndex result(*this);
        ++(*this);
        return result;
    }

    // Pre-increment
    CircularIndex & operator++()
    {
        this->index_ = (this->index_ + 1) % this->wrapCount_;
        return *this;
    }

    // Post-decrement
    CircularIndex operator--(int)
    {
        CircularIndex result(*this);
        --(*this);
        return result;
    }

    // Pre-decrement
    CircularIndex & operator--()
    {
        this->index_ =
            (this->index_ + this->wrapCount_ - 1) % this->wrapCount_;

        return *this;
    }

    CircularIndex operator+(CircularIndex<wrapCount> addend)
    {
        CircularIndex result(*this);
        return result += addend;
    }

    CircularIndex & operator+=(CircularIndex<wrapCount> addend)
    {
        this->index_ = (this->index_ + addend.index_) % this->wrapCount_;
        return *this;
    }

    CircularIndex operator-(CircularIndex<wrapCount> addend)
    {
        CircularIndex result(*this);
        return result -= addend;
    }

    CircularIndex & operator-=(CircularIndex<wrapCount> addend)
    {
        // Add wrapCount before subtracting to ensure that we never go less
        // than zero. (Similar to operator--).
        this->index_ =
            (this->index_ + this->wrapCount_ - addend.index_)
            % this->wrapCount_;

        return *this;
    }

    /* Intentional use of the implicit type conversion operator. */
    operator size_t () const
    {
        return this->index_;
    }

    bool operator == (const CircularIndex &other) const
    {
        return this->index_ == other.index_;
    }

    bool operator != (const CircularIndex &other) const
    {
        return this->index_ != other.index_;
    }

    void Reset()
    {
        this->index_ = 0;
    }

private:
    size_t wrapCount_;
    size_t index_;
};


template<size_t N>
CircularIndex<N> operator-(CircularIndex<N> left, CircularIndex<N> right)
{
    return left.operator-(right);
}

template<size_t N>
CircularIndex<N> operator+(CircularIndex<N> left, CircularIndex<N> right)
{
    return left.operator+(right);
}


template<size_t wrapCount>
CircularIndex<wrapCount> CopyCircularIndex(
    const CircularIndex<wrapCount> &index)
{
    return CircularIndex<wrapCount>(index);
}

} // end namespace jive
