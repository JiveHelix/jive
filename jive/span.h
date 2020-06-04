/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once
#include <cstddef>

#if (__cplusplus > 201703L)
#include <span>
#else

/** std::span will be available in c++20, with more features than implemented
 ** here. Until then, the interface of this class closely matches the proposed
 ** span class. The standard span class
 **
 ** span provides a view of array data, allowing functions to return views of
 ** array data with sizes set at compile time.
 **
 **/

namespace std
{

template<typename T, std::ptrdiff_t N>
class span
{
public:
    span(T (&array)[N])
        :
        data_(&array[0])
    {

    }

    T & operator[](std::ptrdiff_t index)
    {
        return this->data_[index];
    }

    T * data() { return this->data_; }

    T * begin()
    {
        return this->data_;
    }

    T * end()
    {
        return this->data_ + N;
    }

private:
    T *data_;
};

} // namespace std

#endif // Not c++20 yet.

template<typename T, std::ptrdiff_t N>
std::span<T, N> make_span(T (&array)[N])
{
    return std::span<T, N>(array);
};

