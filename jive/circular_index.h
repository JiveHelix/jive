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

template<size_t wrapCount>
class CircularIndex
{
public:
    CircularIndex()
        :
        index_(0)
    {

    }

    CircularIndex(const CircularIndex &other)
        :
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
        this->index_ = (this->index_ + 1) % wrapCount;
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
            (this->index_ + wrapCount - 1) % wrapCount;

        return *this;
    }

    CircularIndex operator+(size_t addend)
    {
        CircularIndex result(*this);
        return result += addend;
    }

    CircularIndex & operator+=(size_t addend)
    {
        this->index_ = (this->index_ + (addend % wrapCount)) % wrapCount;
        return *this;
    }

    CircularIndex operator-(size_t addend)
    {
        CircularIndex result(*this);
        return result -= addend;
    }

    CircularIndex & operator-=(size_t addend)
    {
        size_t modulusAddend = addend % wrapCount;
            
        // modulusAddend is guaranteed to be less than wrapCount
        // Add wrapCount before subtracting to ensure that we never go less
        // than zero. (Similar to operator--.
        this->index_ = (this->index_ + wrapCount - modulusAddend) % wrapCount;
        return *this;
    }

    /* Intentional use of the implicit type conversion operator. */
    operator size_t () const
    {
        return this->index_;
    }

    bool operator == (const CircularIndex &other)
    {
        return this->index_ == other.index_;
    }

    bool operator != (const CircularIndex &other)
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

template<size_t wrapCount>
CircularIndex<wrapCount> CopyCircularIndex(
    const CircularIndex<wrapCount> &index)
{
    return CircularIndex<wrapCount>(index);
}

} // end namespace jive
