#include <arba/rand/rnrg/xoron64_range_engine.hpp>
#include <arba/rand/rnrg/xorshift_range_engine.hpp>

#include <arba/rand/rnrg/rnrg_benchmark.hpp>
#include <arba/rand/bit_balanced_uints.hpp>

#include <arba/cppx/policy/build_type.hpp>

#include <gtest/gtest.h>

#include <ranges>
#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>

static constexpr std::size_t one_Mb = 1024 * 1024ull;

using rnrg_benchmark_result = rand::rnrg_benchmark_result<rand::bit_balanced_uint64>;

void benchmark_(const std::size_t nb_bytes,
                const std::size_t nb_seeds,
                cppx::EndiannessPolicy auto endianness_policy,
                cppx::ExecutionPolicy auto execution_policy,
                rnrg_benchmark_result& xorshift64_bm_res,
                rnrg_benchmark_result& xoron64_bm_res)
{
    rand::rnrg_benchmark benchmark;
    const auto seeds = rand::bit_balanced_uint64s::enumerators | std::views::take(nb_seeds);
    rand::xorshift64_range_engine<> xorshift64_rnrg;
    xorshift64_bm_res = benchmark.compute(xorshift64_rnrg, seeds, nb_bytes, endianness_policy);
    rand::xoron64_range_engine<> xoron64_rnrg;
    xoron64_bm_res = benchmark.compute(xoron64_rnrg, seeds, nb_bytes, endianness_policy, execution_policy);
}

#ifdef ARBA_CPPX_EXECUTION_ALL_STD_POLICIES
TEST(benchmark_rnrg64s_tests, benchmark_HUD_128Mb_neutral_par)
{
    constexpr std::size_t nb_bytes = 128 * one_Mb + 7;
    rnrg_benchmark_result xorshift64_bm_res;
    rnrg_benchmark_result xoron64_bm_res;
    benchmark_(nb_bytes, 1, cppx::endianness_neutral, std::execution::par, xorshift64_bm_res, xoron64_bm_res);
    ASSERT_LT(xoron64_bm_res.average_execution_duration, xorshift64_bm_res.average_execution_duration);
    ASSERT_GE(xoron64_bm_res.average_homogeneous_byte_distribution_index, 0.999);
    ASSERT_GE(xoron64_bm_res.average_integer_uniqueness_index, 0.9999999);
}

TEST(benchmark_rnrg64s_tests, benchmark_HUD_4Mb_neutral_par)
{
    constexpr std::size_t nb_bytes = 4 * one_Mb + 7;
    rnrg_benchmark_result xorshift64_bm_res;
    rnrg_benchmark_result xoron64_bm_res;
    benchmark_(nb_bytes, 20, cppx::endianness_neutral, std::execution::par, xorshift64_bm_res, xoron64_bm_res);
    ASSERT_LT(xoron64_bm_res.average_execution_duration, xorshift64_bm_res.average_execution_duration);
    ASSERT_GE(xoron64_bm_res.average_homogeneous_byte_distribution_index, 0.999);
    ASSERT_EQ(xoron64_bm_res.average_integer_uniqueness_index, 1.);
}

TEST(benchmark_rnrg64s_tests, benchmark_HUD_4Mb_specific_par)
{
    constexpr std::size_t nb_bytes = 4 *one_Mb + 7;
    rnrg_benchmark_result xorshift64_bm_res;
    rnrg_benchmark_result xoron64_bm_res;
    benchmark_(nb_bytes, 20, cppx::endianness_specific, std::execution::par, xorshift64_bm_res, xoron64_bm_res);
    ASSERT_LT(xoron64_bm_res.average_execution_duration, xorshift64_bm_res.average_execution_duration);
    ASSERT_GE(xoron64_bm_res.average_homogeneous_byte_distribution_index, 0.999);
    ASSERT_EQ(xoron64_bm_res.average_integer_uniqueness_index, 1.);
}
#endif

