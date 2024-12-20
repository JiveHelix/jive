/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include "jive/static_join.h"
#include "jive/detail/auto_format_detail.h"


namespace jive
{


namespace flag
{

struct None
{
    static constexpr std::string_view value = "";
};

struct Minus
{
    static constexpr std::string_view value = "-";
};

struct Plus
{
    static constexpr std::string_view value = "+";
};

struct Hash
{
    static constexpr std::string_view value = "#";
};

struct Zero
{
    static constexpr std::string_view value = "0";
};

struct Alternate
{
    static constexpr std::string_view value = "#";
};

} // end namespace flag


/**
 ** Creates a format string as static class member value.
 **
 ** Uses T and base to select the correct length modifier and format specifier.
 **
 ** Width and Precision placeholders are included, so width and precision must
 ** be included as the first two arguments when using this format string.
 **
 ** Examples:
 ** AutoFormat<double, 10>::value is "%#*.*lg"
 ** AutoFormat<float, 10>::value is "%#*.*g"
 ** AutoFormat<uint16_t, 16>::value is "%*.*hX"
 **
 **/
template<typename T, int base, typename Flag = flag::None>
struct AutoFormat
{
    static constexpr std::string_view percent = "%";
    static constexpr std::string_view widthAndPrecision = "*.*";

    static constexpr std::string_view value =
        jive::StaticJoin
        <
            percent,
            Flag::value,
            widthAndPrecision,
            detail::LengthModifier<T>::value,
            detail::FormatSpecifier<T, base>::value
        >::value;
};


template<typename T, int base, typename Flag = flag::None>
struct FixedFormat
{
    static constexpr std::string_view percent = "%";
    static constexpr std::string_view widthAndPrecision = "*.*";

    static constexpr std::string_view value =
        jive::StaticJoin
        <
            percent,
            Flag::value,
            widthAndPrecision,
            detail::LengthModifier<T>::value,
            detail::FixedSpecifier<T, base>::value
        >::value;
};


template<typename T, int base, typename Flag = flag::None>
struct ScientificFormat
{
    static constexpr std::string_view percent = "%";
    static constexpr std::string_view widthAndPrecision = "*.*";

    static constexpr std::string_view value =
        jive::StaticJoin
        <
            percent,
            Flag::value,
            widthAndPrecision,
            detail::LengthModifier<T>::value,
            detail::ScientificSpecifier<T, base>::value
        >::value;
};


} // end namespace jive
