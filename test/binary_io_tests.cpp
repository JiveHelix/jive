/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#include <catch2/catch.hpp>
#include <limits>
#include <sstream>
#include <algorithm>
#include "jive/binary_io.h"
#include "jive/power.h"
#include "jive/testing/gettys_words.h"


TEMPLATE_TEST_CASE(
    "Read and write integers.",
    "[binary_io][integers]",
    int8_t,
    uint8_t,
    int16_t,
    uint16_t,
    int32_t,
    uint32_t,
    int64_t,
    uint64_t)
{
    auto value = GENERATE(
        take(
            100,
            random<TestType>(
                std::numeric_limits<TestType>::min(),
                std::numeric_limits<TestType>::max())));

    STATIC_REQUIRE(std::is_same_v<decltype(value), TestType>);

    std::stringstream stream;
    jive::io::Write(stream, value);

    auto recovered = jive::io::Read<TestType>(stream);

    REQUIRE(recovered == value);
}


TEMPLATE_TEST_CASE(
    "Read and write floats.",
    "[binary_io][floats]",
    float,
    double)
{
    auto value = GENERATE(
        take(
            100,
            random<TestType>(
                std::numeric_limits<TestType>::min(),
                std::numeric_limits<TestType>::max())));

    STATIC_REQUIRE(std::is_same_v<decltype(value), TestType>);

    std::stringstream stream;
    jive::io::Write(stream, value);

    auto recovered = jive::io::Read<TestType>(stream);

    REQUIRE(recovered == value);
}


TEMPLATE_TEST_CASE(
    "Read and write strings",
    "[binary_io]",
    uint8_t,
    uint16_t,
    uint32_t,
    uint64_t)
{
    auto letterCount = GENERATE(
        take(
            100,
            random<size_t>(
                0,
                std::min(
                    static_cast<TestType>(jive::Power<2, 20>()),
                    std::numeric_limits<TestType>::max()))));

    auto value = RandomGettysWords().MakeLetters(letterCount);

    std::stringstream stream;
    jive::io::WriteString<TestType>(stream, value);

    auto recovered = jive::io::ReadString<TestType>(stream);

    REQUIRE(recovered == value);
}