TEST(benchmark_rnrg64s_tests, benchmark_HUD_4Mb_neutral_seq)
{
    constexpr std::size_t nb_bytes = 4 * one_Mb + 7;
    rnrg_benchmark_result xorshift64_bm_res;
    rnrg_benchmark_result xoron64_bm_res;
    benchmark_(nb_bytes, 20, cppx::endianness_neutral, std::execution::seq, xorshift64_bm_res, xoron64_bm_res);
    ASSERT_LT(xoron64_bm_res.average_execution_duration, xorshift64_bm_res.average_execution_duration);
    ASSERT_GE(xoron64_bm_res.average_homogeneous_byte_distribution_index, 0.999);
    ASSERT_GE(xoron64_bm_res.average_integer_uniqueness_index, 0.9999999);
}

TEST(benchmark_rnrg64s_tests, benchmark_HUD_4Mb_specific_seq)
{
    constexpr std::size_t nb_bytes = 4 * one_Mb + 7;
    rnrg_benchmark_result xorshift64_bm_res;
    rnrg_benchmark_result xoron64_bm_res;
    benchmark_(nb_bytes, 20, cppx::endianness_specific, std::execution::seq, xorshift64_bm_res, xoron64_bm_res);
    ASSERT_LT(xoron64_bm_res.average_execution_duration, xorshift64_bm_res.average_execution_duration);
    ASSERT_GE(xoron64_bm_res.average_homogeneous_byte_distribution_index, 0.999);
    ASSERT_GE(xoron64_bm_res.average_integer_uniqueness_index, 0.9999999);
}

TEST(benchmark_rnrg64s_tests, benchmark_HUD_1Mb_neutral_seq)
{
    constexpr std::size_t nb_bytes = one_Mb + 7;
    rnrg_benchmark_result xorshift64_bm_res;
    rnrg_benchmark_result xoron64_bm_res;
    benchmark_(nb_bytes, 20, cppx::endianness_neutral, std::execution::seq, xorshift64_bm_res, xoron64_bm_res);
    ASSERT_LT(xoron64_bm_res.average_execution_duration, xorshift64_bm_res.average_execution_duration);
    ASSERT_GE(xoron64_bm_res.average_homogeneous_byte_distribution_index, 0.999);
    ASSERT_EQ(xoron64_bm_res.average_integer_uniqueness_index, 1.);
}

TEST(benchmark_rnrg64s_tests, benchmark_HUD_1Mb_specific_seq)
{
    constexpr std::size_t nb_bytes = one_Mb + 7;
    rnrg_benchmark_result xorshift64_bm_res;
    rnrg_benchmark_result xoron64_bm_res;
    benchmark_(nb_bytes, 20, cppx::endianness_specific, std::execution::seq, xorshift64_bm_res, xoron64_bm_res);
    ASSERT_LT(xoron64_bm_res.average_execution_duration, xorshift64_bm_res.average_execution_duration);
    ASSERT_GE(xoron64_bm_res.average_homogeneous_byte_distribution_index, 0.999);
    ASSERT_EQ(xoron64_bm_res.average_integer_uniqueness_index, 1.);
}

TEST(benchmark_rnrg64s_tests, benchmark_HUD_3Kb_neutral_seq)
{
    constexpr std::size_t nb_bytes = 3072 + 7;
    rnrg_benchmark_result xorshift64_bm_res;
    rnrg_benchmark_result xoron64_bm_res;
    benchmark_(nb_bytes, 20, cppx::endianness_neutral, std::execution::seq, xorshift64_bm_res, xoron64_bm_res);
    ASSERT_LT(xoron64_bm_res.average_execution_duration, xorshift64_bm_res.average_execution_duration);
    ASSERT_GE(xoron64_bm_res.average_homogeneous_byte_distribution_index, 0.99);
    ASSERT_EQ(xoron64_bm_res.average_integer_uniqueness_index, 1.);
}

TEST(benchmark_rnrg64s_tests, benchmark_HUD_3Kb_specific_seq)
{
    constexpr std::size_t nb_bytes = 3072 + 7;
    rnrg_benchmark_result xorshift64_bm_res;
    rnrg_benchmark_result xoron64_bm_res;
    benchmark_(nb_bytes, 20, cppx::endianness_specific, std::execution::seq, xorshift64_bm_res, xoron64_bm_res);
    ASSERT_LT(xoron64_bm_res.average_execution_duration, xorshift64_bm_res.average_execution_duration);
    ASSERT_GE(xoron64_bm_res.average_homogeneous_byte_distribution_index, 0.99);
    ASSERT_EQ(xoron64_bm_res.average_integer_uniqueness_index, 1.);
}
