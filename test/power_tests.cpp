/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#include <catch2/catch.hpp>
#include "jive/power.h"

TEST_CASE("Power is computed at compile time", "[power]")
{
    STATIC_REQUIRE(jive::Power<2, 0>() == 1);
    STATIC_REQUIRE(jive::Power<3, 0>() == 1);
    STATIC_REQUIRE(jive::Power<2, 1>() == 2);
    STATIC_REQUIRE(jive::Power<2, 2>() == 4);
    STATIC_REQUIRE(jive::Power<2, 3>() == 8);
    STATIC_REQUIRE(jive::Power<2, 5>() == 32);
    STATIC_REQUIRE(jive::Power<2, 10>() == 1024);
    STATIC_REQUIRE(jive::Power<3, 13>() == 1594323);
}
