/**
  * @file math_templates.h
  *
  * @brief Compile-time math functions.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 29 Apr 2018
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <utility>

namespace jive
{

namespace detail
{

template<ssize_t base, size_t ...Index>
constexpr auto Power(std::index_sequence<Index...>)
{
    return ((static_cast<void>(Index), base) * ... * 1);
}

} // end namespace detail

template<ssize_t base, auto exponent>
constexpr auto Power()
{
    return detail::Power<base>(std::make_index_sequence<exponent>{});
}

} // end namespace jive
