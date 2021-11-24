/**
  * @file comparison_operator_tests.cpp
  * 
  * @brief Test the compile-time checks for comparison operators.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 31 Aug 2021
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#include <catch2/catch.hpp>

#include "jive/comparison_operators.h"

struct NoOperators
{

};

struct DeclaresLess
{
    bool operator<(const DeclaresLess &) const { return true; }
};

struct DeclaresGreater
{
    bool operator>(const DeclaresGreater &) const { return true; }
};

struct DeclaresEqualTo
{
    bool operator==(const DeclaresEqualTo &) const { return true; }
};

struct DeclaresNotEqualTo
{
    bool operator!=(const DeclaresNotEqualTo &) const { return true; }
};

struct DeclaresLessEqual
{
    bool operator<=(const DeclaresLessEqual &) const { return true; }
};

struct DeclaresGreaterEqual
{
    bool operator>=(const DeclaresGreaterEqual &) const { return true; }
};


TEST_CASE("No operators declared", "[comparison_operators]")
{
    using TypeUnderTest = NoOperators;

    STATIC_REQUIRE(!jive::HasLess<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasGreaterEqual<TypeUnderTest>::value);

    STATIC_REQUIRE(!jive::HasMemberLess<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberGreaterEqual<TypeUnderTest>::value);
}


TEST_CASE("float has comparisons but not as members", "[comparison_operators]")
{
    using TypeUnderTest = float;

    STATIC_REQUIRE(jive::HasLess<TypeUnderTest>::value);
    STATIC_REQUIRE(jive::HasGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(jive::HasEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(jive::HasNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(jive::HasLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(jive::HasGreaterEqual<TypeUnderTest>::value);

    STATIC_REQUIRE(!jive::HasMemberLess<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberGreaterEqual<TypeUnderTest>::value);
}


TEST_CASE("declares member less", "[comparison_operators]")
{
    using TypeUnderTest = DeclaresLess;

    STATIC_REQUIRE(jive::HasLess<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasGreaterEqual<TypeUnderTest>::value);

    STATIC_REQUIRE(jive::HasMemberLess<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberGreaterEqual<TypeUnderTest>::value);
}


TEST_CASE("declares member greater", "[comparison_operators]")
{
    using TypeUnderTest = DeclaresGreater;

    STATIC_REQUIRE(!jive::HasLess<TypeUnderTest>::value);
    STATIC_REQUIRE(jive::HasGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasGreaterEqual<TypeUnderTest>::value);

    STATIC_REQUIRE(!jive::HasMemberLess<TypeUnderTest>::value);
    STATIC_REQUIRE(jive::HasMemberGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberGreaterEqual<TypeUnderTest>::value);
}


TEST_CASE("declares member equal to", "[comparison_operators]")
{
    using TypeUnderTest = DeclaresEqualTo;

    STATIC_REQUIRE(!jive::HasLess<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(jive::HasEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasGreaterEqual<TypeUnderTest>::value);

    STATIC_REQUIRE(!jive::HasMemberLess<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(jive::HasMemberEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberGreaterEqual<TypeUnderTest>::value);
}


TEST_CASE("declares member not equal to", "[comparison_operators]")
{
    using TypeUnderTest = DeclaresNotEqualTo;

    STATIC_REQUIRE(!jive::HasLess<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(jive::HasNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasGreaterEqual<TypeUnderTest>::value);

    STATIC_REQUIRE(!jive::HasMemberLess<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(jive::HasMemberNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberGreaterEqual<TypeUnderTest>::value);
}


TEST_CASE("declares member less equal", "[comparison_operators]")
{
    using TypeUnderTest = DeclaresLessEqual;

    STATIC_REQUIRE(!jive::HasLess<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(jive::HasLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasGreaterEqual<TypeUnderTest>::value);

    STATIC_REQUIRE(!jive::HasMemberLess<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(jive::HasMemberLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberGreaterEqual<TypeUnderTest>::value);
}


TEST_CASE("declares member greater equal", "[comparison_operators]")
{
    using TypeUnderTest = DeclaresGreaterEqual;

    STATIC_REQUIRE(!jive::HasLess<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(jive::HasGreaterEqual<TypeUnderTest>::value);

    STATIC_REQUIRE(!jive::HasMemberLess<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberGreater<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberNotEqualTo<TypeUnderTest>::value);
    STATIC_REQUIRE(!jive::HasMemberLessEqual<TypeUnderTest>::value);
    STATIC_REQUIRE(jive::HasMemberGreaterEqual<TypeUnderTest>::value);
}
