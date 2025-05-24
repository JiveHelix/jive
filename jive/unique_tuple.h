#pragma once



namespace jive
{


 // UniqueTuple takes in variadic type arguments, and returns a tuple without
// any repeated types. Every element of the tuple is unique.
template <typename T, typename... Ts>
struct UniqueArgs_ : std::type_identity<T> {};

template <typename ...Ts, typename U, typename ...Us>
struct UniqueArgs_<std::tuple<Ts...>, U, Us...>
    :
    std::conditional_t
    <
        std::disjunction_v<std::is_same<U, Ts>...>,

        // U is the same as one of the T's.
        // It will not be included a second time.
        // Continue checking the remaining U's.
        UniqueArgs_<std::tuple<Ts...>, Us...>,

        // U is not already in the set of T's.
        // Add it to the unique tuple.
        // Continue checking the remaining U's.
        UniqueArgs_<std::tuple<Ts..., U>, Us...>
    > {};

template <typename ...Ts>
using UniqueArgs = typename UniqueArgs_<std::tuple<>, Ts...>::type;


template<typename Tuple> struct UniqueTuple_;

template<typename... Ts>
struct UniqueTuple_<std::tuple<Ts...>>
{
    using type = UniqueArgs<Ts...>;
};


template<typename Tuple>
using UniqueTuple = typename UniqueTuple_<Tuple>::type;


} // end namespace jive
