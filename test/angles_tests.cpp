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


// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("Convert between radians and degrees", "[angles]")
{
    using Angles = jive::Angles<double>;

    const size_t angleCount = 12;

    double radians = -2 * Angles::pi;
    double radiansStep = Angles::pi / static_cast<double>(angleCount);

    double degrees = -Angles::fullRotationDegrees;

    double degreesStep = Angles::halfRotationDegrees
        / static_cast<double>(angleCount);

    // Catch's default Approx does nothing when the argument is zero.
    // Give it an explicit margin when we are testing for zero.
    static constexpr double marginNearZero = 1e-13;
    static auto approxZero = Approx(0.0).margin(marginNearZero);

    for (size_t count = 0; count < 2 * angleCount + 1; ++count)
    {
        if (std::abs(degrees) < marginNearZero)
        {
            REQUIRE(jive::ToDegrees(radians) == approxZero);
            REQUIRE(jive::ToRadians(degrees) == approxZero);
        }
        else
        {
            REQUIRE(jive::ToDegrees(radians) == Approx(degrees));
            REQUIRE(jive::ToRadians(degrees) == Approx(radians));
        }

        radians += radiansStep;
        degrees += degreesStep;
    }
}

TEST_CASE("Convert multiple values to degrees", "[Angles]")
{
    // Multple angles (with different types) can be converted in the same
    // variadic call to jive::ToDegrees(...)
    using Angles = jive::Angles<double>;
    static constexpr auto chunkSize = 5;
    static constexpr auto testCount = 100;
    static constexpr double lowerBound = -4 * Angles::pi;
    static constexpr double upperBound = 4 * Angles::pi;

    auto values = GENERATE(
        take(testCount, chunk(chunkSize, random(lowerBound, upperBound))));

    REQUIRE(values.size() == chunkSize);

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

    std::tie(aDegrees, bDegrees, cDegrees, dDegrees, eDegrees) = jive::
        ToDegrees(aRadians, bRadians, cRadians, dRadians, eRadians);

    auto aExpected = Approx(
        Angles::halfRotationDegrees * values.at(0) / Angles::pi);

    auto bExpected = Approx(
        Angles::halfRotationDegrees * values.at(1) / Angles::pi);

    auto cExpected = Approx(static_cast<float>(
        Angles::halfRotationDegrees * values.at(2) / Angles::pi));

    auto dExpected = Approx(static_cast<float>(
        Angles::halfRotationDegrees * values.at(3) / Angles::pi));

    auto eExpected = Approx(
        Angles::halfRotationDegrees * values.at(4) / Angles::pi);

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
    using Angles = jive::Angles<double>;

    static constexpr auto chunkSize = 5;
    static constexpr auto testCount = 100;
    static constexpr double lowerBound = -2 * Angles::fullRotationDegrees;
    static constexpr double upperBound = 2 * Angles::fullRotationDegrees;

    auto values = GENERATE(
        take(testCount, chunk(chunkSize, random(lowerBound, upperBound))));

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

    std::tie(aRadians, bRadians, cRadians, dRadians, eRadians) = jive::
        ToRadians(aDegrees, bDegrees, cDegrees, dDegrees, eDegrees);

    auto aExpected = Approx(
        Angles::pi * values.at(0) / Angles::halfRotationDegrees);

    auto bExpected = Approx(static_cast<float>(
        Angles::pi * values.at(1) / Angles::halfRotationDegrees));

    auto cExpected = Approx(
        Angles::pi * values.at(2) / Angles::halfRotationDegrees);

    auto dExpected = Approx(
        Angles::pi * values.at(3) / Angles::halfRotationDegrees);

    auto eExpected = Approx(static_cast<float>(
        Angles::pi * values.at(4) / Angles::halfRotationDegrees));

    REQUIRE(aRadians == aExpected);
    REQUIRE(bRadians == bExpected);
    REQUIRE(cRadians == cExpected);
    REQUIRE(dRadians == dExpected);
    REQUIRE(eRadians == eExpected);
}
