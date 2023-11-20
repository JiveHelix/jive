/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#include <catch2/catch.hpp>

#include "jive/to_float.h"
#include "jive/precise_string.h"
#include <string>

#define STRING_EXPAND(token) #token
#define STRING(token) STRING_EXPAND(token)

#define MAKE_LITERAL_(value, suffix) value ## suffix
#define MAKE_LITERAL(value, suffix) MAKE_LITERAL_(value, suffix)

template<typename T>
struct Values;

#define POSITIVE_IN_RANGE_FLOAT +4.37728e18
#define NEGATIVE_IN_RANGE_FLOAT -1.37728e13
#define SUBNORMAL_FLOAT 1.5e-43

template<>
struct Values<float>
{
    static constexpr auto positiveInRangeInput =
        STRING(POSITIVE_IN_RANGE_FLOAT);

    static constexpr float positiveInRangeExpected =
        MAKE_LITERAL(POSITIVE_IN_RANGE_FLOAT, f);

    static constexpr auto negativeInRangeInput =
        STRING(NEGATIVE_IN_RANGE_FLOAT);

    static constexpr float negativeInRangeExpected =
        MAKE_LITERAL(NEGATIVE_IN_RANGE_FLOAT, f);

    static constexpr auto positiveOutOfRangeInput = "3.41e38";
    static constexpr auto negativeOutOfRangeInput = "-3.41e38";
    static constexpr auto smallOutOfRangeInput = "1.3e-46";
};


#define POSITIVE_IN_RANGE_DOUBLE +3.1415926e157
#define NEGATIVE_IN_RANGE_DOUBLE -9.883e87
#define SUBNORMAL_DOUBLE 2.718e-320

template<>
struct Values<double>
{
    static constexpr auto positiveInRangeInput =
        STRING(POSITIVE_IN_RANGE_DOUBLE);

    static constexpr double positiveInRangeExpected =
        POSITIVE_IN_RANGE_DOUBLE;

    static constexpr auto negativeInRangeInput =
        STRING(NEGATIVE_IN_RANGE_DOUBLE);

    static constexpr double negativeInRangeExpected =
        NEGATIVE_IN_RANGE_DOUBLE;

    static constexpr auto positiveOutOfRangeInput = "1.798e308";
    static constexpr auto negativeOutOfRangeInput = "-3.41e309";
    static constexpr auto smallOutOfRangeInput = "4.93e-325";
};


#if defined(__x86_64__) && !defined(_WIN32)
#define POSITIVE_IN_RANGE_LONG_DOUBLE +2.7e4001
#define NEGATIVE_IN_RANGE_LONG_DOUBLE -8.88e-3141
#define SUBNORMAL_LONG_DOUBLE 2.718e-4945
#else
#define POSITIVE_IN_RANGE_LONG_DOUBLE POSITIVE_IN_RANGE_DOUBLE
#define NEGATIVE_IN_RANGE_LONG_DOUBLE NEGATIVE_IN_RANGE_DOUBLE
#define SUBNORMAL_LONG_DOUBLE SUBNORMAL_DOUBLE
#endif

template<>
struct Values<long double>
{
    static constexpr auto positiveInRangeInput =
        STRING(POSITIVE_IN_RANGE_LONG_DOUBLE);

    static constexpr long double positiveInRangeExpected =
        MAKE_LITERAL(POSITIVE_IN_RANGE_LONG_DOUBLE, L);

    static constexpr auto negativeInRangeInput =
        STRING(NEGATIVE_IN_RANGE_LONG_DOUBLE);

    static constexpr long double negativeInRangeExpected =
        MAKE_LITERAL(NEGATIVE_IN_RANGE_LONG_DOUBLE, L);

    static constexpr auto positiveOutOfRangeInput = "1.1898e4932";

    static constexpr auto negativeOutOfRangeInput =
        "-1.189732e4932";

    static constexpr auto smallOutOfRangeInput = "3.645e-4952";
};



TEMPLATE_TEST_CASE(
    "Convert positive in-range string to floating-point", "[to_float]",
    float,
    double,
    long double)
{
    using Type = TestType;
    auto converted = jive::ToFloat<Type>(Values<Type>::positiveInRangeInput);
    REQUIRE(converted == Values<Type>::positiveInRangeExpected);
}


