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


#ifdef FOR_EACH_USE_STD_APPLY

#if __cplusplus < 201703L
#error std::apply is only available in C++17 and later.
#endif

template<typename Tuple, typename Function>
void ForEach(Tuple &&tuple, Function &&function)
{
    std::apply(
        [&](auto &&...item)
        {
            (static_cast<void>(
                function(std::forward<decltype(item)>(item))),
             ...);
        },
        std::forward<Tuple>(tuple));
}
#endif


/**
 ** I have reverted back to the C++14 method of applying a function over
 ** a tuple. If the Function is a lambda expression that instantiates templated
 ** types, std::apply appears to hide those types from the linker. Using the
 ** expander trick, the linker correctly sees template instantiations.
 **
 ** Use the expander trick:
 ** https://stackoverflow.com/questions/30563254/how-can-i-expand-call-to-variadic-template-base-classes/30563282#30563282
 **/
template<typename Tuple, typename Function, std::size_t... I>
void ForEach(Tuple &&tuple, Function &&function, std::index_sequence<I...>)
{
    auto ignored =
        { 0, (static_cast<void>(function(std::get<I>(tuple))), 0)... };

    static_cast<void>(ignored);
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

} // end namespace jive
