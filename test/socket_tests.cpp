/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#include <catch2/catch.hpp>

#include "jive/socket/socket.h"
#include "jive/socket/address.h"

TEST_CASE("Ill-formed address is detected", "[socket]")
{
    REQUIRE_THROWS_AS(
        jive::Address("127.0.1"),
        jive::SocketError);

    try
    {
        jive::Address("127.0.1");
    }
    catch (jive::SocketError &error)
    {
        REQUIRE(error.code() == std::errc::bad_address);
    }
}
