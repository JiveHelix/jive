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

#include <type_traits>


namespace jive
{


template<typename T, typename = std::void_t<>>
struct IsIterable: std::false_type {};

template<typename T>
struct IsIterable<
    T,
    std::void_t<
        std::enable_if_t<
            std::is_same_v<
                decltype(std::declval<T>().begin()),
                decltype(std::declval<T>().end())
            >
        >
    >
>: std::true_type {};


/** If the container defines key_type and mapped_type, it is close enough
 ** to be considered map-like for our purposes.**/
template<typename T, typename = std::void_t<>>
struct IsMapLike: public std::false_type {};

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


} // end namespace jive
