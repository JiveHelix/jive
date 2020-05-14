/**
  * @file describe_type.h
  * 
  * @brief Provides a string representation of any type. For fundamental types
  * and common STL containers, the name is created statically by DescribeType.
  * For integral types, the format is int8_t, uint8_t, int16_t, uint16_t, etc.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 12 May 2020
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  *
  */

#pragma once

#include <string>

#include "jive/detail/describe_type_detail.h"

namespace jive
{

template<typename T, typename Enable = void>
struct DescribeType {};


/** Describe all containers except the map-like ones. **/
template<typename T, typename = std::void_t<>>
struct DescribeContainer
{
    static constexpr auto value =
        jive::StaticJoin<
            detail::ContainerName<T>::value,
            detail::tOpen,
            DescribeType<typename T::value_type>::value,
            detail::tClose>::value;
};

/** Describe both the key_type and mapped_type of the map-like container.**/
template<typename T>
struct DescribeContainer<
    T,
    std::void_t<std::enable_if_t<detail::IsMapLike<T>::value>>>
{
    static constexpr auto value =
        jive::StaticJoin<
            detail::ContainerName<T>::value,
            detail::tOpen,
            DescribeType<typename T::key_type>::value,
            detail::comma,
            DescribeType<typename T::mapped_type>::value,
            detail::tClose>::value;
};


template<typename T>
struct DescribeType<T, std::enable_if_t<detail::IsSupportedContainer<T>::value>>
{
    static constexpr auto value = DescribeContainer<T>::value;
};

template<typename T>
struct DescribeType<T, std::enable_if_t<std::is_fundamental_v<T>>>
{
    static constexpr auto value = detail::DescribeNumeric<T>::value;
};

template<>
struct DescribeType<std::string>
{
    static constexpr std::string_view value = "string";
};

template<>
struct DescribeType<std::wstring>
{
    static constexpr std::string_view value = "wstring";
};


template<typename, typename = std::void_t<>>
struct HasStaticDescribeType: std::false_type {};

template<typename T>
struct HasStaticDescribeType<
    T,
    std::void_t<decltype(DescribeType<T>::value)>> : std::true_type {};


template<typename T>
constexpr std::enable_if_t<HasStaticDescribeType<T>::value, std::string_view>
GetTypeName()
{
    return DescribeType<T>::value;
}

template<typename T>
std::enable_if_t<!HasStaticDescribeType<T>::value, std::string>
GetTypeName()
{
    return detail::DemangleTypeName<T>();
}

} // end namespace jive
