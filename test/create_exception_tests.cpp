/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#include <catch2/catch.hpp>

#include "jive/create_exception.h"

CREATE_EXCEPTION(TestException, std::runtime_error);
CREATE_EXCEPTION(ChildException, TestException);

TEST_CASE("Created exception throws", "[exceptions]")
{
    REQUIRE_THROWS_AS(throw TestException(""), TestException);
    REQUIRE_THROWS_AS(throw TestException(""), std::runtime_error);
}

TEST_CASE("Created exception throws with message", "[exceptions]")
{
    REQUIRE_THROWS_WITH(
        throw TestException("Something went wrong"),
        "Something went wrong");
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("Subclassed exception throws", "[exceptions]")
{
    REQUIRE_THROWS_AS(throw ChildException(""), ChildException);
    REQUIRE_THROWS_AS(throw ChildException(""), TestException);
    REQUIRE_THROWS_AS(throw ChildException(""), std::runtime_error);
}

TEST_CASE("Subclassed exception throws with message", "[exceptions]")
{
    REQUIRE_THROWS_WITH(
        throw ChildException("Something else went wrong"),
        "Something else went wrong");
}
