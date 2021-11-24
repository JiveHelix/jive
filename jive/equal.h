/**
  * @file equal.h
  * 
  * @brief Compare floating point values to within specified precision.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 10 Aug 2020
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <limits>
#include <cmath>
#include "jive/power.h"


namespace jive
{

/** @tparam imprecision Scales the unit of least precision.
 ** A value of 1 (the default) uses the epsilon.
 ** Higher values make the comparison less precise,
 ** zero makes the comparison equivalent to '=='.
 ** 
 ** It has no effect on integer types.
 **/
template<typename T, size_t imprecision = 1, typename = void>
struct Equal
{
    // For all non-floating point comparisons.
    template<typename Left, typename Right>
    bool operator()(Left &&left, Right &&right)
    {
        return std::forward<Left>(left) == std::forward<Right>(right);
    }
};


/** For all floating-point comparisons **/
template<typename T, size_t imprecision>
struct Equal<T, imprecision, std::enable_if_t<std::is_floating_point_v<T>>>
{
    static constexpr auto precision =
        std::numeric_limits<T>::epsilon() * imprecision;

    bool operator()(T left, T right)
    {
        auto difference = std::abs(left - right);
                
        auto tolerance = std::abs(left + right) * precision;

        // left and right compare equal when the difference is less than the
        // scaled precision, or when the difference is subnormal.
        return (difference < tolerance)
            || (difference < std::numeric_limits<T>::min());
    }
};


/**
 ** @tparam digits The number of significant figures to compare (in base 10)
 ** 'digits' is ignored unless T is floating-point.
 **
 ** @return true for floating-point values that compare equal to within the
 ** digits-th decimal place (as written in normalized scientific notation).
 **/
template<
    typename T,
    size_t digits = std::numeric_limits<T>::digits10,
    typename = void>
struct DigitsEqual
{
    // For all non-floating point comparisons.
    template<typename Left, typename Right>
    bool operator()(Left &&left, Right &&right)
    {
        return std::forward<Left>(left) == std::forward<Right>(right);
    }
};


/**
 ** @tparam digits The number of significant figures to compare (in base 10)
 **/
template<typename T, size_t digits>
struct DigitsEqual<
    T,
    digits,
    std::enable_if_t<std::is_floating_point_v<T>>
>
{
    static constexpr auto precision = 
        static_cast<T>(1.0) / static_cast<T>(jive::Power<10, digits>());

    bool operator()(T left, T right)
    {
        if constexpr (precision < std::numeric_limits<T>::epsilon())
        {
            return left == right;
        }
        else
        {
            auto difference = std::abs(left - right);

#ifdef USE_PRECISE_DIGITS
            auto scale =
                std::pow(
                    10,
                    std::floor(
                        std::log10(
                            std::max(
                                std::abs(left),
                                std::abs(right)))));
#else
            auto scale = std::abs(left + right);
#endif
            auto tolerance = scale * precision;

            // left and right compare equal when the difference is less than
            // the scaled precision, or when the difference is subnormal.
            return (difference < tolerance)
                || (difference < std::numeric_limits<T>::min());
        }
    }
};

} // namespace jive
