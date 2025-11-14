#pragma once

#include <arba/cppx/policy/endianness_policy.hpp>
#include <arba/cppx/policy/execution_policy.hpp>
#include <arba/core/container/span.hpp>

#include <chrono>
#include <numeric>
#include <span>
#include <vector>
#include <algorithm>
#include <limits>
#include <concepts>
#include <cstdint>
#include <cassert>

inline namespace arba
{
namespace rand
{

template <typename SeedT>
class rnrg_benchmark_result
{
public:
    using seed_type = SeedT;

    std::size_t range_byte_size = 0;
    std::vector<seed_type> seeds;
    std::vector<double> homogeneous_byte_distribution_indexes;
    std::vector<double> integer_uniqueness_indexes;
    std::vector<double> execution_durations;
    double average_homogeneous_byte_distribution_index = std::numeric_limits<double>::quiet_NaN();
    double average_integer_uniqueness_index = std::numeric_limits<double>::quiet_NaN();
    double average_execution_duration = std::numeric_limits<double>::quiet_NaN();

    void reserve(std::size_t nb_seeds)
    {
        seeds.reserve(nb_seeds);
        homogeneous_byte_distribution_indexes.reserve(nb_seeds);
        integer_uniqueness_indexes.reserve(nb_seeds);
        execution_durations.reserve(nb_seeds);
    }
};

class rnrg_benchmark
{
public:
    using duration_type = std::chrono::duration<double, ::std::chrono::milliseconds::period>;
    using clock_type = std::chrono::steady_clock;
    using time_point_type = std::chrono::time_point<clock_type>;

    bool compute_homogeneous_byte_distribution_index :1 = true;
    bool compute_integer_uniqueness_index :1 = true;

    template <class RnrgT, std::ranges::range SeedRangeT>
    requires (std::convertible_to<std::ranges::range_value_t<SeedRangeT>, typename RnrgT::integer_type>)
        rnrg_benchmark_result<std::ranges::range_value_t<SeedRangeT>>
        compute(RnrgT& rnrg, const SeedRangeT& seeds, std::size_t range_byte_size) const
    {
        return compute(rnrg, seeds, range_byte_size, cppx::endianness_specific, std::execution::seq);
    }

    template <class RnrgT, std::ranges::range SeedRangeT>
    requires (std::convertible_to<std::ranges::range_value_t<SeedRangeT>, typename RnrgT::integer_type>)
        rnrg_benchmark_result<std::ranges::range_value_t<SeedRangeT>>
        compute(RnrgT& rnrg, const SeedRangeT& seeds, std::size_t range_byte_size,
                cppx::EndiannessPolicy auto endianness_policy) const
    {
        return compute(rnrg, seeds, range_byte_size, endianness_policy, std::execution::seq);
    }

    template <class RnrgT, std::ranges::range SeedRangeT>
    requires (std::convertible_to<std::ranges::range_value_t<SeedRangeT>, typename RnrgT::integer_type>)
        rnrg_benchmark_result<std::ranges::range_value_t<SeedRangeT>>
        compute(RnrgT& rnrg, const SeedRangeT& seeds, std::size_t range_byte_size,
                cppx::ExecutionPolicy auto execution_policy) const
    {
        return compute(rnrg, seeds, range_byte_size, cppx::endianness_specific, execution_policy);
    }

    template <class RnrgT, std::ranges::range SeedRangeT>
        requires (std::convertible_to<std::ranges::range_value_t<SeedRangeT>, typename RnrgT::integer_type>)
    rnrg_benchmark_result<std::ranges::range_value_t<SeedRangeT>>
    compute(RnrgT& rnrg, const SeedRangeT& seeds, std::size_t range_byte_size,
            cppx::EndiannessPolicy auto endianness_policy,
            cppx::ExecutionPolicy auto execution_policy) const
    {
        using integer_t = typename RnrgT::integer_type;

        assert(range_byte_size > 0);
        std::vector<std::byte> bytes(range_byte_size, std::byte{0});

        rnrg_benchmark_result<std::ranges::range_value_t<SeedRangeT>> bm_res;
        bm_res.range_byte_size = range_byte_size;
        bm_res.reserve(seeds.size());
        std::array<std::size_t, 256> byte_counters;
        const std::span ints = core::as_writable_span<integer_t>(std::span(bytes));
        for (auto seed : seeds)
        {
            rnrg.seed(seed);
            bm_res.seeds.push_back(seed);
            std::ranges::fill(bytes, std::byte{0});

            const time_point_type start_time_point = clock_type::now();
            if constexpr (requires{ rnrg(std::span(bytes), endianness_policy, execution_policy); })
                rnrg(std::span(bytes), endianness_policy, execution_policy);
            else
                rnrg(std::span(bytes), endianness_policy);
            const duration_type duration = std::chrono::duration_cast<duration_type>(clock_type::now() - start_time_point);
            bm_res.execution_durations.push_back(duration.count());

            if (compute_homogeneous_byte_distribution_index)
            {
                byte_counters.fill(0);
                for (std::byte val : bytes)
                    ++byte_counters[static_cast<uint8_t>(val)];
                const auto min_iter = std::ranges::min_element(byte_counters);
                const auto max_iter = std::ranges::max_element(byte_counters);
                const double homogeneous_byte_distribution_index = 1. - double(*max_iter - *min_iter) / bytes.size();
                bm_res.homogeneous_byte_distribution_indexes.push_back(homogeneous_byte_distribution_index);
            }
            else
                bm_res.homogeneous_byte_distribution_indexes.push_back(std::numeric_limits<double>::quiet_NaN());

            if (compute_integer_uniqueness_index)
            {
                std::sort(ints.begin(), ints.end());
                const auto uniq_iter = std::unique(ints.begin(), ints.end());
                const double integer_uniqueness_index = std::distance(ints.begin(), uniq_iter) / double(ints.size());
                bm_res.integer_uniqueness_indexes.push_back(integer_uniqueness_index);
            }
            else
                bm_res.integer_uniqueness_indexes.push_back(std::numeric_limits<double>::quiet_NaN());
        }

        // clang-format off
        if (compute_homogeneous_byte_distribution_index)
            bm_res.average_homogeneous_byte_distribution_index =
                std::reduce(bm_res.homogeneous_byte_distribution_indexes.cbegin(),
                            bm_res.homogeneous_byte_distribution_indexes.cend()) / seeds.size();
        if (compute_integer_uniqueness_index)
            bm_res.average_integer_uniqueness_index =
                std::reduce(bm_res.integer_uniqueness_indexes.cbegin(),
                            bm_res.integer_uniqueness_indexes.cend()) / seeds.size();
        bm_res.average_execution_duration =
            std::reduce(bm_res.execution_durations.cbegin(),
                        bm_res.execution_durations.cend()) / seeds.size();
        // clang-format on

        return bm_res;
    }
};

} // namepsace rand
} // namepsace arba
