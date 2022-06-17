#include <catch2/catch.hpp>
#include "jive/revision.h"


TEST_CASE("BUILD_REVISION is set", "[revision]")
{
    REQUIRE(!std::string(buildRevision).empty());
}


TEST_CASE("BUILD_TAG is set", "[revision]")
{
    REQUIRE(!std::string(buildTag).empty());
}


TEST_CASE("BUILD_TIME is set", "[revision]")
{
    REQUIRE(!std::string(buildTime).empty());
}
