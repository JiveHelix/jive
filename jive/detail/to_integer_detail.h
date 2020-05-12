/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <type_traits>
#include <cwchar>

namespace jive
{

namespace detail
{

template<typename T, int base, typename String>
auto ToInteger(const String &asString)
{
    if constexpr (std::is_same_v<typename String::value_type, wchar_t>)
    {
        if constexpr (std::is_signed<T>::value)
        {
            return wcstoll(asString.data(), nullptr, base);
        }
        else
        {
            return wcstoull(asString.data(), nullptr, base);      
        }
    }
    else
    {
        if constexpr (std::is_signed<T>::value)
        {
            return strtoll(asString.data(), nullptr, base);
        }
        else
        {
            return strtoull(asString.data(), nullptr, base);      
        }
    }
}

template<typename T, int base, typename Enable = void>
struct AllowedDigits {};

template<typename T>
struct AllowedDigits<T, 2, std::enable_if_t<std::is_unsigned_v<T>>>
{
    static constexpr std::string_view value = "+01";
};

template<typename T>
struct AllowedDigits<T, 2, std::enable_if_t<std::is_signed_v<T>>>
{
    static constexpr std::string_view value = "-+01";
};

template<typename T>
struct AllowedDigits<T, 8, std::enable_if_t<std::is_unsigned_v<T>>>
{
    static constexpr std::string_view value = "+01234567";
};

template<typename T>
struct AllowedDigits<T, 8, std::enable_if_t<std::is_signed_v<T>>>
{
    static constexpr std::string_view value = "-+01234567";
};

template<typename T>
struct AllowedDigits<T, 10, std::enable_if_t<std::is_unsigned_v<T>>>
{
    static constexpr std::string_view value = "+0123456789";
};

template<typename T>
struct AllowedDigits<T, 10, std::enable_if_t<std::is_signed_v<T>>>
{
    static constexpr std::string_view value = "-+0123456789";
};

template<typename T>
struct AllowedDigits<T, 16, std::enable_if_t<std::is_unsigned_v<T>>>
{
    static constexpr std::string_view value = "+0123456789AaBbCcDdEeFfxX";
};

template<typename T>
struct AllowedDigits<T, 16, std::enable_if_t<std::is_signed_v<T>>>
{
    static constexpr std::string_view value = "-+0123456789AaBbCcDdEeFfxX";
};

template<typename T, int base, typename String>
constexpr bool ValidateDigits(const String &input)
{
    auto allowedDigits = AllowedDigits<T, base>::value;

    for (auto &digit: input)
    {
        if (allowedDigits.find(digit) == std::string_view::npos)
        {
            // digit not allowed for this base
            return false;
        }
    }

    return true;
}

} // end namespace detail

} // end namespace jive
