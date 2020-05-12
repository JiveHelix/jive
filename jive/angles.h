/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 05 May 2020
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <cmath>

namespace jive
{


template<typename T>
auto ToDegrees(T radians) -> T
{
    return radians * static_cast<T>(180.0 / M_PI);
}


template<typename T, typename ...U>
auto ToDegrees(T radians, U ...otherRadians) -> std::tuple<T, U...>
{
    return std::make_tuple(ToDegrees(radians), ToDegrees(otherRadians)...);
}


template<typename T>
auto ToRadians(T degrees) -> T
{
    return degrees * static_cast<T>(M_PI / 180.0);
}


template<typename T, typename ...U>
auto ToRadians(T degrees, U ...otherDegrees) -> std::tuple<T, U...>
{
    return std::make_tuple(ToRadians(degrees), ToRadians(otherDegrees)...);
}


} // end namespace jive