TEMPLATE_TEST_CASE(
    "Convert negative in-range string to floating-point", "[to_float]",
    float,
    double,
    long double)
{
    using Type = TestType;
    auto converted = jive::ToFloat<Type>(Values<Type>::negativeInRangeInput);
    REQUIRE(converted == Values<Type>::negativeInRangeExpected);
}


TEMPLATE_TEST_CASE(
    "Convert positive out-of-range string to floating-point", "[to_float]",
    float,
    double,
    long double)
{
    using Type = TestType;

    REQUIRE_THROWS_AS(
        jive::ToFloat<Type>(Values<Type>::positiveOutOfRangeInput),
        std::out_of_range);
}


TEMPLATE_TEST_CASE(
    "Convert negative out-of-range string to floating-point", "[to_float]",
    float,
    double,
    long double)
{
    using Type = TestType;

    REQUIRE_THROWS_AS(
        jive::ToFloat<Type>(Values<Type>::negativeOutOfRangeInput),
        std::out_of_range);
}


TEMPLATE_TEST_CASE(
    "Convert small out-of-range string to floating-point", "[to_float]",
    float,
    double,
    long double)
{
    using Type = TestType;

    REQUIRE_THROWS_AS(
        jive::ToFloat<Type>(Values<Type>::smallOutOfRangeInput),
        std::out_of_range);
}


TEST_CASE("Convert string with appended non-digits", "[to_float]")
{
    auto value = GENERATE(take(10, random(-1e30, 1e30)));

    auto asString = jive::PreciseString(value);
    auto appended = asString + "x";
    auto recovered = jive::ToFloat<double>(appended);

    REQUIRE(recovered == Approx(value));
}


TEMPLATE_TEST_CASE(
    "Use MaybeFloat to convert positive in-range string to floating-point",
    "[to_float]",
    float,
    double,
    long double)
{
    using Type = TestType;
    auto converted = jive::MaybeFloat<Type>(Values<Type>::positiveInRangeInput);
    REQUIRE(*converted == Values<Type>::positiveInRangeExpected);
}


TEMPLATE_TEST_CASE(
    "Use MaybeFloat to convert negative in-range string to floating-point",
    "[to_float]",
    float,
    double,
    long double)
{
    using Type = TestType;
    auto converted = jive::MaybeFloat<Type>(Values<Type>::negativeInRangeInput);
    REQUIRE((*converted) == Values<Type>::negativeInRangeExpected);
}


TEMPLATE_TEST_CASE(
    "Use MaybeFloat to convert positive out-of-range string to floating-point",
    "[to_float]",
    float,
    double,
    long double)
{
    using Type = TestType;
    std::optional<Type> result;

    REQUIRE_NOTHROW(
        result = jive::MaybeFloat<Type>(Values<Type>::positiveOutOfRangeInput));

    REQUIRE(!result);
}


TEMPLATE_TEST_CASE(
    "Use MaybeFloat to convert negative out-of-range string to floating-point",
    "[to_float]",
    float,
    double,
    long double)
{
    using Type = TestType;
    std::optional<Type> result;

    REQUIRE_NOTHROW(
        result = jive::MaybeFloat<Type>(Values<Type>::negativeOutOfRangeInput));

    REQUIRE(!result);
}


TEMPLATE_TEST_CASE(
    "Use MaybeFloat to convert small out-of-range string to floating-point",
    "[to_float]",
    float,
    double,
    long double)
{
    using Type = TestType;
    std::optional<Type> result;

    REQUIRE_NOTHROW(
        result = jive::MaybeFloat<Type>(Values<Type>::smallOutOfRangeInput));

    REQUIRE(!result);
}


TEST_CASE(
    "Use MaybeFloat to convert string with appended non-digits",
    "[to_float]")
{
    auto value = GENERATE(take(10, random(-1e30, 1e30)));

    auto asString = jive::PreciseString(value);
    auto appended = asString + "x";
    auto recovered = jive::MaybeFloat<double>(appended);

    // MaybeFloat requires the entire string to be used in the conversion.
    REQUIRE(!recovered);
}
