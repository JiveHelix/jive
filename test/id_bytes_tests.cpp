/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#include <catch2/catch.hpp>

#include "jive/id_bytes.h"


TEMPLATE_TEST_CASE(
    "Test IdBytes",
    "[id_bytes]",
    jive::IdBytes<2>,
    jive::IdBytes<4>,
    jive::IdBytes<8>,
    jive::IdBytes<16>)
{
    // Run each test 5 times
    GENERATE(1, 2, 3, 4, 5);

    TestType value{};
    TestType otherValue{};

    static constexpr uint8_t maxByteValue =
        std::numeric_limits<uint8_t>::max();

    auto generator =
        Catch::Generators::RandomIntegerGenerator<uint8_t>(0, maxByteValue);

    for (size_t index = 0; index < sizeof(TestType); ++index)
    {
        value.bytes[index] = otherValue.bytes[index] = generator.get();
        generator.next();
    }

    SECTION("Compares Equal")
    {
        REQUIRE(value == otherValue);
    }

    SECTION("Compares not equal")
    {
        while (otherValue.bytes[1] == value.bytes[1])
        {
            otherValue.bytes[1] = generator.get();
            generator.next();
        }

        REQUIRE(otherValue != value);
    }
}
