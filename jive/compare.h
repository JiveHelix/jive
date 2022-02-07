/**
  * @file compare.h
  * 
  * @brief Create a class with comparison operators for a single member. This
  * is obsoleted by operator<=>.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 15 Jul 2020
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <utility>


namespace jive
{

/** The type of a function is defined by its arguments and return value. All of
 ** the comparison operators have the same signature, so they will not be
 ** considered distinct types. Create them as structs with a static Call
 ** function to execute the comparison.
 **/

namespace detail
{

struct Equal
{
    template <typename Left, typename Right>
    static bool Call(Left &&left, Right &&right)
    {
        return std::forward<Left>(left) == std::forward<Right>(right);
    }
};

struct NotEqual
{
    template <typename Left, typename Right>
    static bool Call(Left &&left, Right &&right)
    {
        return std::forward<Left>(left) != std::forward<Right>(right);
    }
};

struct LessThan
{
    template <typename Left, typename Right>
    static bool Call(Left &&left, Right &&right)
    {
        return std::forward<Left>(left) < std::forward<Right>(right);
    }
};

struct GreaterThan
{
    template <typename Left, typename Right>
    static bool Call(Left &&left, Right &&right)
    {
        return std::forward<Left>(left) > std::forward<Right>(right);
    }
};

struct LessThanOrEqual
{
    template <typename Left, typename Right>
    static bool Call(Left &&left, Right &&right)
    {
        return std::forward<Left>(left) <= std::forward<Right>(right);
    }
};

struct GreaterThanOrEqual
{
    template <typename Left, typename Right>
    static bool Call(Left &&left, Right &&right)
    {
        return std::forward<Left>(left) >= std::forward<Right>(right);
    }
};

} // namespace detail


/**
 ** @class Compare
 **
 ** The only function required for derived classes is following:

    template <typename Operator>
    bool Compare(const T &right) const
    {
        // return Operator::Call(...);
    }

 ** where Operator::Call is a binary function accepting any two comparable
 ** types.
 **/

template <typename T>
struct Compare
{
    // Note: `friend` functions are non-member functions.
    // This allows us to override the comparison operators without any casting
    // to the derived type.

    friend bool operator==(const T &left, const T &right)
    {
        return left.template Compare<detail::Equal>(right);
    }

    friend bool operator!=(const T &left, const T &right)
    {
        return left.template Compare<detail::NotEqual>(right);
    }

    friend bool operator<(const T &left, const T &right)
    {
        return left.template Compare<detail::LessThan>(right);
    }

    friend bool operator>(const T &left, const T &right)
    {
        return left.template Compare<detail::GreaterThan>(right);
    }

    friend bool operator<=(const T &left, const T &right)
    {
        return left.template Compare<detail::LessThanOrEqual>(right);
    }

    friend bool operator>=(const T &left, const T &right)
    {
        return left.template Compare<detail::GreaterThanOrEqual>(right);
    }
};

} // namespace jive


#ifdef BUILD_COMPARE_EXAMPLE

#include <iostream>

template<typename T>
class Foo: jive::Compare<Foo<T>>
{
public:
    Foo(T value): value_(value) {}

    template <typename Operator>
    bool Compare(const Foo &other) const
    {
        return Operator::Call(this->value_, other.value_);
    }

private:
    T value_;
};


int main()
{
    Foo<double> p{3.14};
    Foo<double> q{3.13999};

    std::cout << std::boolalpha
        << "p == q: " << (p == q)
        << ", p != q: " << (p != q)
        << ", p < q: " << (p < q)
        << ", p > q: " << (p > q)
        << ", p <= q: " << (p <= q)
        << ", p >= q: " << (p >= q) << std::endl;;

    return 0;
}
#endif
