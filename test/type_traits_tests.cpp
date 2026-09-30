/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#include <string>
#include <string_view>

#include <catch2/catch.hpp>
#include <jive/type_traits.h>


TEST_CASE("std::string_view is not a container.", "[type_traits]")
{
    STATIC_REQUIRE(jive::IsString<std::string>);
    STATIC_REQUIRE(!jive::IsValueContainer<std::string>);

    STATIC_REQUIRE(jive::IsStringView<std::string_view>);
    STATIC_REQUIRE(!jive::IsValueContainer<std::string_view>);
}
