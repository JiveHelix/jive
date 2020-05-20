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

template<typename T, typename S>
bool WillOverflow(S value)
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


template<typename T, typename S, typename Overflow = CheckOverflow>
T RoundIfIntegral(S value)
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


template<typename S, typename T>
auto MultiplyRounded(S scale, T value) -> T
{
    return RoundIfIntegral<T>(scale * static_cast<S>(value));
}

template<typename S, typename T>
auto MultiplyRounded(NoCheckOverflow, S scale, T value) -> T
{
    return RoundIfIntegral<T, S, NoCheckOverflow>(
        scale * static_cast<S>(value));
}


template<typename S, typename T, typename ...U>
auto MultiplyRounded(S scale, T first, U ...values) -> std::tuple<T, U...>
{
    return std::make_tuple(
        RoundIfIntegral<T>(scale * static_cast<S>(first)),
        RoundIfIntegral<decltype(values)>(
            scale * static_cast<S>(values))...);
}

template<typename S, typename T, typename ...U>
auto MultiplyRounded(
    NoCheckOverflow,
    S scale,
    T first,
    U ...values) -> std::tuple<T, U...>
{
    return std::make_tuple(
        RoundIfIntegral<T, S, NoCheckOverflow>(scale * static_cast<S>(first)),
        RoundIfIntegral<decltype(values), S, NoCheckOverflow>(
            scale * static_cast<S>(values))...);
}

} // end namespace jive
