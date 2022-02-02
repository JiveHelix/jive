/**
  * @file to_integer.h
  *
  * @brief Converts string to integer of any integral type.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 01 May 2020
  * @copyright Jive Helix
  *
  * Licensed under the MIT license. See LICENSE file.
  *
  */

#pragma once

#include <type_traits>
#include <string_view>
#include <typeinfo>
#include <limits>

#include "jive/detail/to_integer_detail.h"
#include "jive/strings.h"

namespace jive
{

template<typename T, int base = base10>
std::enable_if_t<std::is_integral<T>::value, T>
ToInteger(const std::string_view &asString)
{
#ifndef NDEBUG
    std::string trimmedInput = jive::strings::Trim(std::string(asString), " ");

    if (!detail::ValidateDigits<T, base>(trimmedInput))
    {
        throw std::invalid_argument(
            std::string("Expected only digits: ")
            + std::string(detail::AllowedDigits<T, base>::value)
            + ", found " + std::string(asString));
    }
#endif

    // result will either be long long or unsigned long long depending on the
    // sign of T.
    auto result = detail::ToInteger<T, base>(asString);

    if ((result > std::numeric_limits<T>::max())
        || (result < std::numeric_limits<T>::min()))
    {
        throw std::out_of_range(
            "Input is out-of-range of requested type.");
    }

    return static_cast<T>(result);
}

} // end namespace jive
