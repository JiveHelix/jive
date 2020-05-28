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

#include <type_traits>

/**

Any class that contains a Property will have it's assignment operators
implicitly deleted, because Property wraps Owner by reference. Assignment
operators will have to be created explicitly, but do not copy the instances of
Property in your assignment operators; they still reference their respective
instance members

**/

namespace jive
{


template<typename T, typename Owner, T Owner::* Member>
struct PropertyTraits
{
    using type = T;
    using owner = Owner;
    constexpr static auto member = Member;
};


template<typename Traits>
class Property
{
public:
    using T = typename Traits::type;
    using Owner = typename Traits::owner;
    constexpr static auto member = Traits::member;

    explicit Property(Owner &owner): owner_{owner} {}

    // Implicit conversion to const reference of wrapped value.
    operator const T & () const { return this->owner_.*member; }

    // Explicit conversion to const reference using call operator.
    const T & operator()() const { return this->owner_.*member; }

    bool operator==(const T &other) const { return ((*this)() == other); }
    bool operator!=(const T &other) const { return ((*this)() != other); }
    bool operator<(const T &other) const { return ((*this)() < other); }
    bool operator<=(const T &other) const { return ((*this)() <= other); }
    bool operator>(const T &other) const { return ((*this)() > other); }
    bool operator>=(const T &other) const { return ((*this)() >= other); }

protected:
    Owner &owner_;
};


template<
    typename T,
    typename Owner,
    T Owner::* Member,
    void(Owner::*OnAssign)(void)>
struct AssignableTraits
{
    using type = T;
    using owner = Owner;
    constexpr static auto member = Member;
    constexpr static auto onAssign = OnAssign;
};


template<typename Traits, typename Base = Property<Traits>>
class AssignableProperty: public Base
{
public:
    using T = typename Traits::type;
    using Owner = typename Traits::owner;
    constexpr static auto member = Traits::member;
    constexpr static auto OnAssign = Traits::onAssign;

    explicit AssignableProperty(Owner &owner)
        :
        Base(owner)
    {

    }

    AssignableProperty & operator=(T other)
    {
        this->owner_.*member = other;
        (this->owner_.*(OnAssign))();
        return *this;
    }
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
