#include <arba/rand/rng/xorshift_engine.hpp>
#include <arba/rand/rand.hpp>

#include <gtest/gtest.h>

#include <algorithm>

using random_number_generator_t = rand::xorshift64_engine;
using result_t = random_number_generator_t::result_type;

static_assert(std::uniform_random_bit_generator<random_number_generator_t>);
static_assert(random_number_generator_t::min() == std::numeric_limits<result_t>::min());
static_assert(random_number_generator_t::max() == std::numeric_limits<result_t>::max());

TEST(xorshift64_engine_tests, xorshift64_engine__positive_seed__ok)
{
    random_number_generator_t rng(42);
    EXPECT_EQ(rng.seed(), 42);
    const result_t first = rng();
    EXPECT_NE(rng.seed(), 42);
    const result_t second = rng();
    EXPECT_NE(rng.seed(), 42);
    EXPECT_NE(first, second);
}

TEST(xorshift64_engine_tests, xorshift64_engine__null_seed__ok)
{
    random_number_generator_t rng(0);
    EXPECT_EQ(rng.seed(), std::numeric_limits<result_t>::max());
    const result_t first = rng();
    EXPECT_NE(first, 0);
    const result_t second = rng();
    EXPECT_NE(second, 0);
    EXPECT_NE(first, second);
}

TEST(xorshift64_engine_tests, seed__n__ok)
{
    random_number_generator_t rng(42);
    const result_t value = rng();
    rng.seed(42);
    const result_t value_2 = rng();
    ASSERT_EQ(value, value_2);
    ASSERT_EQ(value, 45'454'805'674ull);
    ASSERT_EQ(value_2, 45'454'805'674ull);
}

TEST(xorshift64_engine_tests, discard__n__ok)
{
    const result_t first = rand::xorshift64(rand::xorshift64(rand::xorshift64(42)));
    random_number_generator_t rng_2(42);
    rng_2.discard(2);
    const result_t second = rng_2();
    EXPECT_EQ(first, second);
}

TEST(xorshift64_engine_tests, xorshift64_engine__distribution__balanced)
{
    random_number_generator_t rng(42);

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
