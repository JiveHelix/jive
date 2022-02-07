/**
  * @file overflow.h
  * 
  * @brief Detect overflow when converting between types.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 22 Apr 2021
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <type_traits>


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


template<typename Target, typename Source>
bool CheckConvertible(Source value)
{
    if constexpr (std::is_integral_v<Target>)
    {
        if constexpr (std::is_integral_v<Source>)
        {
            // Both Target and Source are integrals
            // Check the round trip
            Target result = static_cast<Target>(value);
            Source check = static_cast<Source>(result);
            return (check == value);
        }
        else
        {
            static_assert(
                std::is_floating_point_v<Source>,
                "Source type must be either integral or floating-point");

            return !WillOverflow<Target>(value);
        }
    }
}


} // namespace jive
