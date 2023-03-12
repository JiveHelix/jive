/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#include <catch2/catch.hpp>

#include "jive/path.h"


TEST_CASE("Join paths", "[path]")
{
    REQUIRE(jive::path::Join("foo", "bar") == "foo/bar");
    REQUIRE(jive::path::Join({"foo", "bar", "frob"}) == "foo/bar/frob");
}

TEST_CASE("Split path", "[path]")
{
    auto [directory, base] = jive::path::Split("foo/bar/frob.txt");
    REQUIRE(directory == "foo/bar");
    REQUIRE(base == "frob.txt");
}

TEST_CASE("Split path with only directory", "[path]")
{
    auto [directory, base] = jive::path::Split("foo/bar/");
    REQUIRE(directory == "foo/bar");
    REQUIRE(base == "");
}

TEST_CASE("Split empty path", "[path]")
{
    auto [directory, base] = jive::path::Split("");
    REQUIRE(directory == "");
    REQUIRE(base == "");
}

TEST_CASE("Split path with only file", "[path]")
{
    auto [directory, base] = jive::path::Split("frob.txt");
    REQUIRE(directory == "");
    REQUIRE(base == "frob.txt");
}

TEST_CASE("Get directory of path", "[path]")
{
    auto directory = jive::path::Directory("/wibble/wobble/frob");
    REQUIRE(directory == "/wibble/wobble");
}

TEST_CASE("Get base name of path", "[path]")
{
    auto base = jive::path::Base("/wibble/wobble/frob");
    REQUIRE(base == "frob");
}

TEST_CASE("Split path on extension", "[path]")
{
    std::string extension;

    std::tie(std::ignore, extension) =
        jive::path::SplitExtension("/wibble/wobble/frob.py");

    REQUIRE(extension == ".py");
}
