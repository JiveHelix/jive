/**
  * @file zip_apply.h
  *
  * @brief Iterate over two tuples, using one element from each as arguments to
  * a function.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 29 Jul 2020
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <tuple>
#include <type_traits>

namespace jive
{

/** Expects a tuple of tuples that will be expanded as arguments to Function.
 **/
template<typename Tuple, typename Function>
void ApplyElements(Tuple &&tuple, Function &&function)
{
    std::apply(
        [&](auto &&...item)
        {
            (static_cast<void>(
                std::apply(
                    std::forward<Function>(function),
                    std::forward<decltype(item)>(item))),
             ...);
        },
        std::forward<Tuple>(tuple));
}


template<typename First, typename Second>
constexpr auto Zip(First &&firsts, Second &&seconds)
{
    return std::apply(
        [&](auto &&...first)
        {
            return std::apply(
                [&](auto &&...second)
                {
                    return std::tuple(
                        std::pair(
                            std::forward<decltype(first)>(first),
                            std::forward<decltype(second)>(second))...);
                },
                std::forward<Second>(seconds));
        },
        std::forward<First>(firsts));
}


template<typename First, typename Second, typename Function>
void ZipApply(
    Function &&function,
    First &&first,
    Second &&second)
{
    static_assert(
        std::is_invocable_v
        <
            Function,
            std::tuple_element_t<0, std::remove_reference_t<First>>,
            std::tuple_element_t<0, std::remove_reference_t<Second>>
        >,
        "Function most be invocable with two arguments, one from each tuple.");

    static_assert(
        std::tuple_size_v<std::remove_reference_t<First>>
            == std::tuple_size_v<std::remove_reference_t<Second>>,
        "Tuples must have the same length.");

    ApplyElements(
        Zip(
            std::forward<First>(first),
            std::forward<Second>(second)),
        std::forward<Function>(function));
}

} // namespace jive
