
#include <jive/overflow.h>
#include <catch2/catch.hpp>


TEST_CASE("Negative signed not convertible to unsigned", "[overflow]")
{
    int8_t a = -42;
    int16_t b = -4200;
    int32_t c = -42000000;
    int64_t d = -42000000000;

    REQUIRE(!jive::CheckConvertible<uint64_t>(a));
    REQUIRE(!jive::CheckConvertible<uint64_t>(b));
    REQUIRE(!jive::CheckConvertible<uint64_t>(c));
    REQUIRE(!jive::CheckConvertible<uint64_t>(d));
}


TEST_CASE(
    "Large unsigned type convertible to small type when in range",
    "[overflow]")
{
    uint64_t a = 42;

    REQUIRE(jive::CheckConvertible<uint32_t>(a));
    REQUIRE(jive::CheckConvertible<uint16_t>(a));
    REQUIRE(jive::CheckConvertible<uint8_t>(a));

    REQUIRE(jive::CheckConvertible<int32_t>(a));
    REQUIRE(jive::CheckConvertible<int16_t>(a));
    REQUIRE(jive::CheckConvertible<int8_t>(a));
}


TEST_CASE(
    "Unsigned to signed not convertible when it becomes negative.",
    "[overflow]")
{
    uint64_t a = 131;
    REQUIRE(!jive::CheckConvertible<int8_t>(a));
}


TEST_CASE(
    "Large type not convertible to small type when it would overflow.",
    "[overflow]")
{
    int16_t a = 1000;
    REQUIRE(!jive::CheckConvertible<uint8_t>(a));
    REQUIRE(!jive::CheckConvertible<int8_t>(a));
}
