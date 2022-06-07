/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <limits>

template<typename T, typename = void>
struct TargetHelper
{
    using Type = T;
};

template<typename T>
struct TargetHelper
<
    T,
    std::enable_if_t
    <
        sizeof(T) == 1 && std::is_signed_v<T>
    >
>
{
    using Type = int16_t;
};

template<typename T>
struct TargetHelper
<
    T,
    std::enable_if_t
    <
        sizeof(T) == 1 && !std::is_signed_v<T>
    >
>
{
    using Type = uint16_t;
};


template<typename T, typename Target_ = T>
struct CastLimits
{
    using Target = typename TargetHelper<Target_>::Type;

    static constexpr Target Min()
    {
        return static_cast<Target>(std::numeric_limits<T>::min());
    }

    static constexpr Target Max()
    {
        return static_cast<Target>(std::numeric_limits<T>::max());
    }
};
