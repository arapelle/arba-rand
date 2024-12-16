#include <algorithm>
#include <arba/rand/urng.hpp>
#include <cstdlib>
#include <gtest/gtest.h>

// Tests urng_*

TEST(urng_tests, test_urng_u8_min_max)
{
    rand::urng_u8<1, 100> rng(42);
    ASSERT_EQ(rng.min(), 1);
    ASSERT_EQ(rng.max(), 100);

    std::array<unsigned, decltype(rng)::max() + 1> counters{ 0 };
    ASSERT_TRUE(std::ranges::all_of(counters, [](auto counter) { return counter == 0; }));

    constexpr unsigned factor = 1000;
    for (unsigned times = (counters.size() - 1) * factor; times; --times)
    {
        uint8_t index = rng();
        ++counters.at(index);
    }

    EXPECT_EQ(counters.front(), 0);
    std::ranges::for_each(std::ranges::subrange(counters.begin() + 1, counters.end()),
                          [=](const auto& counter) { EXPECT_GE(counter, 0.9 * factor); });
}

TEST(urng_tests, test_urng_u8)
{
    rand::urng_u8 rng(42, 1, 100);
    ASSERT_EQ(rng.min(), std::mt19937::min());
    ASSERT_EQ(rng.max(), std::mt19937::max());

    std::array<unsigned, 101> counters{ 0 };
    ASSERT_TRUE(std::ranges::all_of(counters, [](auto counter) { return counter == 0; }));

    constexpr unsigned factor = 1000;
    for (unsigned times = (counters.size() - 1) * factor; times; --times)
    {
        uint8_t index = rng();
        ++counters.at(index);
    }

    EXPECT_EQ(counters.front(), 0);
    std::ranges::for_each(std::ranges::subrange(counters.begin() + 1, counters.end()),
                          [=](const auto& counter) { EXPECT_GE(counter, 0.9 * factor); });
}

TEST(urng_tests, test_urng_i8_min_max)
{
    rand::urng_i8<-2, 3> rng(42);
    ASSERT_EQ(rng.min(), -2);
    ASSERT_EQ(rng.max(), 3);

    std::array<unsigned, 6> counters{ 0 };
    ASSERT_TRUE(std::ranges::all_of(counters, [](auto counter) { return counter == 0; }));

    constexpr unsigned factor = 200;
    for (unsigned times = counters.size() * factor; times; --times)
    {
        uint8_t index = rng() + 2;
        ++counters.at(index);
    }

    std::ranges::for_each(counters, [=](const auto& counter) { EXPECT_GE(counter, 0.9 * factor); });
}

TEST(urng_tests, test_urng_byte_min_max)
{
    rand::urng_byte<std::byte{ 1 }, std::byte{ 100 }> rng(42);
    ASSERT_EQ(rng.min(), std::byte{ 1 });
    ASSERT_EQ(rng.max(), std::byte{ 100 });

    std::array<unsigned, static_cast<unsigned>(decltype(rng)::max()) + 1> counters{ 0 };
    ASSERT_TRUE(std::ranges::all_of(counters, [](auto counter) { return counter == 0; }));

    constexpr unsigned factor = 1000;
    for (unsigned times = (counters.size() - 1) * factor; times; --times)
    {
        std::byte byte = rng();
        unsigned index = static_cast<unsigned>(byte);
        ++counters.at(index);
    }

    EXPECT_EQ(counters.front(), 0);
    std::ranges::for_each(std::ranges::subrange(counters.begin() + 1, counters.end()),
                          [=](const auto& counter) { EXPECT_GE(counter, 0.9 * factor); });
}
