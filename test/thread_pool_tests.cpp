/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#include <catch2/catch.hpp>
#include <jive/thread_pool.h>

#include <chrono>


TEST_CASE("Run concurrent threads.", "[threads]")
{
    using namespace std::chrono_literals;
    auto threadPool = jive::GetThreadPool();

    int p{};
    int q{};
    int r{};
    int s{};

    auto sentry1 = threadPool->AddJob(
        [&]()
        {
            std::this_thread::sleep_for(1s);
            p = 1;
        });

    auto sentry2 = threadPool->AddJob(
        [&]()
        {
            std::this_thread::sleep_for(1s);
            q = 2;
        });

    auto sentry3 = threadPool->AddJob(
        [&]()
        {
            std::this_thread::sleep_for(1s);
            r = 3;
        });

    auto sentry4 = threadPool->AddJob(
        [&]()
        {
            std::this_thread::sleep_for(1s);
            s = 4;
        });

    auto sentry5 = threadPool->AddJob(
        [&]()
        {
            std::this_thread::sleep_for(1s);

            throw std::runtime_error("foo");
        });

    REQUIRE(p == 0);
    REQUIRE(q == 0);
    REQUIRE(r == 0);
    REQUIRE(s == 0);

    sentry1.Wait();
    sentry2.Wait();
    sentry3.Wait();
    sentry4.Wait();

    REQUIRE(p == 1);
    REQUIRE(q == 2);
    REQUIRE(r == 3);
    REQUIRE(s == 4);

    REQUIRE_THROWS_AS(sentry5.Wait(), std::runtime_error);
}
