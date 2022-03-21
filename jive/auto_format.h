/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include "jive/static_join.h"
#include "jive/formatter.h"
#include "jive/detail/auto_format_detail.h"

namespace jive
{

/**
 ** Creates a format string as static class member value.
 **
 ** Uses T and base to select the correct length modifier and format specifier.
 ** 
 ** Width and Precision placeholders are included, so width and precision must
 ** be included as the first two arguments when using this format string.
 **
 ** Examples:
 ** AutoFormat<double, 10>::value is "%*.*l#g"
 ** AutoFormat<float, 10>::value is "%*.*#g"
 ** AutoFormat<uint16_t, 16>::value is "%*.*hX"
 **
 **/
template<typename T, int base>
struct AutoFormat
{
    static constexpr std::string_view percent = "%";
    static constexpr std::string_view widthAndPrecision = "*.*";
    static constexpr std::string_view value =
        jive::StaticJoin<
            percent,
            widthAndPrecision,
            detail::LengthModifier<T>::value,
            detail::FormatSpecifier<T, base>::value>::value;
};


template<int base, typename T>
std::string AutoFormatter(T value, int width = -1 , int precision = -1)
{
    return Formatter<32>(
        AutoFormat<T, base>::value.data(),
        width,
        precision,
        value);
}

} // end namespace jive
