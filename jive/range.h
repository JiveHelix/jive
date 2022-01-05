/**
  * @file range.h
  * 
  * @brief Easy iteration over a range of values.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 26 Oct 2021
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <iostream>


namespace jive
{


template<typename T>
class RangeIterator
{
public:
    RangeIterator(T value): value_(value)
    {

    }

    bool operator!=(const RangeIterator &other) const
    {
        return this->value_ != other.value_;
    }

    T operator*() const
    {
        return this->value_;
    }
    
    RangeIterator & operator++()
    {
        ++(this->value_);
        return *this;
    }

private:
    T value_;
};


template<typename T>
class Range
{
public:
    Range(T begin, T end): begin_(begin), end_(end)
    {

    }

    RangeIterator<T> begin() const
    {
        return RangeIterator<T>(this->begin_);
    }

    RangeIterator<T> end() const
    {
        return RangeIterator<T>(this->end_);
    }

private:
    const T begin_;
    const T end_;
};


} // end namespace jive
