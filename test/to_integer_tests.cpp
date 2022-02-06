/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#include <catch2/catch.hpp>

#include "jive/to_integer.h"
#include <string_view>

// Create types to use with the product test case.
#define MAKE_TEST_VALUES(asDec_, asOct_, asHex_)                    \
    template<typename T>                                            \
    struct p ## asDec_                                              \
    {                                                               \
        using type = T;                                             \
        static constexpr std::string_view asDec = #asDec_;          \
        static constexpr std::string_view asOct = #asOct_;          \
        static constexpr std::string_view asHex = #asHex_;          \
        static constexpr auto value = (asDec_);                     \
    };                                                              \
    template<typename T>                                            \
    struct n ## asDec_                                              \
    {                                                               \
        using type = T;                                             \
        static constexpr std::string_view asDec = "-" #asDec_;      \
        static constexpr std::string_view asOct = "-" #asOct_;      \
        static constexpr std::string_view asHex = "-" #asHex_;      \
        static constexpr auto value = -(asDec_);                    \
    }


MAKE_TEST_VALUES(0, 0, 0);
MAKE_TEST_VALUES(42, 52, 2a);
MAKE_TEST_VALUES(56, 70, 38);
MAKE_TEST_VALUES(105, 151, 69);
MAKE_TEST_VALUES(119, 167, 77);

static constexpr int base8 = 8;
static constexpr int base10 = 10;
static constexpr int base16 = 16;

#ifndef _WIN32
// g++ warns about sign-conversion in this expanded macro
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
TEMPLATE_PRODUCT_TEST_CASE(
    "Convert strings to signed integers",
    "[to_integer]",
    (p0, p42, p56, p105, p119, n0, n42, n56, n105, n119),
    (int8_t, int16_t, int32_t, int64_t))
{
#ifndef _WIN32
#pragma GCC diagnostic pop
#endif
    using type = typename TestType::type;

    auto valueFromDec = jive::ToInteger<type, base10>(TestType::asDec);
    auto valueFromOct = jive::ToInteger<type, base8>(TestType::asOct);
    auto valueFromHex = jive::ToInteger<type, base16>(TestType::asHex);

    REQUIRE(valueFromDec == TestType::value);
    REQUIRE(valueFromOct == TestType::value);
    REQUIRE(valueFromHex == TestType::value);
}

#ifndef _WIN32
// g++ warns about sign-conversion in this expanded macro
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
TEMPLATE_PRODUCT_TEST_CASE(
    "Convert positive strings to unsigned integers",
    "[to_integer]",
    ( p0, p42, p56, p105, p119),
    (uint8_t, uint16_t, uint32_t, uint64_t))
{
#ifndef _WIN32
#pragma GCC diagnostic pop
#endif
    using type = typename TestType::type;

    auto valueFromDec = jive::ToInteger<type, base10>(TestType::asDec);
    auto valueFromOct = jive::ToInteger<type, base8>(TestType::asOct);
    auto valueFromHex = jive::ToInteger<type, base16>(TestType::asHex);

    REQUIRE(valueFromDec == TestType::value);
    REQUIRE(valueFromOct == TestType::value);
    REQUIRE(valueFromHex == TestType::value);
}
