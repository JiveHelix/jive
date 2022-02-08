/**
  * @file precise_string_detail.h
  *
  * @brief A helper struct to build format strings that preserve floating-point
  * precision.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 09 Jul 2020
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include "jive/formatter.h"


namespace jive
{

namespace detail
{

#define STRING_EXPAND(token) #token
#define STRING(token) STRING_EXPAND(token)
#define FLOAT_FORMAT(digits, specifier) "%." STRING(digits) specifier

// Extra space for potential sign, decimal, 'e', exponent sign, and NULL byte
inline constexpr size_t extraSize = 5;

template<typename T>
struct FloatFormat {};

#define FLOAT_DIGITS 7
#define DOUBLE_DIGITS 16
#define LONG_DOUBLE_DIGITS 19

template<>
struct FloatFormat<float>
{
    static constexpr const char * format = FLOAT_FORMAT(FLOAT_DIGITS, "g");
    static constexpr size_t maximumExponentDigits = 2;
    static constexpr size_t size =
        FLOAT_DIGITS + maximumExponentDigits + extraSize;
};

template<>
struct FloatFormat<double>
{
    static constexpr const char * format = FLOAT_FORMAT(DOUBLE_DIGITS, "lg");
    static constexpr size_t maximumExponentDigits = 3;
    static constexpr size_t size =
        DOUBLE_DIGITS + maximumExponentDigits + extraSize;
};

template<>
struct FloatFormat<long double>
{
    static constexpr const char * format =
        FLOAT_FORMAT(LONG_DOUBLE_DIGITS, "Lg");

    static constexpr size_t maximumExponentDigits = 4;
    static constexpr size_t size =
        LONG_DOUBLE_DIGITS + maximumExponentDigits + extraSize;
};

} // end namespace detail

} // end namespace jive
