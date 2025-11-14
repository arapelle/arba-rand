#include <arba/rand/bit_balanced_uints.hpp>
#include <arba/rand/rng/xorshift_engine.hpp>
#include <arba/rand/rnrg/rnrg_benchmark.hpp>
#include <arba/rand/rnrg/xoron64_range_engine.hpp>
#include <arba/rand/rnrg/xorshift_range_engine.hpp>

#include <arba/cppx/policy/execution_policy.hpp>
#include <gtest/gtest.h>

#include <algorithm>
#include <ranges>
#include <vector>

using random_number_generator_t = rand::xorshift64_engine;

TEST(xoron64_range_engine_tests, constructor__positive_seed__ok)
{
    using random_number_range_generator_t = rand::xoron64_range_engine<>;

    random_number_range_generator_t rnrg(42);
    EXPECT_EQ(rnrg.seed(), 42);
}

TEST(xoron64_range_engine_tests, constructor__null_seed__ok)
{
    using random_number_range_generator_t = rand::xoron64_range_engine<>;
    using integer_t = random_number_range_generator_t::integer_type;

    random_number_generator_t rng(0);
    EXPECT_EQ(rng.seed(), std::numeric_limits<integer_t>::max());
}

TEST(xoron64_range_engine_tests, generate_random_bytes__le_default_init_range_size__ok)
{
    using random_number_range_generator_t = rand::xoron64_range_engine<>;
    using integer_t = random_number_range_generator_t::integer_type;

    const integer_t seed = 42;

    random_number_range_generator_t rnrg(seed);
    const std::size_t container_size = 2000;
    std::array<std::byte, container_size> alpha_bytes, beta_bytes;
    std::ranges::fill(alpha_bytes, std::byte{ 0 });
    std::ranges::fill(beta_bytes, std::byte{ 0 });
    rnrg(std::span(alpha_bytes), cppx::endianness_specific);
    rand::xorshift64_range_engine<> xs64rg(seed);
    xs64rg(std::span(beta_bytes), cppx::endianness_specific);
    ASSERT_TRUE(std::ranges::equal(alpha_bytes, beta_bytes));
}

TEST(xoron64_range_engine_tests, generate_random_bytes__gt_default_init_range_size__ok)
{
    using random_number_range_generator_t = rand::xoron64_range_engine<>;
    using integer_t = random_number_range_generator_t::integer_type;

    const integer_t seed = 42;

    random_number_range_generator_t rnrg(seed);
    const std::size_t container_size = 4000;
    std::array<std::byte, container_size> alpha_bytes, beta_bytes;
    std::ranges::fill(alpha_bytes, std::byte{ 0 });
    std::ranges::fill(beta_bytes, std::byte{ 0 });
    rnrg(std::span(alpha_bytes), cppx::endianness_specific);
    rand::xorshift64_range_engine<> xs64rg(seed);
    xs64rg(std::span(beta_bytes), cppx::endianness_specific);
    ASSERT_FALSE(std::ranges::equal(alpha_bytes, beta_bytes));
}

TEST(xoron64_range_engine_tests, generate_random_bytes__check_operations__ok)
{
    using random_number_range_generator_t = rand::xoron64_range_engine<>;
    using integer_t = random_number_range_generator_t::integer_type;

    const integer_t seed = 42;

    random_number_range_generator_t rnrg(seed);
    const std::size_t container_size = 4000;
    std::array<std::byte, container_size> bytes_array;
    std::span bytes(bytes_array);
    std::ranges::fill(bytes, std::byte{ 0 });
    rnrg(bytes, cppx::endianness_specific);

    integer_t n = core::as_uint<sizeof(integer_t) * 8>(bytes.subspan(1, 8));
    n ^= random_number_range_generator_t::alpha_xor_mask;
    n = std::rotl(n, 3);

    integer_t w = core::as_uint<sizeof(integer_t) * 8>(bytes.subspan(2048, 8));
    ASSERT_EQ(n, w);
}

