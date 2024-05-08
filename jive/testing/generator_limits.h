/**
  * @file generator_limits.h
  *
  * @brief Helper for catch2 generators.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 28 Mar 2022
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once


#include <type_traits>


template<typename T, typename Enable = void>
struct IntegralGeneratorType_
{
};

template<typename T>
struct IntegralGeneratorType_<T, std::enable_if_t<std::is_signed_v<T>>>
{
    static_assert(std::is_integral_v<T>);
    using Type = int64_t;
};

template<typename T>
struct IntegralGeneratorType_<T, std::enable_if_t<std::is_unsigned_v<T>>>
{
    static_assert(std::is_integral_v<T>);
    using Type = uint64_t;
};


template<typename T>
using IntegralGeneratorType = typename IntegralGeneratorType_<T>::Type;


template<typename T, typename Enable = void>
struct GeneratorType_ {};

template<typename T>
struct GeneratorType_<T, std::enable_if_t<std::is_floating_point_v<T>>>
{
    using Type = T;
};

template<typename T>
struct GeneratorType_<T, std::enable_if_t<std::is_integral_v<T>>>
{
    using Type = IntegralGeneratorType<T>;
};


template<typename T>
using GeneratorType = typename GeneratorType_<T>::Type;


template<typename T, typename Target_ = T>
struct GeneratorLimits
{
    using Target = GeneratorType<Target_>;

    static constexpr Target Lowest()
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            return static_cast<Target>(
                std::numeric_limits<T>::lowest()
                / static_cast<T>(2));
        }
        else
        {
            return static_cast<Target>(std::numeric_limits<T>::lowest());
        }
    }

    static constexpr Target Max()
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            return static_cast<Target>(
                std::numeric_limits<T>::max()
                / static_cast<T>(2));
        }
        else
        {
            return static_cast<Target>(std::numeric_limits<T>::max());
        }
    }
};
