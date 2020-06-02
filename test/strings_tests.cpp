/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#include <catch2/catch.hpp>

#include "jive/strings.h"


TEST_CASE("strings::Trim shortens string", "[strings]")
{
    const char bytes[8] = {'f', 'o', 'o', 0, 0, 0, 0, 0};

    // Intentially create a string that has null bytes
    std::string value(bytes, 8);

    REQUIRE(value.size() == 8);

    jive::strings::Trim(value);

    REQUIRE(value.size() == 3);
}


TEST_CASE("strings::Make creates a string without nullbytes", "[strings]")
{
    const char bytes[8] = {'w', 'i', 'b', 'b', 'l', 'e', 0, 0};

    std::string value = jive::strings::Make(bytes);
    REQUIRE(value.size() == 6);
}


TEST_CASE("strings::Split separates string on token", "[strings]")
{
    std::string value{"foo.bar,car.wibble,wobble.frob"};

    SECTION("Split on '.'")
    {
        auto split = jive::strings::Split(value, '.');
        CHECK(split.size() == 4);
        REQUIRE(split
            == std::vector<std::string>{
                "foo",
                "bar,car",
                "wibble,wobble",
                "frob"});
    }

    SECTION("Split on ','")
    {
        auto split = jive::strings::Split(value, ',');
        CHECK(split.size() == 3);
        REQUIRE(split == std::vector<std::string>{
            "foo.bar",
            "car.wibble",
            "wobble.frob"});
    }

    SECTION("Split with 'word' token in the middle of string")
    {
        auto split = jive::strings::Split(value, "wibble");
        CHECK(split.size() == 2);
        REQUIRE(split == std::vector<std::string>{
            "foo.bar,car.",
            ",wobble.frob"});
    }

    SECTION("Split with 'word' token at beginning of string")
    {
        auto split = jive::strings::Split(value, "foo");
        CHECK(split.size() == 2);
        REQUIRE(split
            == std::vector<std::string>{"", ".bar,car.wibble,wobble.frob"});
    }

    SECTION("Split with 'word' token at end of string")
    {
        auto split = jive::strings::Split(value, "frob");
        CHECK(split.size() == 2);
        REQUIRE(split
            == std::vector<std::string>{"foo.bar,car.wibble,wobble.", ""});
    }

    SECTION("Split with char token at beginning of string")
    {
        auto split = jive::strings::Split(value, "f");
        CHECK(split.size() == 3);
        REQUIRE(split == std::vector<std::string>{
            "",
            "oo.bar,car.wibble,wobble.",
            "rob"});
    }

    SECTION("Split with char token at end of string")
    {
        auto split = jive::strings::Split(value, "b");
        CHECK(split.size() == 7);
        REQUIRE(split == std::vector<std::string>{
            "foo.",
            "ar,car.wi",
            "",
            "le,wo",
            "",
            "le.fro",
            ""});
    }
}


TEST_CASE(
    "strings::SplitOnWhiteSpace separates string on whitespace",
    "[strings]")
{
    std::string value{"foo.bar  car\n\t .wibble,wobble\f.\rfrob\n"};

    auto split = jive::strings::SplitOnWhiteSpace(value);

    CHECK(split.size() == 6);

    REQUIRE(split == std::vector<std::string>{
        "foo.bar",
        "car",
        ".wibble,wobble",
        ".",
        "frob",
        ""});
}


TEST_CASE("strings::Join creates a new string around token", "[strings]")
{
    auto joined =
        jive::strings::Join( {"let's", "make", "this", "hyphenated"}, '-');

    REQUIRE(joined == "let's-make-this-hyphenated");
}
