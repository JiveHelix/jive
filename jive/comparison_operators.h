/**
  * @file comparison_operators.h
  * 
  * @brief Checks for comparison operators.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 31 Aug 2021
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <type_traits>


namespace jive
{


template<typename T, typename = void>
struct HasLess: std::false_type {};

template<typename T>
struct HasLess<
    T,
    std::void_t<
        decltype(std::declval<T>() < std::declval<T>())
    >
>: std::true_type {};


template<typename T, typename = void>
struct HasGreater: std::false_type {};

template<typename T>
struct HasGreater<
    T,
    std::void_t<
        decltype(std::declval<T>() > std::declval<T>())
    >
>: std::true_type {};


template<typename T, typename = void>
struct HasLessEqual: std::false_type {};

template<typename T>
struct HasLessEqual<
    T,
    std::void_t<
        decltype(std::declval<T>() <= std::declval<T>())
    >
>: std::true_type {};


template<typename T, typename = void>
struct HasGreaterEqual: std::false_type {};

template<typename T>
struct HasGreaterEqual<
    T,
    std::void_t<
        decltype(std::declval<T>() >= std::declval<T>())
    >
>: std::true_type {};


template<typename T, typename = void>
struct HasEqualTo: std::false_type {};

template<typename T>
struct HasEqualTo<
    T,
    std::void_t<
        decltype(std::declval<T>() == std::declval<T>())
    >
>: std::true_type {};


template<typename T, typename = void>
struct HasNotEqualTo: std::false_type {};

template<typename T>
struct HasNotEqualTo<
    T,
    std::void_t<
        decltype(std::declval<T>() != std::declval<T>())
    >
>: std::true_type {};


template<typename T, typename = void>
struct HasMemberLess: std::false_type {};

template<typename T>
struct HasMemberLess<
    T,
    std::void_t<
        decltype(std::declval<T>().operator<(std::declval<T>()))
    >
>: std::true_type {};


template<typename T, typename = void>
struct HasMemberGreater: std::false_type {};

template<typename T>
struct HasMemberGreater<
    T,
    std::void_t<
        decltype(std::declval<T>().operator>(std::declval<T>()))
    >
>: std::true_type {};


template<typename T, typename = void>
struct HasMemberLessEqual: std::false_type {};

template<typename T>
struct HasMemberLessEqual<
    T,
    std::void_t<
        decltype(std::declval<T>().operator<=(std::declval<T>()))
    >
>: std::true_type {};


template<typename T, typename = void>
struct HasMemberGreaterEqual: std::false_type {};

template<typename T>
struct HasMemberGreaterEqual<
    T,
    std::void_t<
        decltype(std::declval<T>().operator>=(std::declval<T>()))
    >
>: std::true_type {};


template<typename T, typename = void>
struct HasMemberEqualTo: std::false_type {};

template<typename T>
struct HasMemberEqualTo<
    T,
    std::void_t<
        decltype(std::declval<T>().operator==(std::declval<T>()))
    >
>: std::true_type {};


template<typename T, typename = void>
struct HasMemberNotEqualTo: std::false_type {};

template<typename T>
struct HasMemberNotEqualTo<
    T,
    std::void_t<
        decltype(std::declval<T>().operator!=(std::declval<T>()))
    >
>: std::true_type {};


} // end namespace jive
