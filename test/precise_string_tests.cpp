/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#include <catch2/catch.hpp>
#include "jive/precise_string.h"
#include "jive/testing/generator_limits.h"
#include "jive/to_float.h"

TEMPLATE_TEST_CASE(
    "PreciseString round trip",
    "[precise_string]",
    float,
    double,
    long double)
{
    auto value = GENERATE(
        take(
            30,
            random(
                GeneratorLimits<TestType>::Lowest(),
                GeneratorLimits<TestType>::Max())));

    auto asString = jive::PreciseString(value);
    auto recovered = jive::ToFloat<TestType>(asString);
    REQUIRE(recovered == Approx(value));
}
