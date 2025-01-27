#include <arba/rand/rand.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdlib>

// Tests rand_int<URNG, IT>(urng, [a, b])

TEST(rand_tests, test_rand_int_rng_min_max)
{
    std::mt19937_64 rng(42);

    std::array<unsigned, 256> counters{ 0 };
    ASSERT_TRUE(std::ranges::all_of(counters, [](auto counter) { return counter == 0; }));

    constexpr unsigned factor = 1000;
    for (unsigned times = (counters.size() - 1) * factor; times; --times)
    {
        uint16_t index = rand::rand_int<uint16_t>(rng, 1, counters.size() - 1);
        ASSERT_GE(index, 1);
        ASSERT_LE(index, counters.size() - 1);
        ++counters.at(index);
    }

    EXPECT_EQ(counters.front(), 0);
    std::ranges::for_each(std::ranges::subrange(counters.begin() + 1, counters.end()),
                          [=](const auto& counter) { EXPECT_GE(counter, 0.9 * factor); });
}

// Tests rand_int<IT>([a, b])

TEST(rand_tests, test_rand_int)
{
    rand::reseed(42);
    uint64_t value = rand::rand_int<uint64_t>();
    EXPECT_EQ(value, 13'930'160'852'258'120'406ULL);
}

TEST(rand_tests, test_reseed)
{
    rand::reseed(42);
    uint64_t value = rand::rand_int<uint64_t>(0, 1'000'000);
    rand::reseed(42);
    uint64_t value_2 = rand::rand_int<uint64_t>(0, 1'000'000);
    ASSERT_EQ(value, value_2);
    ASSERT_EQ(value, 755'156);
    ASSERT_EQ(value_2, 755'156);
}

TEST(rand_tests, test_rand_int_min_max)
{
    rand::reseed(42);

    std::array<unsigned, 256> counters{ 0 };
    ASSERT_TRUE(std::ranges::all_of(counters, [](auto counter) { return counter == 0; }));

    constexpr unsigned factor = 1000;
    for (unsigned times = (counters.size() - 1) * factor; times; --times)
    {
        uint16_t index = rand::rand_int<uint16_t>(1, counters.size() - 1);
        ASSERT_GE(index, 1);
        ASSERT_LE(index, counters.size() - 1);
        ++counters.at(index);
    }

    EXPECT_EQ(counters.front(), 0);
    std::ranges::for_each(std::ranges::subrange(counters.begin() + 1, counters.end()),
                          [=](const auto& counter) { EXPECT_GE(counter, 0.9 * factor); });
}