TEST(xoron64_range_engine_tests, determinist_generating__ok)
{
    using random_number_range_generator_t = rand::xoron64_range_engine<>;
    using integer_t = random_number_range_generator_t::integer_type;

    const integer_t seed = 72;

    random_number_range_generator_t rnrg(seed);
    const std::size_t container_size = 4103 * sizeof(integer_t) - 1;
    std::array<std::byte, container_size> alpha_bytes;
    std::array<std::byte, container_size> beta_bytes;
    std::ranges::fill(alpha_bytes, std::byte{ 0 });
    std::ranges::fill(beta_bytes, std::byte{ 0 });
    rnrg(std::span(alpha_bytes), cppx::endianness_specific);
    rnrg = random_number_range_generator_t(seed);
    rnrg(std::span(beta_bytes), cppx::endianness_specific);
    ASSERT_TRUE(std::ranges::equal(alpha_bytes, beta_bytes));
}

TEST(xoron64_range_engine_tests, generate_random__endianness_neutral__ok)
{
    using random_number_range_generator_t = rand::xoron64_range_engine<>;
    using integer_t = random_number_range_generator_t::integer_type;

    const integer_t seed = 72;

    random_number_range_generator_t rnrg(seed);
    const std::size_t container_size = 4103 * sizeof(integer_t) - 1;
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

TEST(xoron64_range_engine_tests, seed__s__ok)
{
    using random_number_range_generator_t = rand::xoron64_range_engine<>;
    using integer_t = random_number_range_generator_t::integer_type;

    const integer_t seed = 72;

    random_number_range_generator_t rnrg(seed);
    const std::size_t container_size = 32 * sizeof(integer_t) - 1;
    std::array<std::byte, container_size> alpha_bytes;
    std::array<std::byte, container_size> beta_bytes;
    std::ranges::fill(alpha_bytes, std::byte{ 0 });
    std::ranges::fill(beta_bytes, std::byte{ 0 });
    rnrg(std::span(alpha_bytes), cppx::endianness_specific);
    rnrg.seed(seed);
    rnrg(std::span(beta_bytes), cppx::endianness_specific);
    ASSERT_TRUE(std::ranges::equal(alpha_bytes, beta_bytes));
}

TEST(xoron64_range_engine_tests, discard__n__ok)
{
    using random_number_range_generator_t = rand::xoron64_range_engine<>;
    using integer_t = random_number_range_generator_t::integer_type;

    const integer_t seed = 72;

    random_number_range_generator_t rnrg(seed);
    const std::size_t container_size = 32 * sizeof(integer_t) - 1;
    std::array<std::byte, container_size> alpha_bytes;
    std::array<std::byte, container_size> beta_bytes;
    std::ranges::fill(alpha_bytes, std::byte{ 0 });
    std::ranges::fill(beta_bytes, std::byte{ 0 });
    rnrg.discard(5);
    rnrg(std::span(alpha_bytes), cppx::endianness_specific);
    random_number_generator_t rng(seed);
    rng.discard(5);
    rnrg = random_number_range_generator_t(rng.seed());
    rnrg(std::span(beta_bytes), cppx::endianness_specific);
    ASSERT_TRUE(std::ranges::equal(alpha_bytes, beta_bytes));
}

TEST(xoron64_range_engine_tests, rnrg_benchmark)
{
    using random_number_range_generator_t = rand::xoron64_range_engine<>;

    random_number_range_generator_t rnrg;
    rand::rnrg_benchmark benchmark;
    const rand::rnrg_benchmark_result bm_res =
        benchmark.compute(rnrg, rand::bit_balanced_uint64s::enumerators, 1024 * 1024 + 7);
    std::cout << "AH: " << bm_res.average_homogeneous_byte_distribution_index << std::endl;
    std::cout << "AU: " << bm_res.average_integer_uniqueness_index << std::endl;
    std::cout << "AD: " << bm_res.average_execution_duration << std::endl;
    ASSERT_GT(bm_res.average_homogeneous_byte_distribution_index, 0.99);
    ASSERT_GT(bm_res.average_integer_uniqueness_index, 0.90);
}
