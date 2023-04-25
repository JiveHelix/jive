
#include <jive/scope_flag.h>
#include <catch2/catch.hpp>


TEST_CASE("ScopeFlag", "[jive]")
{
    bool myFlag{};
    REQUIRE(!myFlag);

    {
        auto flag = jive::ScopeFlag(myFlag);
        REQUIRE(myFlag);
    }

    REQUIRE(!myFlag);
}


TEST_CASE("CountFlag", "[jive]")
{
    using Flag = jive::CountFlag<uint16_t>;

    Flag myFlag{};
    REQUIRE(!myFlag);

    {
        auto flag = jive::ScopedCountFlag(myFlag);
        REQUIRE(myFlag);

        {
            auto secondFlag = jive::ScopedCountFlag(myFlag);
            REQUIRE(myFlag);
        }

        REQUIRE(myFlag);
    }

    REQUIRE(!myFlag);
}
