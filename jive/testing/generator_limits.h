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

#include "jive/testing/cast_limits.h"
#include "jive/testing/random_type.h"


template<typename T, typename Enable = void>
struct GeneratorType {};

template<typename T>
struct GeneratorType<T, std::enable_if_t<std::is_floating_point_v<T>>>
{
    using type = T;
};

template<typename T>
struct GeneratorType<T, std::enable_if_t<std::is_integral_v<T>>>
{
    using type = typename RandomType<T>::type;
};


template<typename T, typename Enable = void>
struct GeneratorLimits {};

template<typename T>
struct GeneratorLimits<T, std::enable_if_t<std::is_integral_v<T>>>
{
    using Target = typename GeneratorType<T>::type;

    static constexpr Target Lowest()
    {
        return static_cast<Target>(std::numeric_limits<T>::lowest());
    }

    static constexpr Target Max()
    {
        return static_cast<Target>(std::numeric_limits<T>::max());
    }
};


template<typename T>
struct GeneratorLimits<T, std::enable_if_t<std::is_floating_point_v<T>>>
{
    using Target = typename GeneratorType<T>::type;

    static constexpr Target Lowest()
    {
        return static_cast<Target>(
                std::numeric_limits<T>::lowest()
                / static_cast<T>(2));
    }

    static constexpr Target Max()
    {
        return static_cast<Target>(
                std::numeric_limits<T>::max()
                / static_cast<T>(2));
    }
};
