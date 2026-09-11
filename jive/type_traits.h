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
#include <array>


namespace jive
{


template<typename T, typename = void>
struct IsIterable_: std::false_type {};

template<typename T>
struct IsIterable_
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

template<typename T>
concept IsIterable = IsIterable_<T>::value;


/** If the container defines key_type and mapped_type, it is close enough
 ** to be considered map-like for our purposes.**/
template<typename T, typename = std::void_t<>>
struct IsMapLike_: std::false_type {};

template<typename T>
struct IsMapLike_<
    T,
    std::void_t<
        typename T::key_type,
        typename T::mapped_type>>: public std::true_type {};

template<typename T>
concept IsMapLike = IsMapLike_<T>::value;


template<typename T>
struct IsString_: std::false_type {};

template<>
struct IsString_<std::string>: std::true_type {};

template<typename T>
concept IsString = IsString_<T>::value;

template<typename T>
struct IsArray_: std::false_type {};

template<typename T, size_t N>
struct IsArray_<std::array<T, N>>: std::true_type {};

template<typename T>
concept IsArray = IsArray_<T>::value;


template<typename T, typename = std::void_t<>>
struct IsValueContainer_: std::false_type {};

template<typename T>
struct IsValueContainer_<
    T,
    std::void_t<
        std::enable_if_t<
            jive::IsIterable<T>
            && std::is_integral_v<decltype(std::declval<T>().size())>
            && !IsArray<T>
            && !IsMapLike<T>
            && !IsString<T>
        >,
        typename T::value_type
    >
>: std::true_type {};


template<typename T>
concept IsValueContainer = IsValueContainer_<T>::value;


template<typename T, typename = std::void_t<>>
struct IsKeyValueContainer_: std::false_type {};

template<typename T>
struct IsKeyValueContainer_<
    T,
    std::void_t<
        std::enable_if_t<
            jive::IsIterable<T>
            && jive::IsMapLike<T>
        >
    >
>: std::true_type {};


template<typename T>
concept IsKeyValueContainer = IsKeyValueContainer_<T>::value;


template<typename T>
struct IsBitset_: std::false_type {};

template<size_t N>
struct IsBitset_<std::bitset<N>> : std::true_type {};

template<typename T>
concept IsBitset = IsBitset_<T>::value;


template<typename T, typename Enable = void>
struct HasOutputStreamOperator_: std::false_type {};

template<typename T>
struct HasOutputStreamOperator_
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

template<typename T>
concept HasOutputStreamOperator = HasOutputStreamOperator_<T>::value;


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
