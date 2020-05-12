/**
  * @file multiply_rounded.h
  * 
  * @brief Multiply any number/type of values, rounding the result if the value
  * is an integral type.
  * 
  * If more than one value is provided, the result is a std::tuple. When called
  * with a single value, the result is a single value of the same type.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 03 May 2020
  *
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <type_traits>
#include <limits>
#include <cmath>
#include <stdexcept>

namespace jive
{

template<typename T>
bool WillOverflow(double value)
{
    if ((value > std::numeric_limits<T>::max())
            || (value < std::numeric_limits<T>::min()))
    {
        return true;
    }
    
    return false;
}


struct CheckOverflow {};
struct NoCheckOverflow {};


template<typename T, typename Overflow = CheckOverflow>
T RoundIfIntegral(double value)
{
    if constexpr (std::is_integral<T>::value)
    {
        auto result = round(value);

        if constexpr (std::is_same_v<Overflow, CheckOverflow>)
        {
            if (WillOverflow<T>(result))
            {
                throw std::overflow_error("value will overflow target type.");
            }
        }

        return static_cast<T>(result);
    }

    return static_cast<T>(value);
}


template<typename T>
auto MultiplyRounded(double scale, T value) -> T
{
    return RoundIfIntegral<T>(scale * static_cast<double>(value));
}

template<typename T>
auto MultiplyRounded(NoCheckOverflow, double scale, T value) -> T
{
    return RoundIfIntegral<T, NoCheckOverflow>(
        scale * static_cast<double>(value));
}


template<typename T, typename ...U>
auto MultiplyRounded(double scale, T first, U ...values) -> std::tuple<T, U...>
{
    return std::make_tuple(
        RoundIfIntegral<T>(scale * static_cast<double>(first)),
        RoundIfIntegral<decltype(values)>(
            scale * static_cast<double>(values))...);
}

template<typename T, typename ...U>
auto MultiplyRounded(
    NoCheckOverflow,
    double scale,
    T first,
    U ...values) -> std::tuple<T, U...>
{
    return std::make_tuple(
        RoundIfIntegral<T, NoCheckOverflow>(scale * static_cast<double>(first)),
        RoundIfIntegral<decltype(values), NoCheckOverflow>(
            scale * static_cast<double>(values))...);
}

} // end namespace jive
