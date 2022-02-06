/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#include <catch2/catch.hpp>

#include "jive/multiply_rounded.h"
#include "jive/testing/cast_limits.h"
#include "jive/testing/random_type.h"

using namespace Catch::literals;

TEMPLATE_TEST_CASE(
    "Product of integer and double is rounded.",
    "[multiply_rounded]",
    int8_t,
    uint8_t,
    int16_t,
    uint16_t,
    int32_t,
    uint32_t,
    int64_t,
    uint64_t)
{
    using Limits = 
        CastLimits<TestType, typename RandomType<TestType>::type>;

    auto value = static_cast<TestType>(
        GENERATE(
            take(
                10,
                random(Limits::Min(), Limits::Max()))));

    auto scale = GENERATE(
        take(
            10,
            random(
                CastLimits<TestType, double>::Min(),
                CastLimits<TestType, double>::Max())));

    STATIC_REQUIRE(std::is_same_v<decltype(value), TestType>);
    STATIC_REQUIRE(std::is_same_v<decltype(scale), double>);

    // Allow the product to overflow.
    // The expectedValue should overflow the same way.
    // All we are testing for here is that the product is rounded.
    auto product = jive::MultiplyRounded(jive::NoCheckOverflow{}, scale, value);

    auto roundedProductAsDouble = round(scale * static_cast<double>(value));
    auto expectedValue = static_cast<TestType>(roundedProductAsDouble);

    REQUIRE(product == expectedValue);
}


TEMPLATE_TEST_CASE(
    "Product of floating-point and double is not rounded.",
    "[multiply_rounded]",
    float,
    double)
{
    auto value = GENERATE(
        take(
            10,
            random(
                CastLimits<TestType>::Min(),
                CastLimits<TestType>::Max())));

    auto scale = GENERATE(
        take(
            10,
            random(
                CastLimits<TestType, double>::Min(),
                CastLimits<TestType, double>::Max())));

    STATIC_REQUIRE(std::is_same_v<decltype(value), TestType>);
    STATIC_REQUIRE(std::is_same_v<decltype(scale), double>);

    // Allow the product to overflow.
    // The expectedValue should overflow the same way.
    // All we are testing for here is that the product is rounded.
    auto product = jive::MultiplyRounded(jive::NoCheckOverflow{}, scale, value);

    auto expectedValue =
        Approx(static_cast<TestType>(scale * static_cast<double>(value)));

    REQUIRE(product == expectedValue);
}


TEMPLATE_TEST_CASE(
    "Overflow is detected for product of integer and double.",
    "[multiply_rounded]",
    int8_t,
    uint8_t,
    int16_t,
    uint16_t,
    int32_t,
    uint32_t,
    int64_t,
    uint64_t)
{
    using Limits = CastLimits<TestType, typename RandomType<TestType>::type>;

    auto value = static_cast<TestType>(
        GENERATE(
            take(
                10,
                filter(
                    [](auto v) { return v != static_cast<decltype(v)>(0); },
                    random(Limits::Min(), Limits::Max())))));

    // Choose a scale that is expected to cause overflow
    static constexpr auto scaleFactor = 1.5;
    auto valueAsDouble = static_cast<double>(value);
    auto maximumValue = CastLimits<TestType, double>::Max();
    auto scaleToCauseOverflow = (maximumValue * scaleFactor) / valueAsDouble;

    REQUIRE_THROWS_AS(
        jive::MultiplyRounded(scaleToCauseOverflow, value),
        std::overflow_error);
}


// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE(
    "Computes the rounded product of multiple signed arguments wihout overflow",
    "[multiply_rounded]")
{
    std::vector<int32_t> values =
        GENERATE(take(10, chunk(5, random(-5000, 5000))));

    REQUIRE(values.size() == 5);

    static constexpr auto rangeLow = -3.0;
    static constexpr auto rangeHigh = 3.0;
    auto scale = GENERATE(take(10, random(rangeLow, rangeHigh)));

    auto aValue = static_cast<int16_t>(values[0]);
    auto bValue = values[1];
    auto cValue = static_cast<int64_t>(values[2]);
    auto dValue = static_cast<float>(values[3]);
    auto eValue = static_cast<double>(values[4]);

    auto [aResult, bResult, cResult, dResult, eResult] =
        jive::MultiplyRounded(scale, aValue, bValue, cValue, dValue, eValue);

    auto aExpected = static_cast<int16_t>(
        round(scale * static_cast<double>(aValue)));

    auto bExpected = static_cast<int32_t>(
        round(scale * static_cast<double>(bValue)));

    auto cExpected = static_cast<int64_t>(
        round(scale * static_cast<double>(cValue)));

    // Expect floating-point to be unrounded.
    auto dExpected = static_cast<float>(
        scale * static_cast<double>(dValue));

    auto eExpected = scale * eValue;

    STATIC_REQUIRE(std::is_same_v<decltype(aValue), decltype(aExpected)>);
    STATIC_REQUIRE(std::is_same_v<decltype(bValue), decltype(bExpected)>);
    STATIC_REQUIRE(std::is_same_v<decltype(cValue), decltype(cExpected)>);
    STATIC_REQUIRE(std::is_same_v<decltype(dValue), decltype(dExpected)>);
    STATIC_REQUIRE(std::is_same_v<decltype(eValue), decltype(eExpected)>);

    REQUIRE(aResult == aExpected);
    REQUIRE(bResult == bExpected);
    REQUIRE(cResult == cExpected);
    REQUIRE(dResult == Approx(dExpected));
    REQUIRE(eResult == Approx(eExpected));
}
