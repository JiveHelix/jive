/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#include <catch2/catch.hpp>

#include "jive/angles.h"
#include <cmath>
#include <array>
#include <tuple>
#include <iostream>

// Use the Catçh `_a` literal use approximate equality for floats.
using namespace Catch::literals;

// (radians, degrees)
inline constexpr std::array<std::tuple<double, double>, 17> radiansAndDegrees
{
    std::make_tuple(-2 * M_PI, -360.0),
    std::make_tuple(-7 * M_PI / 4, -315.0),
    std::make_tuple(-3 * M_PI / 2, -270.0),
    std::make_tuple(-5 * M_PI / 4, -225.0),
    std::make_tuple(-M_PI, -180.0),
    std::make_tuple(-3 * M_PI / 4, -135.0),
    std::make_tuple(-M_PI / 2, -90.0),
    std::make_tuple(-M_PI / 4, -45.0),
    std::make_tuple(0, 0.0),
    std::make_tuple(M_PI / 4, 45.0),
    std::make_tuple(M_PI / 2, 90.0),
    std::make_tuple(3 * M_PI / 4, 135.0),
    std::make_tuple(M_PI, 180.0),
    std::make_tuple(5 * M_PI / 4, 225.0),
    std::make_tuple(3 * M_PI / 2, 270.0),
    std::make_tuple(7 * M_PI / 4, 315.0),
    std::make_tuple(2 * M_PI, 360.0)
};

TEST_CASE("Convert single value to degrees", "[angles]")
{
    for (auto [radians, degrees]: radiansAndDegrees)
    {
        REQUIRE(jive::ToDegrees(radians) == Approx(degrees));
    }
}

TEST_CASE("Convert single value to radians", "[angles]")
{
    for (auto [radians, degrees]: radiansAndDegrees)
    {
        REQUIRE(jive::ToRadians(degrees) == Approx(radians));
    }
}

TEST_CASE("Convert multiple values to degrees", "[angles]")
{
    // Multple angles (with different types) can be converted in the same
    // variadic call to jive::ToDegrees(...)

    auto values = GENERATE(take(100, chunk(5, random(-4 * M_PI, 4 * M_PI))));

    REQUIRE(values.size() == 5);

    double aRadians = values.at(0);
    double bRadians = values.at(1);
    float cRadians = static_cast<float>(values.at(2));
    float dRadians = static_cast<float>(values.at(3));
    double eRadians = values.at(4);

    double aDegrees;
    double bDegrees;
    float cDegrees;
    float dDegrees;
    double eDegrees;

    std::tie(aDegrees, bDegrees, cDegrees, dDegrees, eDegrees) =
        jive::ToDegrees(aRadians, bRadians, cRadians, dRadians, eRadians);

    auto aExpected = Approx(180.0 * values.at(0) / M_PI);
    auto bExpected = Approx(180.0 * values.at(1) / M_PI);
    auto cExpected = Approx(static_cast<float>(180.0 * values.at(2) / M_PI));
    auto dExpected = Approx(static_cast<float>(180.0 * values.at(3) / M_PI));
    auto eExpected = Approx(180.0 * values.at(4) / M_PI);

    REQUIRE(aDegrees == aExpected);
    REQUIRE(bDegrees == bExpected);
    REQUIRE(cDegrees == cExpected);
    REQUIRE(dDegrees == dExpected);
    REQUIRE(eDegrees == eExpected);
}

TEST_CASE("Convert multiple values to radians", "[angles]")
{
    // Multple angles (with different types) can be converted in the same
    // variadic call to jive::ToRadians(...)
    auto values = GENERATE(take(100, chunk(5, random(-720.0, 720.0))));

    REQUIRE(values.size() == 5);

    double aDegrees = values.at(0);
    float bDegrees = static_cast<float>(values.at(1));
    double cDegrees = values.at(2);
    double dDegrees = values.at(3);
    float eDegrees = static_cast<float>(values.at(4));

    double aRadians;
    float bRadians;
    double cRadians;
    double dRadians;
    float eRadians;

    std::tie(aRadians, bRadians, cRadians, dRadians, eRadians) =
        jive::ToRadians(aDegrees, bDegrees, cDegrees, dDegrees, eDegrees);

    auto aExpected = Approx(M_PI * values.at(0) / 180.0);
    auto bExpected = Approx(static_cast<float>(M_PI * values.at(1) / 180.0));
    auto cExpected = Approx(M_PI * values.at(2) / 180.0);
    auto dExpected = Approx(M_PI * values.at(3) / 180.0);
    auto eExpected = Approx(static_cast<float>(M_PI * values.at(4) / 180.0));

    REQUIRE(aRadians == aExpected);
    REQUIRE(bRadians == bExpected);
    REQUIRE(cRadians == cExpected);
    REQUIRE(dRadians == dExpected);
    REQUIRE(eRadians == eExpected);
}
