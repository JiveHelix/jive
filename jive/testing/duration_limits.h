/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#pragma once

#undef min

#include <limits>
#include <chrono>
#include "jive/power.h"


template<typename T>
using TypeLimit = std::numeric_limits<T>;

template<typename T, typename Enable = void>
struct Limits;

template<typename T>
struct Limits<T, std::enable_if_t<std::is_signed_v<T>>>
{
    // Test TimeValue at least 100 years into the future and the past.
    static constexpr std::chrono::duration<int64_t> upper{
        5 * jive::Power<10, 9>()};

    static constexpr std::chrono::duration<int64_t> lower{
        -2 * jive::Power<10, 9>()};
};

template<typename T>
struct Limits<T, std::enable_if_t<std::is_unsigned_v<T>>>
{
    // Past 100 years into the future
    static constexpr std::chrono::duration<uint64_t> upper{
        5 * jive::Power<10, 9>()};

    // lower bound for unsigned is 0
    static constexpr std::chrono::duration<uint64_t> lower{0};
};

template<typename Duration, typename Target = typename Duration::rep>
struct DurationLimits
{
    using T = typename Duration::rep;

    static constexpr auto upper = static_cast<Target>(
        (Duration(TypeLimit<T>::max()) > Limits<T>::upper)
        ? Duration(Limits<T>::upper).count()
        : TypeLimit<T>::max());

    static constexpr auto lower = static_cast<Target>(
        (Duration(TypeLimit<T>::min()) < Limits<T>::lower)
        ? Duration(Limits<T>::lower).count()
        : TypeLimit<T>::min());
};
