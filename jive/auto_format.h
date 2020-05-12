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

    static constexpr std::string_view value =
        jive::StaticJoin<
            percent,
            detail::LengthModifier<T>::value,
            detail::FormatSpecifier<T, base>::value>::value;
};


template<int base, typename T>
std::string AutoFormatter(T value)
{
    return FastFormatter<32>(
        AutoFormat<T, base>::value.data(),
        value);
}

} // end namespace jive
