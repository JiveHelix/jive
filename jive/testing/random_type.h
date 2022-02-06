#pragma once

#include <type_traits>


template<typename T, typename Enable = void>
struct RandomType {};

template<typename T>
struct RandomType<T, std::enable_if_t<std::is_signed_v<T>>>
{
    using type = int64_t;
};

template<typename T>
struct RandomType<T, std::enable_if_t<std::is_unsigned_v<T>>>
{
    using type = uint64_t;
};
