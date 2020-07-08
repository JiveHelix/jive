/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#include <catch2/catch.hpp>

#include "jive/to_float.h"
#include <string_view>

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
    static constexpr std::string_view positiveInRangeInput =
        STRING(POSITIVE_IN_RANGE_FLOAT);

    static constexpr float positiveInRangeExpected =
        MAKE_LITERAL(POSITIVE_IN_RANGE_FLOAT, f);

    static constexpr std::string_view negativeInRangeInput =
        STRING(NEGATIVE_IN_RANGE_FLOAT);

    static constexpr float negativeInRangeExpected =
        MAKE_LITERAL(NEGATIVE_IN_RANGE_FLOAT, f);

    static constexpr std::string_view positiveOutOfRangeInput = "3.41e38";
    static constexpr std::string_view negativeOutOfRangeInput = "-3.41e38";
    static constexpr std::string_view smallOutOfRangeInput = "1.3e-46";
};


#define POSITIVE_IN_RANGE_DOUBLE +3.1415926e157
#define NEGATIVE_IN_RANGE_DOUBLE -9.883e87
#define SUBNORMAL_DOUBLE 2.718e-320

template<>
struct Values<double>
{
    static constexpr std::string_view positiveInRangeInput =
        STRING(POSITIVE_IN_RANGE_DOUBLE);

    static constexpr double positiveInRangeExpected =
        POSITIVE_IN_RANGE_DOUBLE;

    static constexpr std::string_view negativeInRangeInput =
        STRING(NEGATIVE_IN_RANGE_DOUBLE);

    static constexpr double negativeInRangeExpected =
        NEGATIVE_IN_RANGE_DOUBLE;

    static constexpr std::string_view positiveOutOfRangeInput = "1.798e308";
    static constexpr std::string_view negativeOutOfRangeInput = "-3.41e309";
    static constexpr std::string_view smallOutOfRangeInput = "4.93e-325";
};


#define POSITIVE_IN_RANGE_LONG_DOUBLE +2.7e4001
#define NEGATIVE_IN_RANGE_LONG_DOUBLE -8.88e-3141
#define SUBNORMAL_LONG_DOUBLE 2.718e-4945

template<>
struct Values<long double>
{
    static constexpr std::string_view positiveInRangeInput =
        STRING(POSITIVE_IN_RANGE_LONG_DOUBLE);

    static constexpr long double positiveInRangeExpected =
        MAKE_LITERAL(POSITIVE_IN_RANGE_LONG_DOUBLE, L);

    static constexpr std::string_view negativeInRangeInput =
        STRING(NEGATIVE_IN_RANGE_LONG_DOUBLE);

    static constexpr long double negativeInRangeExpected = 
        MAKE_LITERAL(NEGATIVE_IN_RANGE_LONG_DOUBLE, L);

    static constexpr std::string_view positiveOutOfRangeInput = "1.1898e4932";

    static constexpr std::string_view negativeOutOfRangeInput =
        "-1.189732e4932";

    static constexpr std::string_view smallOutOfRangeInput = "3.645e-4952";
};



TEMPLATE_TEST_CASE(
    "Convert positive in-range string to floating-point", "[to_float]",
    float,
    double,
    long double)
{
    using type = TestType;
    auto converted = jive::ToFloat<type>(Values<type>::positiveInRangeInput);
    REQUIRE(converted == Values<type>::positiveInRangeExpected);
}


TEMPLATE_TEST_CASE(
    "Convert negative in-range string to floating-point", "[to_float]",
    float,
    double,
    long double)
{
    using type = TestType;
    auto converted = jive::ToFloat<type>(Values<type>::negativeInRangeInput);
    REQUIRE(converted == Values<type>::negativeInRangeExpected);
}


TEMPLATE_TEST_CASE(
    "Convert positive out-of-range string to floating-point", "[to_float]",
    float,
    double,
    long double)
{
    using type = TestType;
    REQUIRE_THROWS_AS(
        jive::ToFloat<type>(Values<type>::positiveOutOfRangeInput),
        std::out_of_range);
}


TEMPLATE_TEST_CASE(
    "Convert negative out-of-range string to floating-point", "[to_float]",
    float,
    double,
    long double)
{
    using type = TestType;
    REQUIRE_THROWS_AS(
        jive::ToFloat<type>(Values<type>::negativeOutOfRangeInput),
        std::out_of_range);
}


TEMPLATE_TEST_CASE(
    "Convert small out-of-range string to floating-point", "[to_float]",
    float,
    double,
    long double)
{
    using type = TestType;
    REQUIRE_THROWS_AS(
        jive::ToFloat<type>(Values<type>::smallOutOfRangeInput),
        std::out_of_range);
}
