/**
  * @file for_each.h
  *
  * @brief Calls a function for each item in a std::tuple.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 01 May 2020
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  *
  * Requires C++14 or later.
  */

#pragma once

#include <tuple>

namespace jive
{

#if __cplusplus >= 201703L

template<typename Tuple, typename Function>
void ForEach(Tuple &&tuple, Function &&function)
{
    std::apply(
        [&](auto &&...item) -> void
        {
            (
                static_cast<void>(
                    function(std::forward<decltype(item)>(item))), ...);
        },
        std::forward<Tuple>(tuple));
}

#else

/*
Without fold expressions in C++14, we must use the expander trick:
https://stackoverflow.com/questions/30563254/how-can-i-expand-call-to-variadic-template-base-classes/30563282#30563282
*/
template<typename Tuple, typename Function, std::size_t... I>
void ForEach(Tuple &&tuple, Function &&function, std::index_sequence<I...>)
{
    auto ignored =
        { 0, (static_cast<void>(function(std::get<I>(tuple))), 0)... };
}

template<typename Tuple, typename Function>
void ForEach(Tuple &&tuple, Function &&function)
{
    constexpr auto itemCount = std::tuple_size<
        typename std::remove_reference<decltype(tuple)>::type>::value;

    ForEach(
        std::forward<Tuple>(tuple),
        std::forward<Function>(function),
        std::make_index_sequence<itemCount>{});
}

#endif

} // end namespace jive
