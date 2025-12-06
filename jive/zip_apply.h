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

#include <cstddef>
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


template<typename First, typename Second, size_t ... I>
constexpr auto Zip(First &&firsts, Second &&seconds, std::index_sequence<I...>)
{
    return std::make_tuple(
        std::pair(std::get<I>(firsts), std::get<I>(seconds))...);
}

#if 1
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

    constexpr auto size = std::tuple_size_v<std::remove_reference_t<First>>;

    ApplyElements(
        Zip(
            std::forward<First>(first),
            std::forward<Second>(second),
            std::make_index_sequence<size>{}),
        std::forward<Function>(function));
}

#else

template<typename F, typename A, typename B, std::size_t... I>
inline void ZipForEachImpl(F &&f, A &&a, B &&b, std::index_sequence<I...>)
{
    (std::invoke(std::forward<F>(f),
                 std::get<I>(std::forward<A>(a)),
                 std::get<I>(std::forward<B>(b))), ...);
}

template<typename F, typename A, typename B>
inline void ZipApply(F &&f, A &&a, B &&b)
{
    using TA = std::remove_reference_t<A>;
    using TB = std::remove_reference_t<B>;
    static_assert(std::tuple_size_v<TA> == std::tuple_size_v<TB>);

    ZipForEachImpl(std::forward<F>(f),
                   std::forward<A>(a),
                   std::forward<B>(b),
                   std::make_index_sequence<std::tuple_size_v<TA>>{});
}

#endif

} // namespace jive
