#include <arba/rand/rnrg/xorshift_range_engine.hpp>
#include <arba/rand/rng/xorshift_engine.hpp>
#include <arba/cppx/policy/execution_policy.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

using random_number_generator_t = rand::xorshift32_engine;

TEST(xorshift32_range_engine_tests, constructor__positive_seed__ok)
{
    using random_number_range_generator_t = rand::xorshift32_range_engine<>;

    random_number_range_generator_t rnrg(42);
    EXPECT_EQ(rnrg.seed(), 42);
}

TEST(xorshift32_range_engine_tests, constructor__null_seed__ok)
{
    using random_number_range_generator_t = rand::xorshift32_range_engine<>;
    using integer_t = random_number_range_generator_t::integer_type;

    random_number_generator_t rng(0);
    EXPECT_EQ(rng.seed(), std::numeric_limits<integer_t>::max());
}

TEST(xorshift32_range_engine_tests, generate_random_ints__endianness_specific__ok)
{
    using random_number_range_generator_t = rand::xorshift32_range_engine<4>;
    using integer_t = random_number_range_generator_t::integer_type;
    constexpr std::size_t number_of_seeds = random_number_range_generator_t::number_of_seeds;
    constexpr int rotation_factor = random_number_range_generator_t::rotation_factor;

    const integer_t seed = 42;

    std::array<random_number_generator_t, number_of_seeds> rngs;
    rngs[0] = random_number_generator_t(seed);
    rngs[1] = random_number_generator_t(std::rotl(rngs[0].seed(), rotation_factor));
    rngs[2] = random_number_generator_t(~rngs[0].seed());
    rngs[3] = random_number_generator_t(std::rotr(~rngs[0].seed(), rotation_factor));

    random_number_range_generator_t rnrg(seed);
    constexpr std::size_t nb_iterations = 3;
    std::array<integer_t, (nb_iterations * number_of_seeds)> ints;
    std::ranges::fill(ints, 0);
    rnrg(std::as_writable_bytes(std::span(ints)), cppx::endianness_specific);

    for (std::size_t i = 0; i < number_of_seeds; ++i)
    {
        ASSERT_EQ(ints[i], rngs[i]());
        rngs[i].discard(nb_iterations - 1);
        ASSERT_EQ(ints[(nb_iterations - 1) * number_of_seeds + i], rngs[i].seed());
    }

    ASSERT_EQ(rnrg.seed(), rngs[0]());
}

TEST(xorshift32_range_engine_tests, generate_random_bytes__endianness_specific__ok)
{
    using random_number_range_generator_t = rand::xorshift32_range_engine<4>;
    using integer_t = random_number_range_generator_t::integer_type;
    constexpr std::size_t number_of_seeds = random_number_range_generator_t::number_of_seeds;
    constexpr int rotation_factor = random_number_range_generator_t::rotation_factor;

    const integer_t seed = 42;

    std::array<random_number_generator_t, number_of_seeds> rngs;
    rngs[0] = random_number_generator_t(seed);
    rngs[1] = random_number_generator_t(std::rotl(rngs[0].seed(), rotation_factor));
    rngs[2] = random_number_generator_t(~rngs[0].seed());
    rngs[3] = random_number_generator_t(std::rotr(~rngs[0].seed(), rotation_factor));

    random_number_range_generator_t rnrg(seed);
    const std::size_t remaining_size = sizeof(integer_t) - 1;
    constexpr std::size_t nb_iterations = 3;
    std::array<std::byte, (nb_iterations * number_of_seeds * sizeof(integer_t)) + remaining_size> bytes;
    std::ranges::fill(bytes, std::byte{0});
    rnrg(std::as_writable_bytes(std::span(bytes)), cppx::endianness_specific);

    std::span ints = core::as_writable_span<integer_t>(std::span<std::byte>(bytes), std::nothrow);
    for (std::size_t i = 0; i < number_of_seeds; ++i)
    {
        ASSERT_EQ(ints[i], rngs[i]());
        rngs[i].discard(nb_iterations - 1);
        ASSERT_EQ(ints[(nb_iterations - 1) * number_of_seeds + i], rngs[i].seed());
    }

    const integer_t last_uint = rngs[0]();
    std::span expected_last_bytes = core::as_bytes(last_uint).first(remaining_size);
    std::span last_bytes = std::span(bytes).last(remaining_size);
    ASSERT_TRUE(std::ranges::equal(last_bytes, expected_last_bytes));

    ASSERT_EQ(rnrg.seed(), rngs[0]());
}

TEST(xorshift32_range_engine_tests, determinist_generating__ok)
{
    using random_number_range_generator_t = rand::xorshift32_range_engine<>;
    using integer_t = random_number_range_generator_t::integer_type;

    const integer_t seed = 72;

    random_number_range_generator_t rnrg(seed);
    const std::size_t container_size = 32 * sizeof(integer_t) - 1;
    std::array<std::byte, container_size> alpha_bytes;
    std::array<std::byte, container_size> beta_bytes;
    std::ranges::fill(alpha_bytes, std::byte{0});
    std::ranges::fill(beta_bytes, std::byte{0});
    rnrg(std::span(alpha_bytes), cppx::endianness_specific);
    rnrg = random_number_range_generator_t(seed);
    rnrg(std::span(beta_bytes), cppx::endianness_specific);
    ASSERT_TRUE(std::ranges::equal(alpha_bytes, beta_bytes));
}

