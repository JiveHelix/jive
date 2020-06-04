/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 05 May 2020
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <cmath>

namespace jive
{


inline constexpr long double pi = 3.14159265358979323846L;
inline constexpr long double halfRotationDegrees = 180.0L;
inline constexpr long double fullRotationDegrees = 360.0L;
inline constexpr long double degreesPerRadian = halfRotationDegrees / pi;
inline constexpr long double radiansPerDegree = pi / halfRotationDegrees;


template<typename T>
struct Angles
{
    static_assert(std::is_floating_point_v<T>);

    static constexpr T pi = static_cast<T>(::jive::pi);

    static constexpr T halfRotationDegrees =
        static_cast<T>(::jive::halfRotationDegrees);

    static constexpr T fullRotationDegrees =
        static_cast<T>(::jive::fullRotationDegrees);

    static constexpr T degreesPerRadian =
        static_cast<T>(::jive::degreesPerRadian);

    static constexpr T radiansPerDegree =
        static_cast<T>(::jive::radiansPerDegree);
};



template<typename T>
auto ToDegrees(T radians) -> T
{
    return radians * Angles<T>::degreesPerRadian;
}


template<typename T, typename ...U>
auto ToDegrees(T radians, U ...otherRadians) -> std::tuple<T, U...>
{
    return std::make_tuple(ToDegrees(radians), ToDegrees(otherRadians)...);
}


template<typename T>
auto ToRadians(T degrees) -> T
{
    return degrees * Angles<T>::radiansPerDegree;
}


template<typename T, typename ...U>
auto ToRadians(T degrees, U ...otherDegrees) -> std::tuple<T, U...>
{
    return std::make_tuple(ToRadians(degrees), ToRadians(otherDegrees)...);
}


} // end namespace jive
