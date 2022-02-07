/**
  * @file random_type.h
  * 
  * @brief Promotes type to 64-bit signed or unsigned for use in MSVC random
  * number generator in Catch2.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 05 Feb 2022
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <type_traits>


template<typename T, typename Enable = void>
struct RandomType {};

template<typename T>
struct RandomType<T, std::enable_if_t<std::is_signed_v<T>>>
{
    using type = int64_t;
};

template<typename T>
struct RandomType<T, std::enable_if_t<std::is_unsigned_v<T>>>
{
    using type = uint64_t;
};
