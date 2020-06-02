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


template<typename Target, typename Source>
std::enable_if_t<(
    std::is_floating_point_v<Source>
    && std::is_integral_v<Target>), std::pair<Source, Source>>
GetExtrema()
{
    constexpr auto sourceDigits = std::numeric_limits<Source>::digits;
    constexpr auto targetDigits = std::numeric_limits<Target>::digits;

    constexpr Target maximum = std::numeric_limits<Target>::max();
    constexpr Target minimum = std::numeric_limits<Target>::min();

    constexpr auto maximumAsSource = static_cast<Source>(maximum);
    constexpr auto minimumAsSource = static_cast<Source>(minimum);

    if constexpr (targetDigits > sourceDigits)
    {
        // Source type is unable to represent the Target type exactly at the
        // postive extreme.
        // Use the next lower representable value.
        return {minimumAsSource, std::nexttoward(maximumAsSource, 0.0L)};
    }
    else
    {
        return {minimumAsSource, maximumAsSource};
    }
}


template<typename Target, typename Source>
std::enable_if_t<(
    std::is_floating_point_v<Source>
    && std::is_integral_v<Target>), bool>
WillOverflow(Source value)
{
    auto [minimum, maximum] = GetExtrema<Target, Source>();
    return (value > maximum) || (value < minimum);
}


struct CheckOverflow {};
struct NoCheckOverflow {};


template<typename Target, typename Source, typename Overflow = CheckOverflow>
Target RoundIfIntegral(Source value)
{
    if constexpr (std::is_integral<Target>::value)
    {
        auto result = round(value);

        if constexpr (std::is_same_v<Overflow, CheckOverflow>)
        {
            if (WillOverflow<Target>(result))
            {
                throw std::overflow_error("value will overflow target type.");
            }
        }

        return static_cast<Target>(result);
    }

    return static_cast<Target>(value);
}


template<typename Source, typename Target>
auto MultiplyRounded(Source scale, Target value) -> Target
{
    return RoundIfIntegral<Target>(scale * static_cast<Source>(value));
}

template<typename Source, typename Target>
auto MultiplyRounded(NoCheckOverflow, Source scale, Target value) -> Target
{
    return RoundIfIntegral<Target, Source, NoCheckOverflow>(
        scale * static_cast<Source>(value));
}


template<typename Source, typename Target, typename ...U>
auto MultiplyRounded(Source scale, Target first, U ...values)
    -> std::tuple<Target, U...>
{
    return std::make_tuple(
        RoundIfIntegral<Target>(scale * static_cast<Source>(first)),
        RoundIfIntegral<decltype(values)>(
            scale * static_cast<Source>(values))...);
}

template<typename Source, typename Target, typename ...U>
auto MultiplyRounded(
    NoCheckOverflow,
    Source scale,
    Target first,
    U ...values) -> std::tuple<Target, U...>
{
    return std::make_tuple(
        RoundIfIntegral<Target, Source, NoCheckOverflow>(
            scale * static_cast<Source>(first)),

        RoundIfIntegral<decltype(values), Source, NoCheckOverflow>(
            scale * static_cast<Source>(values))...);
}

} // end namespace jive
