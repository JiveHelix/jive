
#include <catch2/catch.hpp>
#include <jive/auto_format.h>


TEST_CASE("AutoFormat", "[jive]")
{
    using DoubleFormat = jive::AutoFormat<double, 10>;
    REQUIRE(DoubleFormat::value == "%*.*lg");

    using AlternateFormat = jive::AutoFormat<double, 10, jive::flag::Alternate>;
    REQUIRE(AlternateFormat::value == "%#*.*lg");

    using FloatFormat = jive::AutoFormat<float, 10>;
    REQUIRE(FloatFormat::value == "%*.*g");

    using IntegerFormat = jive::AutoFormat<int16_t, 10>;
    REQUIRE(IntegerFormat::value == "%*.*hd");

    using UnsignedFormat = jive::AutoFormat<uint16_t, 10>;
    REQUIRE(UnsignedFormat::value == "%*.*hu");
}


TEST_CASE("FixedFormat", "[jive]")
{
    using DoubleFormat = jive::FixedFormat<double, 10>;
    REQUIRE(DoubleFormat::value == "%*.*lf");

    using AlternateFormat =
        jive::FixedFormat<double, 10, jive::flag::Alternate>;

    REQUIRE(AlternateFormat::value == "%#*.*lf");

    using FloatFormat = jive::FixedFormat<float, 10>;
    REQUIRE(FloatFormat::value == "%*.*f");

    using IntegerFormat = jive::FixedFormat<int16_t, 10>;
    REQUIRE(IntegerFormat::value == "%*.*hd");

    using UnsignedFormat = jive::FixedFormat<uint16_t, 10>;
    REQUIRE(UnsignedFormat::value == "%*.*hu");
}


TEST_CASE("ScientificFormat", "[jive]")
{
    using DoubleFormat = jive::ScientificFormat<double, 10>;
    REQUIRE(DoubleFormat::value == "%*.*le");

    using AlternateFormat =
        jive::ScientificFormat<double, 10, jive::flag::Alternate>;

    REQUIRE(AlternateFormat::value == "%#*.*le");

    using FloatFormat = jive::ScientificFormat<float, 10>;
    REQUIRE(FloatFormat::value == "%*.*e");

    using IntegerFormat = jive::ScientificFormat<int16_t, 10>;
    REQUIRE(IntegerFormat::value == "%*.*hd");

    using UnsignedFormat = jive::ScientificFormat<uint16_t, 10>;
    REQUIRE(UnsignedFormat::value == "%*.*hu");
}
