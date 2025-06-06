/**
  * @file describe_type_detail.h
  *
  * @brief Most of the implementation of describe_type.h
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 12 May 2020
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  *
  */

#pragma once


#include <type_traits>
#include <vector>
#include <deque>
#include <queue>
#include <list>
#include <map>
#include <string_view>
#include <typeinfo>

#ifndef _WIN32
#include <cstdlib>
#include <memory>
#include <cxxabi.h>
#endif // !_WIN32

#include "jive/static_join.h"


namespace jive
{

namespace detail
{

// Pieces of the type descriptions
inline constexpr std::string_view _t = "_t";
inline constexpr std::string_view tOpen = "<";
inline constexpr std::string_view tClose = ">";
inline constexpr std::string_view comma = ", ";


/** Choose the first part of the name using is_signed **/
template<typename T, typename Enable = void>
struct IntegralStem {};

/** Signed types begin with "int" **/
template<typename T>
struct IntegralStem<T, std::enable_if_t<std::is_signed_v<T>>>
{
    static_assert(std::is_integral_v<T>);
    static constexpr std::string_view value = "int";
};

/** Unsigned types begin with "uint" **/
template<typename T>
struct IntegralStem<T, std::enable_if_t<std::is_unsigned_v<T>>>
{
    static_assert(std::is_integral_v<T>);
    static constexpr std::string_view value = "uint";
};


/** Specializations of Bits for 8, 16, 32, 64, and 128, based on sizeof(T) **/
template<typename T, typename Enable = void>
struct Bits {};

template<typename T>
struct Bits<T, std::enable_if_t<sizeof(T) == 1>>
{
    static constexpr std::string_view value = "8";
};

template<typename T>
struct Bits<T, std::enable_if_t<sizeof(T) == 2>>
{
    static constexpr std::string_view value = "16";
};

template<typename T>
struct Bits<T, std::enable_if_t<sizeof(T) == 4>>
{
    static constexpr std::string_view value = "32";
};

template<typename T>
struct Bits<T, std::enable_if_t<sizeof(T) == 8>>
{
    static constexpr std::string_view value = "64";
};

template<typename T>
struct Bits<T, std::enable_if_t<sizeof(T) == 16>>
{
    static constexpr std::string_view value = "128";
};


/** The representation of float is probably 32 bits, and double is 64 bits, but
 ** long double isn't 128 bits. Rather than try to assign a bit width to
 ** floats, use ** float, double, and long double.
 **/
template<typename T>
struct FloatName {};

template<>
struct FloatName<float>
{
    static constexpr std::string_view value = "float";
};

template<>
struct FloatName<double>
{
    static constexpr std::string_view value = "double";
};

template<>
struct FloatName<long double>
{
    static constexpr std::string_view value = "long double";
};


template<typename T, typename Enable = void>
struct DescribeNumeric {};

/** Specialize for floating-point **/
template<typename T>
struct DescribeNumeric<T, std::enable_if_t<std::is_floating_point_v<T>>>
{
    static constexpr std::string_view value = FloatName<T>::value;
};

/** Specialize for integers **/
template<typename T>
struct DescribeNumeric<T, std::enable_if_t<std::is_integral_v<T>>>
{
    static constexpr std::string_view value =
        jive::StaticJoin<IntegralStem<T>::value, Bits<T>::value, _t>::value;
};


/** Specialize for bool **/
template<>
struct DescribeNumeric<bool>
{
    static constexpr std::string_view value = "bool";
};


/** ContainerName is specialized for a limited set of STL containers. **/
template<typename T>
struct ContainerName;

template<typename T>
struct ContainerName<std::vector<T>>
{
    static constexpr std::string_view value = "vector";
};

template<typename Key, typename Value>
struct ContainerName<std::map<Key, Value>>
{
    static constexpr std::string_view value = "map";
};

template<typename T>
struct ContainerName<std::deque<T>>
{
    static constexpr std::string_view value = "deque";
};

template<typename T>
struct ContainerName<std::list<T>>
{
    static constexpr std::string_view value = "list";
};

template<typename T>
struct ContainerName<std::queue<T>>
{
    static constexpr std::string_view value = "queue";
};


/** If ContainerName is specialized for the container, it is supported. **/
template<typename T, typename = std::void_t<>>
struct IsSupportedContainer: public std::false_type {};

template<typename T>
struct IsSupportedContainer<
    T,
    std::void_t<decltype(ContainerName<T>::value)>>: public std::true_type {};


} // end namespace detail


namespace detail
{

template <typename T>
consteval std::string_view TypeName()
{
#if defined(__clang__)

    constexpr std::string_view p = __PRETTY_FUNCTION__;

    constexpr std::string_view prefix =
        "std::string_view jive::detail::TypeName() [T = ";

    constexpr std::string_view suffix = "]";

#elif defined(__GNUC__)

    constexpr std::string_view p = __PRETTY_FUNCTION__;

    constexpr std::string_view prefix =
        "constexpr std::string_view jive::detail::TypeName() [with T = ";

    constexpr std::string_view suffix = "]";

#elif defined(_MSC_VER)

    constexpr std::string_view p = __FUNCSIG__;

    constexpr std::string_view prefix =
        "class std::basic_string_view<char,struct std::char_traits<char> > "
        "__cdecl jive::detail::TypeName<";

    constexpr std::string_view suffix = ">(void)";

#else
#   error Unsupported compiler
#endif

    const auto start = p.find(prefix) + prefix.size();
    const auto end = p.rfind(suffix);

    return p.substr(start, end - start);
}


} // namespace detail


} // end namespace jive
