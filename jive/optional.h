#pragma once


#include <optional>


namespace jive
{


template<typename T>
struct IsOptional_: std::false_type {};

template<typename T>
struct IsOptional_<std::optional<T>>: std::true_type {};

template<typename T>
inline constexpr bool IsOptional = IsOptional_<T>::value;


template<typename T, typename Enable = void>
struct RemoveOptional_
{
    using Type = T;
};


template<typename T>
struct RemoveOptional_
<
    T,
    std::enable_if_t<IsOptional<T>>
>
{
    using Type = typename T::value_type;
};


template<typename T>
using RemoveOptional = typename RemoveOptional_<T>::Type;



template<typename T, typename Enable = void>
struct MakeOptional_
{
    using Type = std::optional<T>;
};


template<typename T>
struct MakeOptional_
<
    T,
    std::enable_if_t<IsOptional<T>>
>
{
    using Type = T;
};


template<typename T>
using MakeOptional = typename MakeOptional_<T>::Type;




template<typename T, typename U, typename Enable = void>
struct MatchOptional_
{
    using Type = U;
};


template<typename T, typename U>
struct MatchOptional_
<
    T,
    U,
    std::enable_if_t<IsOptional<T>>
>
{
    using Type = std::optional<U>;
};


template<typename T, typename U>
using MatchOptional = typename MatchOptional_<T, U>::Type;


} // end namespace jive
