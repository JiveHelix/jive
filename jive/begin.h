/**
  * @file begin.h
  * 
  * @brief Returns pointer to begin and end of multidimensional array.
  * Similar to std::begin/std::end on 1-D arrays.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 03 Sep 2021
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

namespace jive
{

template<
    typename T,
    std::enable_if_t<
        std::is_array_v<std::remove_reference_t<T>>,
        bool
    > = true
>
auto Begin(T &&array)
{
    using ArrayType = std::remove_reference_t<T>;
    static_assert(std::is_array_v<ArrayType>);

    if constexpr (std::rank_v<ArrayType> == 1)
    {
        return &array[0];
    }
    else
    {
        return Begin(array[0]);
    }
}

template<
    typename T,
    std::enable_if_t<
        std::is_array_v<std::remove_reference_t<T>>,
        bool
    > = true
>
auto End(T &&array)
{
    using ArrayType = std::remove_reference_t<T>;
    static_assert(std::is_array_v<ArrayType>);

    using ValueType = std::remove_all_extents_t<ArrayType>;
    auto begin = Begin(std::forward<T>(array));
    return begin + (sizeof(array) / sizeof(ValueType));
}

} // end namespace jive
