/**
  * @file to_float.h
  *
  * @brief Convert strings to float, double, or long double.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 08 Jul 2020
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <stdexcept>
#include <type_traits>
#include <string_view>
#include <iostream>
#include <optional>


namespace jive
{

template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, T>
ToFloat(std::string_view asString)
{
    if constexpr (std::is_same_v<float, T>)
    {
        return std::stof(asString.data());
    }
    else if constexpr (std::is_same_v<double, T>)
    {
        return std::stod(asString.data());
    }
    else
    {
        static_assert(std::is_same_v<long double, T>);
        return std::stold(asString.data());
    }
}


template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, std::optional<T>>
MaybeFloat(std::string_view asString)
{
    T result;
    size_t end;

    try
    {
        if constexpr (std::is_same_v<float, T>)
        {
            result = std::stof(asString.data(), &end);
        }
        else if constexpr (std::is_same_v<double, T>)
        {
            result = std::stod(asString.data(), &end);
        }
        else
        {
            static_assert(std::is_same_v<long double, T>);
            result = std::stold(asString.data(), &end);
        }
    }
    catch (std::invalid_argument &)
    {
#ifndef NDEBUG
        std::cerr << "Unable to convert " << asString << std::endl;
#endif
        return {};
    }
    catch (std::out_of_range &)
    {
#ifndef NDEBUG
        std::cerr << "Result is out of range: " << asString << std::endl;
#endif
        return {};
    }

    if (end != asString.size())
    {
        return {};
    }

    return result;
}


} // end namespace jive
