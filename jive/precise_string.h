/**
  * @file precise_string.h
  *
  * @brief Preserve floating-point precision when converting to string.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 09 Jul 2020
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <string>
#include "jive/formatter.h"
#include "jive/detail/precise_string_detail.h"


namespace jive
{

template<typename T>
std::enable_if_t<std::is_integral_v<T>, std::string>
PreciseString(T value)
{
    return std::to_string(value);
}

template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, std::string>
PreciseString(T value)
{
    static constexpr auto size = detail::FloatFormat<T>::size;
    static constexpr auto format = detail::FloatFormat<T>::format;

#ifdef __GNUG__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdouble-promotion"
    // floats are always promoted to double in calls to variadic functions.
    // This warning generates too much noise in this case.
#endif

    return jive::FastFormatter<size>(format, value);

#ifdef __GNUG__
#pragma GCC diagnostic pop
#endif
}

} // end namespace jive
