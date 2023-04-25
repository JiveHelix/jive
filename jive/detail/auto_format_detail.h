/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <string_view>
#include <type_traits>


namespace jive
{

namespace detail
{

template<typename T, typename Enable = void>
struct LengthModifier {};

template<typename T>
struct LengthModifier<T, std::enable_if_t<sizeof(T) == 1>>
{
    static constexpr std::string_view value = "hh";
};

template<typename T>
struct LengthModifier<T, std::enable_if_t<sizeof(T) == 2>>
{
    static constexpr std::string_view value = "h";
};

template<typename T>
struct LengthModifier<T, std::enable_if_t<sizeof(T) == 4>>
{
    static constexpr std::string_view value = "";
};

template<typename T>
struct LengthModifier<T, std::enable_if_t<sizeof(T) == 8>>
{
    static constexpr std::string_view value = "l";
};

template<typename T>
struct LengthModifier<
    T,
    std::enable_if_t<(sizeof(T) == 16 && std::is_integral_v<T>)>>
{
    static constexpr std::string_view value = "ll";
};

template<typename T>
struct LengthModifier<
    T,
    std::enable_if_t<(sizeof(T) == 16 && std::is_floating_point_v<T>)>>
{
    static constexpr std::string_view value = "L";
};

template<>
struct LengthModifier<std::size_t, std::void_t<>>
{
    static constexpr std::string_view value = "z";
};

template<typename T, int base, typename Enable = void>
struct FormatSpecifier {};

template<typename T>
struct FormatSpecifier<T, 10, std::enable_if_t<std::is_floating_point_v<T>>>
{
    static constexpr std::string_view value = "g";
};

template<typename T>
struct FormatSpecifier<T, 16, std::enable_if_t<std::is_floating_point_v<T>>>
{
    static constexpr std::string_view value = "a";
};

template<typename T>
struct FormatSpecifier<
    T,
    10,
    std::enable_if_t<(std::is_integral_v<T> && std::is_signed_v<T>)>>
{
    static constexpr std::string_view value = "d";
};

template<typename T>
struct FormatSpecifier<
    T,
    10,
    std::enable_if_t<(std::is_integral_v<T> && std::is_unsigned_v<T>)>>
{
    static constexpr std::string_view value = "u";
};

template<typename T>
struct FormatSpecifier<
    T,
    8,
    std::enable_if_t<std::is_integral_v<T>>>
{
    static constexpr std::string_view value = "o";
};

template<typename T>
struct FormatSpecifier<
    T,
    16,
    std::enable_if_t<std::is_integral_v<T>>>
{
    static constexpr std::string_view value = "x";
};


template<typename T, int base, typename Enable = void>
struct FixedSpecifier
{
    static constexpr std::string_view value =
        FormatSpecifier<T, base>::value;
};

template<typename T>
struct FixedSpecifier<T, 10, std::enable_if_t<std::is_floating_point_v<T>>>
{
    static constexpr std::string_view value = "f";
};

template<typename T, int base, typename Enable = void>
struct ScientificSpecifier
{
    static constexpr std::string_view value =
        FormatSpecifier<T, base>::value;
};

template<typename T>
struct ScientificSpecifier<T, 10, std::enable_if_t<std::is_floating_point_v<T>>>
{
    static constexpr std::string_view value = "e";
};


} // end namespace detail

} // end namespace jive
