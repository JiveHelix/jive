/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <string_view>
#include <utility>

namespace jive
{

namespace detail
{

// Base template is required to allow the index sequences in the
// specialization.
template <
    const std::string_view &,
    typename,
    const std::string_view &,
    typename>
struct Concatenate;

template<
    const std::string_view &first,
    std::size_t ...firstIndex,
    const std::string_view &second,
    std::size_t ...secondIndex>
struct Concatenate<
    first,
    std::index_sequence<firstIndex...>,
    second,
    std::index_sequence<secondIndex...>>
{
    // expand each string view into a char array.
    static constexpr const char value[]{
        first[firstIndex]...,
        second[secondIndex]..., 0};
};

} // end namepsace detail

template<const std::string_view &...> struct StaticJoin;

template<>
struct StaticJoin<>
{
    static constexpr std::string_view value = "";
};

template<const std::string_view &first, const std::string_view &second>
struct StaticJoin<first, second>
{
    // End recursion and call Concatenate when there are only two string_view's
    // left.

    // detail::Concatenate's value is const char [], referenced here by
    // StaticJoin's string_view value.
    static constexpr std::string_view value =
        detail::Concatenate<
            first,
            std::make_index_sequence<first.size()>,
            second,
            std::make_index_sequence<second.size()>>::value;
};

template<const std::string_view &first, const std::string_view & ...others>
struct StaticJoin<first, others...>
{
    // Continue recursion as long as there are more than two string_views.
    static constexpr std::string_view value =
        StaticJoin<first, StaticJoin<others...>::value>::value;
};

} // end namespace jive
