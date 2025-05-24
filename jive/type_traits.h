/**
  * @file type_traits.h
  *
  * @brief Additional utilities for working with types.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 24 Aug 2021
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <ostream>
#include <type_traits>
#include <bitset>


namespace jive
{


template<typename T, typename = void>
struct IsIterable: std::false_type {};

template<typename T>
struct IsIterable
<
    T,
    std::enable_if_t
    <
        !std::is_void_v<decltype(std::declval<T>().begin())>
        &&
        std::is_same_v
        <
            decltype(std::declval<T>().begin()),
            decltype(std::declval<T>().end())
        >
    >
>: std::true_type {};


/** If the container defines key_type and mapped_type, it is close enough
 ** to be considered map-like for our purposes.**/
template<typename T, typename = std::void_t<>>
struct IsMapLike: std::false_type {};

template<typename T>
struct IsMapLike<
    T,
    std::void_t<
        typename T::key_type,
        typename T::mapped_type>>: public std::true_type {};


template<typename T>
struct IsString: std::false_type {};

template<>
struct IsString<std::string>: std::true_type {};


template<typename T, typename = std::void_t<>>
struct IsValueContainer: std::false_type {};

template<typename T>
struct IsValueContainer<
    T,
    std::void_t<
        std::enable_if_t<
            jive::IsIterable<T>::value
            && std::is_integral_v<decltype(std::declval<T>().size())>
            && !jive::IsMapLike<T>::value
            && !IsString<T>::value
        >,
        typename T::value_type
    >
>: std::true_type {};


template<typename T, typename = std::void_t<>>
struct IsKeyValueContainer: std::false_type {};

template<typename T>
struct IsKeyValueContainer<
    T,
    std::void_t<
        std::enable_if_t<
            jive::IsIterable<T>::value
            && jive::IsMapLike<T>::value
        >
    >
>: std::true_type {};


template<typename T>
struct IsBitset : std::false_type {};

template<size_t N>
struct IsBitset<std::bitset<N>> : std::true_type {};


template<typename T, typename Enable = void>
struct HasOutputStreamOperator: std::false_type {};

template<typename T>
struct HasOutputStreamOperator
<
    T,
    std::enable_if_t<
        std::is_same_v
        <
            std::ostream &,
            decltype(
                std::declval<std::ostream &>() << std::declval<const T &>())
        >
    >
>: std::true_type {};


template<typename U, typename ...Ts>
concept IsOneOf = std::disjunction_v<std::is_same<U, Ts>...>;


template<typename ...Ts>
struct TupleContains_
{
    static constexpr bool value = false;
};

template<typename U, typename ...Ts>
struct TupleContains_<U, std::tuple<Ts...>>
{
    static constexpr bool value = IsOneOf<U, Ts...>;
};

template<typename U, typename T>
concept TupleContains = TupleContains_<U, T>::value;


} // end namespace jive
