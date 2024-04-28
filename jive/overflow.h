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
#include <cmath>
#include "jive/constexpr_shim.h"


namespace jive
{


template<typename Target, typename Source>
std::enable_if_t<(std::is_floating_point_v<Source>), std::pair<Source, Source>>
GetExtrema()
{
    constexpr auto sourceDigits = std::numeric_limits<Source>::digits;
    constexpr auto targetDigits = std::numeric_limits<Target>::digits;

    constexpr Target highest = std::numeric_limits<Target>::max();
    constexpr Target lowest = std::numeric_limits<Target>::lowest();

    constexpr auto highestAsSourceType = static_cast<Source>(highest);
    constexpr auto lowestAsSourceType = static_cast<Source>(lowest);

    if constexpr (targetDigits > sourceDigits)
    {
        // Source type is unable to represent the Target type at the
        // extrema.
        // Use the next lowest (closest to zero) representable value.
        return {
            std::nexttoward(lowestAsSourceType, 0.0L),
            std::nexttoward(highestAsSourceType, 0.0L)};
    }
    else
    {
        return {lowestAsSourceType, highestAsSourceType};
    }
}


template<typename Target, typename Source>
std::enable_if_t<(std::is_floating_point_v<Source>), bool>
WillOverflow(Source value)
{
    auto [minimum, maximum] = GetExtrema<Target, Source>();
    return (value > maximum) || (value < minimum);
}


CONSTEXPR_SHIM_PUSH

template<typename Target, typename Source>
bool CheckConvertible(Source value)
{
    if constexpr (std::is_same_v<Target, Source>)
    {
        return true;
    }

    if constexpr (std::is_integral_v<Target>)
    {
        if constexpr (std::is_integral_v<Source>)
        {
            // Both Target and Source are integrals

            if constexpr (std::is_signed_v<Source> && !std::is_signed_v<Target>)
            {
                if (value < 0)
                {
                    return false;
                }
            }

            if constexpr (
                std::is_signed_v<Target>
                && !std::is_signed_v<Source>)
            {
                if (static_cast<Target>(value) < 0)
                {
                    return false;
                }
            }

            // Check the round trip.
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
    else
    {
        static_assert(
            std::is_floating_point_v<Target>,
            "Target type must be either integral or floating-point");

        if constexpr (std::is_floating_point_v<Source>)
        {
            // Both are floating-point
            return !WillOverflow<Target>(value);
        }
        else
        {
            // Consider all integral types convertible to floats.
            return true;
        }

    }

}

CONSTEXPR_SHIM_POP


} // namespace jive
