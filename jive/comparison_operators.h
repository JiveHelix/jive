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
struct HasLess_: std::false_type {};

template<typename T>
struct HasLess_<
    T,
    std::void_t<
        decltype(std::declval<T>() < std::declval<T>())
    >
>: std::true_type {};

template<typename T>
inline constexpr bool HasLess = HasLess_<T>::value;


template<typename T, typename = void>
struct HasGreater_: std::false_type {};

template<typename T>
struct HasGreater_<
    T,
    std::void_t<
        decltype(std::declval<T>() > std::declval<T>())
    >
>: std::true_type {};

template<typename T>
inline constexpr bool HasGreater = HasGreater_<T>::value;


template<typename T, typename = void>
struct HasLessEqual_: std::false_type {};

template<typename T>
struct HasLessEqual_<
    T,
    std::void_t<
        decltype(std::declval<T>() <= std::declval<T>())
    >
>: std::true_type {};

template<typename T>
inline constexpr bool HasLessEqual = HasLessEqual_<T>::value;


template<typename T, typename = void>
struct HasGreaterEqual_: std::false_type {};

template<typename T>
struct HasGreaterEqual_<
    T,
    std::void_t<
        decltype(std::declval<T>() >= std::declval<T>())
    >
>: std::true_type {};

template<typename T>
inline constexpr bool HasGreaterEqual = HasGreaterEqual_<T>::value;


template<typename T, typename = void>
struct HasEqualTo_: std::false_type {};

template<typename T>
struct HasEqualTo_<
    T,
    std::void_t<
        decltype(std::declval<T>() == std::declval<T>())
    >
>: std::true_type {};

template<typename T>
inline constexpr bool HasEqualTo = HasEqualTo_<T>::value;


template<typename T, typename = void>
struct HasNotEqualTo_: std::false_type {};

template<typename T>
struct HasNotEqualTo_<
    T,
    std::void_t<
        decltype(std::declval<T>() != std::declval<T>())
    >
>: std::true_type {};

template<typename T>
inline constexpr bool HasNotEqualTo = HasNotEqualTo_<T>::value;


template<typename T, typename = void>
struct HasMemberLess_: std::false_type {};

template<typename T>
struct HasMemberLess_<
    T,
    std::void_t<
        decltype(std::declval<T>().operator<(std::declval<T>()))
    >
>: std::true_type {};

template<typename T>
inline constexpr bool HasMemberLess = HasMemberLess_<T>::value;


template<typename T, typename = void>
struct HasMemberGreater_: std::false_type {};

template<typename T>
struct HasMemberGreater_<
    T,
    std::void_t<
        decltype(std::declval<T>().operator>(std::declval<T>()))
    >
>: std::true_type {};

template<typename T>
inline constexpr bool HasMemberGreater = HasMemberGreater_<T>::value;


template<typename T, typename = void>
struct HasMemberLessEqual_: std::false_type {};

template<typename T>
struct HasMemberLessEqual_<
    T,
    std::void_t<
        decltype(std::declval<T>().operator<=(std::declval<T>()))
    >
>: std::true_type {};

template<typename T>
inline constexpr bool HasMemberLessEqual = HasMemberLessEqual_<T>::value;


template<typename T, typename = void>
struct HasMemberGreaterEqual_: std::false_type {};

template<typename T>
struct HasMemberGreaterEqual_<
    T,
    std::void_t<
        decltype(std::declval<T>().operator>=(std::declval<T>()))
    >
>: std::true_type {};

template<typename T>
inline constexpr bool HasMemberGreaterEqual = HasMemberGreaterEqual_<T>::value;


template<typename T, typename = void>
struct HasMemberEqualTo_: std::false_type {};

template<typename T>
struct HasMemberEqualTo_<
    T,
    std::void_t<
        decltype(std::declval<T>().operator==(std::declval<T>()))
    >
>: std::true_type {};

template<typename T>
inline constexpr bool HasMemberEqualTo = HasMemberEqualTo_<T>::value;


template<typename T, typename = void>
struct HasMemberNotEqualTo_: std::false_type {};

template<typename T>
struct HasMemberNotEqualTo_<
    T,
    std::void_t<
        decltype(std::declval<T>().operator!=(std::declval<T>()))
    >
>: std::true_type {};

template<typename T>
inline constexpr bool HasMemberNotEqualTo = HasMemberNotEqualTo_<T>::value;


} // end namespace jive
