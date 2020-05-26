/**
  * @file property.h
  *
  * @brief Create read-only and/or assignable properties that reference class
  * data members without the verbosity of getter and setter methods.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 24 May 2020
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <functional>
#include <type_traits>

/**

Any class that contains a Property will have it's assignment operators
implicitly deleted, because Property wraps T by reference. Assignment operators
will have to be created explicitly, but do not copy the instances of Property
in your assignment operators; they still reference their respective instance
members

**/

namespace jive
{


template<typename T>
class Property
{
public:
    explicit Property(T &value): value_{value} {}

    // Implicit converstion to const reference of wrapped value.
    operator const T & () const { return this->value_; }

    // Explicit conversion to const reference using call operator.
    const T & operator()() const { return this->value_; }

    bool operator==(const T &other) const { return (this->value_ == other); }
    bool operator!=(const T &other) const { return (this->value_ != other); }
    bool operator<(const T &other) const { return (this->value_ < other); }
    bool operator<=(const T &other) const { return (this->value_ <= other); }
    bool operator>(const T &other) const { return (this->value_ > other); }
    bool operator>=(const T &other) const { return (this->value_ >= other); }

protected:
    T &value_;
};


template<typename T>
using OnAssign = std::function<void (T)>;


template<typename T, typename Base = Property<T>>
class AssignableProperty: public Base
{
public:
    explicit AssignableProperty(T &value, OnAssign<T> &&onAssign = [](T) {})
        :
        Base(value),
        onAssign_{std::move(onAssign)}
    {

    }

    AssignableProperty & operator=(T other)
    {
        this->value_ = other;
        this->onAssign_(other);
        return *this;
    }

protected:
    OnAssign<T> onAssign_;
};



/** Provides getter and setter properties that map to function calls instead of
 ** referencing a protected data member. The value is computed when requested, and is returned by reference after being cached internally.
 **/
template<typename T>
class PropertyFunctor
{
public:
    using Getter = std::function<T (void)>;

    explicit PropertyFunctor(Getter &&getter): getter_(std::move(getter))
    {

    }

    operator const T & () const
    {
        this->value_ = this->getter_();
        return this->value_;
    }

    const T & operator()() const { return static_cast<const T &>(*this); }

protected:
    Getter getter_;
    T value_;
};


} // end namespace jive
