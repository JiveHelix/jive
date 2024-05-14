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
#include <limits>
#include "jive/constexpr_shim.h"


namespace jive
{


template<typename Target, typename Source>
std::enable_if_t<(std::is_floating_point_v<Source>), std::pair<Source, Source>>
GetExtrema()
{

    constexpr Target highest = std::numeric_limits<Target>::max();
    constexpr Target lowest = std::numeric_limits<Target>::lowest();

    constexpr auto highestAsSourceType = static_cast<Source>(highest);
    constexpr auto lowestAsSourceType = static_cast<Source>(lowest);

    return {lowestAsSourceType, highestAsSourceType};
}


template<typename Target, typename Source>
std::enable_if_t<(std::is_floating_point_v<Source>), bool>
ExceedsTarget(Source value)
{
    auto [lowest, maximum] = GetExtrema<Target, Source>();
    return (value > maximum) || (value < lowest);
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

            return !ExceedsTarget<Target>(value);
        }
    }
    else
    {
        // Target is not integral.
        static_assert(
            std::is_floating_point_v<Target>,
            "Target type must be either integral or floating-point");

        if constexpr (std::is_floating_point_v<Source>)
        {
            // Both are floating-point
            return !ExceedsTarget<Target>(value);
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
