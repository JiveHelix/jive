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
#include <string>
#include <iostream>


namespace jive
{

template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, T>
ToFloat(const std::string &asString)
{
    if constexpr (std::is_same_v<float, T>)
    {
        return std::stof(asString);
    }
    else if constexpr (std::is_same_v<double, T>)
    {
        return std::stod(asString);
    }
    else
    {
        static_assert(std::is_same_v<long double, T>);
        return std::stold(asString);
    }
}


template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, std::optional<T>>
MaybeFloat(const std::string &asString)
{
    T result;
    size_t end;

    try
    {
        if constexpr (std::is_same_v<float, T>)
        {
            result = std::stof(asString, &end);
        }
        else if constexpr (std::is_same_v<double, T>)
        {
            result = std::stod(asString, &end);
        }
        else
        {
            static_assert(std::is_same_v<long double, T>);
            result = std::stold(asString, &end);
        }
    }
    catch (std::invalid_argument &)
    {
        std::cerr << "Unable to convert " << asString << std::endl;
        return {};
    }
    catch (std::out_of_range &)
    {
        std::cerr << "Result is out of range: " << asString << std::endl;
        return {};
    }

    if (end != asString.size())
    {
        return {};
    }

    return result;
}


} // end namespace jive
