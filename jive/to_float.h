/**
  * @file to_float.h
  *
  * @brief Convert strings to float, double, or long double.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 08 Jul 2020
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <stdexcept>
#include <type_traits>
#include <string_view>
#include <limits>
#include <cerrno>
#include <cmath>


namespace jive
{

template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, T>
ToFloat(const std::string_view &asString)
{
    T result;
    char *end;

    if constexpr (std::is_same_v<float, T>)
    {
        result = strtof(asString.data(), &end);
    }
    else if constexpr (std::is_same_v<double, T>)
    {
        result = strtod(asString.data(), &end);
    }
    else
    {
        static_assert(std::is_same_v<long double, T>);
        result = strtold(asString.data(), &end);
    }

    if (end == asString.data())
    {
        throw std::invalid_argument("Unable to convert string");
    }

    if ((std::fabs(result) == std::numeric_limits<T>::infinity())
        || (result == static_cast<T>(0.0)))
    {
        if (errno == ERANGE)
        {
            throw std::out_of_range("Input is out-of-range.");
        }
    }

    return result;
}

} // end namespace jive
