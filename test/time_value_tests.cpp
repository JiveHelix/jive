/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#include <catch2/catch.hpp>

#include <iostream>

#include "jive/time_value.h"
#include "jive/power.h"
#include "utility/duration_limits.h"


template<typename T, typename Enable = void>
struct CheckType;

template<typename T>
struct CheckType<T, std::enable_if_t<std::is_signed_v<T>>>
{
    using type = int64_t;
};

template<typename T>
struct CheckType<T, std::enable_if_t<std::is_unsigned_v<T>>>
{
    using type = uint64_t;
};


TEMPLATE_PRODUCT_TEST_CASE(
    "Create TimeValue from signed durations.",
    "[time_value]",
    (jive::Seconds, jive::Milliseconds, jive::Microseconds, jive::Nanoseconds),
    (int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t))
{
    using rep = typename TestType::rep;
    using bounds = DurationLimits<TestType>;

    auto value = GENERATE(take( 100, random(bounds::lower, bounds::upper)));

    STATIC_REQUIRE(std::is_same_v<decltype(value), rep>);

    // We do not know the TestType, but we can use it to convert to a known
    // type, using the chrono::duration class.
    TestType valueAsDuration{value};
    std::chrono::duration<int64_t, std::nano>
        valueAsNanoseconds{valueAsDuration};

    auto timeValue = jive::TimeValue(valueAsDuration);

    REQUIRE(timeValue.GetNanoseconds() == valueAsNanoseconds.count());
}


TEMPLATE_PRODUCT_TEST_CASE(
    "Math with TimeValues",
    "[time_value]",
    (jive::Seconds, jive::Milliseconds, jive::Microseconds, jive::Nanoseconds),
    (int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t))
{
    using rep = typename TestType::rep;
    using bounds = DurationLimits<TestType>;

    using CheckDuration = std::chrono::duration<int64_t, std::nano>;

    auto first = GENERATE(take(10, random(bounds::lower, bounds::upper)));
    auto second = GENERATE(take(10, random(bounds::lower, bounds::upper)));

    STATIC_REQUIRE(std::is_same_v<decltype(first), rep>);
    STATIC_REQUIRE(std::is_same_v<decltype(second), rep>);

    TestType firstAsDuration{first};
    TestType secondAsDuration{second};

    jive::TimeValue left{firstAsDuration};
    jive::TimeValue right{secondAsDuration};

    CheckDuration checkLeft{firstAsDuration};
    CheckDuration checkRight{secondAsDuration};

    SECTION("Addition")
    {
        auto result = left + right;
        auto expectedDuration = checkLeft + checkRight;
        REQUIRE(result.GetDuration() == expectedDuration);
    }

    SECTION("Subtraction")
    {
        auto result = left - right;
        auto expectedDuration = checkLeft - checkRight;
        REQUIRE(result.GetDuration() == expectedDuration);
    }
}
