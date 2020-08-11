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