TEST(xorshift32_range_engine_tests, generate_random__endianness_neutral__ok)
{
    using random_number_range_generator_t = rand::xorshift32_range_engine<>;
    using integer_t = random_number_range_generator_t::integer_type;

    const integer_t seed = 72;

    random_number_range_generator_t rnrg(seed);
    const std::size_t container_size = 32;
    std::array<integer_t, container_size> alpha_ints;
    std::array<integer_t, container_size> beta_ints;
    std::ranges::fill(alpha_ints, 0);
    std::ranges::fill(beta_ints, 0);
    rnrg(std::as_writable_bytes(std::span(alpha_ints)), cppx::endianness_specific);
    rnrg = random_number_range_generator_t(seed);
    rnrg(std::as_writable_bytes(std::span(beta_ints)), cppx::endianness_neutral);

    bool expected_cmp_result = std::endian::native == std::endian::big;
    ASSERT_TRUE(std::ranges::equal(alpha_ints, beta_ints) == expected_cmp_result);
    ASSERT_EQ(alpha_ints.front(), core::wtoh_when(beta_ints.front(), cppx::endianness_neutral));
}

TEST(xorshift32_engine_tests, seed__s__ok)
{
    using random_number_range_generator_t = rand::xorshift32_range_engine<>;
    using integer_t = random_number_range_generator_t::integer_type;

    const integer_t seed = 72;

    random_number_range_generator_t rnrg(seed);
    const std::size_t container_size = 32 * sizeof(integer_t) - 1;
    std::array<std::byte, container_size> alpha_bytes;
    std::array<std::byte, container_size> beta_bytes;
    std::ranges::fill(alpha_bytes, std::byte{0});
    std::ranges::fill(beta_bytes, std::byte{0});
    rnrg(std::span(alpha_bytes), cppx::endianness_specific);
    rnrg.seed(seed);
    rnrg(std::span(beta_bytes), cppx::endianness_specific);
    ASSERT_TRUE(std::ranges::equal(alpha_bytes, beta_bytes));
}

TEST(xorshift32_range_engine_tests, discard__n__ok)
{
    using random_number_range_generator_t = rand::xorshift32_range_engine<>;
    using integer_t = random_number_range_generator_t::integer_type;

    const integer_t seed = 72;

    random_number_range_generator_t rnrg(seed);
    const std::size_t container_size = 32 * sizeof(integer_t) - 1;
    std::array<std::byte, container_size> alpha_bytes;
    std::array<std::byte, container_size> beta_bytes;
    std::ranges::fill(alpha_bytes, std::byte{0});
    std::ranges::fill(beta_bytes, std::byte{0});
    rnrg.discard(5);
    rnrg(std::span(alpha_bytes), cppx::endianness_specific);
    random_number_generator_t rng(seed);
    rng.discard(5);
    rnrg = random_number_range_generator_t(rng.seed());
    rnrg(std::span(beta_bytes), cppx::endianness_specific);
    ASSERT_TRUE(std::ranges::equal(alpha_bytes, beta_bytes));
}

TEST(xorshift32_range_engine_tests, generate_random_xorshift32__bytes__ok)
{
    using random_number_range_generator_t = rand::xorshift32_range_engine<>;
    using integer_t = random_number_range_generator_t::integer_type;

    const integer_t seed = 72;

    random_number_range_generator_t rnrg(seed);
    const std::size_t container_size = 32 * sizeof(integer_t) - 1;
    std::array<std::byte, container_size> alpha_bytes;
    std::array<std::byte, container_size> beta_bytes;
    std::ranges::fill(alpha_bytes, std::byte{0});
    std::ranges::fill(beta_bytes, std::byte{0});
    rnrg(std::span(alpha_bytes), cppx::endianness_specific);
    rand::generate_random_xorshift32(std::span(beta_bytes), seed, cppx::endianness_specific);
    ASSERT_TRUE(std::ranges::equal(alpha_bytes, beta_bytes));
}

TEST(xorshift32_range_engine_tests, generate_random_xorshift32__ints__ok)
{
    using random_number_range_generator_t = rand::xorshift32_range_engine<>;
    using integer_t = random_number_range_generator_t::integer_type;

    const integer_t seed = 72;

    random_number_range_generator_t rnrg(seed);
    const std::size_t container_size = 32;
    std::array<integer_t, container_size> alpha_ints;
    std::array<integer_t, container_size> beta_ints;
    std::ranges::fill(alpha_ints, 0);
    std::ranges::fill(beta_ints, 0);
    rnrg(std::span(alpha_ints), cppx::endianness_specific);
    rand::generate_random_xorshift32(std::span(beta_ints), seed, cppx::endianness_specific);
    ASSERT_TRUE(std::ranges::equal(alpha_ints, beta_ints));
}
