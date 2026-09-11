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
#include "jive/type_traits.h"

namespace jive
{

template<typename T, typename Enable = void>
struct DescribeType {};

template<typename, typename = std::void_t<>>
struct HasStaticDescribeType: std::false_type {};

template<typename T>
struct HasStaticDescribeType<
    T,
    std::void_t<decltype(DescribeType<T>::value)>> : std::true_type {};


template<typename T, typename Enable = void>
struct DescribeAnyType
{

};


template<typename T>
struct DescribeAnyType
<
    T,
    std::enable_if_t<HasStaticDescribeType<T>::value>
>
{
    static constexpr std::string_view value = DescribeType<T>::value;
};


template<typename T>
struct DescribeAnyType
<
    T,
    std::enable_if_t<!HasStaticDescribeType<T>::value>
>
{
    static constexpr std::string_view value = detail::TypeName<T>();
};



/** Describe all containers except the map-like ones. **/
template<typename T, typename = std::void_t<>>
struct DescribeContainer
{
    static constexpr std::string_view value =
        jive::StaticJoin<
            detail::ContainerName<T>::value,
            detail::tOpen,
            DescribeAnyType<typename T::value_type>::value,
            detail::tClose>::value;
};

/** Describe both the key_type and mapped_type of the map-like container.**/
template<typename T>
struct DescribeContainer
<
    T,
    std::void_t<std::enable_if_t<jive::IsMapLike<T>>>
>
{
    static constexpr std::string_view value =
        jive::StaticJoin<
            detail::ContainerName<T>::value,
            detail::tOpen,
            DescribeAnyType<typename T::key_type>::value,
            detail::comma,
            DescribeAnyType<typename T::mapped_type>::value,
            detail::tClose>::value;
};


template<typename T>
struct DescribeType<T, std::enable_if_t<detail::IsSupportedContainer<T>::value>>
{
    static constexpr std::string_view value = DescribeContainer<T>::value;
};

template<typename T>
struct DescribeType<T, std::enable_if_t<std::is_fundamental_v<T>>>
{
    static constexpr std::string_view value = detail::DescribeNumeric<T>::value;
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


template<typename T>
consteval std::enable_if_t<HasStaticDescribeType<T>::value, std::string_view>
GetTypeName()
{
    return DescribeType<T>::value;
}

template<typename T>
consteval std::enable_if_t<!HasStaticDescribeType<T>::value, std::string_view>
GetTypeName()
{
    return detail::TypeName<T>();
}

} // end namespace jive
