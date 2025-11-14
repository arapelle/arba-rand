#include <arba/rand/rnrg/xoron64_range_engine.hpp>
#include <arba/rand/rnrg/xorshift_range_engine.hpp>

#include <arba/rand/rnrg/rnrg_benchmark.hpp>
#include <arba/rand/bit_balanced_uints.hpp>

#include <ranges>
#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>

class benchmark_rng64s
{
public:
    static constexpr std::size_t one_Gb = 1024 * 1024 * 1024ull;
    static constexpr std::size_t one_Mb = 1024 * 1024ull;
    static constexpr std::size_t one_Kb = 1024ull;

private:
    static std::string_view endianness_str_(cppx::EndiannessPolicy auto endianness_policy)
    {
        if constexpr (std::is_same_v<decltype(endianness_policy), cppx::endianness_specific_t>)
            return "specific";
        else
            return "neutral";
    }

    static std::string indexes_str_(const rand::rnrg_benchmark& benchmark)
    {
        return std::format("{}{}D",
                           benchmark.compute_homogeneous_byte_distribution_index ? "H" : "",
                           benchmark.compute_integer_uniqueness_index ? "U" : "");
    }

    static std::string range_size_str_(std::size_t range_size)
    {
        if (range_size >= one_Gb)
            return std::format("{}Gb", range_size / one_Gb);
        if (range_size >= one_Mb)
            return std::format("{}Mb", range_size / one_Mb);
        if (range_size >= one_Kb)
            return std::format("{}Kb", range_size / one_Kb);
        return std::format("{}", range_size);
    }

public:
    bool default_print_only_average = true;

private:
    void print_benchmark_result_(std::string_view title, const rand::rnrg_benchmark& benchmark, auto const& bm_res)
    {
        print_benchmark_result_(title, benchmark, bm_res, default_print_only_average);
    }

    void print_benchmark_result_(std::string_view title, const rand::rnrg_benchmark& benchmark, auto const& bm_res,
                                bool print_only_average)
    {
        std::cout << "## " << title << std::endl;
        if (!print_only_average)
        {
            for (unsigned i = 0; i < bm_res.seeds.size(); ++i)
            {
                std::cout << "  ";
                if (benchmark.compute_homogeneous_byte_distribution_index)
                    std::cout << "  H: " << std::fixed << std::setprecision(10) << bm_res.homogeneous_byte_distribution_indexes[i];
                if (benchmark.compute_integer_uniqueness_index)
                    std::cout << "  U: " << std::fixed << std::setprecision(10) << bm_res.integer_uniqueness_indexes[i];
                std::cout << "  D(ms): " << std::fixed << std::setprecision(6) << bm_res.execution_durations[i]
                          << "  seed(" << bm_res.seeds[i].name() << ")"
                          << std::endl;
            }
            std::cout << "    ----------" << std::endl;
        }
        std::cout << "  ";
        if (benchmark.compute_homogeneous_byte_distribution_index)
            std::cout << "  H: " << std::fixed << std::setprecision(10) << bm_res.average_homogeneous_byte_distribution_index;;
        if (benchmark.compute_integer_uniqueness_index)
            std::cout << "  U: " << std::fixed << std::setprecision(10) << bm_res.average_integer_uniqueness_index;
        std::cout << "  D(ms): " << std::fixed << std::setprecision(6) << bm_res.average_execution_duration
                  << "  Average"
                  << std::endl;
    }

    void print_header_(const rand::rnrg_benchmark& benchmark,
                      std::size_t nb_bytes, cppx::EndiannessPolicy auto endianness_policy, std::size_t nb_seeds)
    {
        std::cout << "----------------------------------------------------------------------" << std::endl;
        std::cout << std::format("# {} - ({}, {}) - nb_seeds={}", indexes_str_(benchmark),
                                 range_size_str_(nb_bytes), endianness_str_(endianness_policy), nb_seeds) << std::endl;
    }

public:
    void benchmark_HUD__1Go_neutral(std::size_t nb_seeds = 1)
    {
        constexpr std::size_t nb_bytes = one_Gb + 7;
        constexpr auto endianness_policy = cppx::endianness_neutral;
        const auto seeds = rand::bit_balanced_uint64s::enumerators | std::views::take(nb_seeds);
        const rand::rnrg_benchmark benchmark;

        print_header_(benchmark, nb_bytes, endianness_policy, nb_seeds);
        {
            using random_number_range_generator_t = rand::xorshift64_range_engine<>;
            random_number_range_generator_t rnrg;
            const rand::rnrg_benchmark_result bm_res = benchmark.compute(rnrg, seeds, nb_bytes, endianness_policy);
            print_benchmark_result_("rand::xorshift64_range_engine<>", benchmark, bm_res, false);
        }
        {
            using random_number_range_generator_t = rand::xoron64_range_engine<>;
            random_number_range_generator_t rnrg;
            const rand::rnrg_benchmark_result bm_res = benchmark.compute(rnrg, seeds, nb_bytes, endianness_policy, std::execution::seq);
            print_benchmark_result_("rand::xoron64_range_engine<> seq", benchmark, bm_res, false);
        }
    }

private:
    void benchmark_(const rand::rnrg_benchmark& benchmark,
                    const std::size_t nb_bytes,
                    cppx::EndiannessPolicy auto endianness_policy,
                    std::size_t nb_seeds)
    {
        const auto seeds = rand::bit_balanced_uint64s::enumerators | std::views::take(nb_seeds);
        print_header_(benchmark, nb_bytes, endianness_policy, nb_seeds);
        {
            using random_number_range_generator_t = rand::xorshift64_range_engine<>;
            random_number_range_generator_t rnrg;
            const rand::rnrg_benchmark_result bm_res = benchmark.compute(rnrg, seeds, nb_bytes, endianness_policy);
            print_benchmark_result_("rand::xorshift64_range_engine<>", benchmark, bm_res);
        }
        {
            using random_number_range_generator_t = rand::xoron64_range_engine<>;
            random_number_range_generator_t rnrg;
            const rand::rnrg_benchmark_result bm_res = benchmark.compute(rnrg, seeds, nb_bytes, endianness_policy, std::execution::seq);
            print_benchmark_result_("rand::xoron64_range_engine<> seq", benchmark, bm_res);
        }
        {
            using random_number_range_generator_t = rand::xoron64_range_engine<>;
            random_number_range_generator_t rnrg;
            const rand::rnrg_benchmark_result bm_res = benchmark.compute(rnrg, seeds, nb_bytes, endianness_policy, std::execution::par);
            print_benchmark_result_("rand::xoron64_range_engine<> par", benchmark, bm_res);
        }
    }

public:
    void benchmark_D__1Go(cppx::EndiannessPolicy auto endianness_policy,
                         std::size_t nb_seeds = rand::bit_balanced_uint64s::enumerators.size())
    {
        constexpr std::size_t nb_bytes = one_Gb + 7;
        const rand::rnrg_benchmark benchmark{ false, false };
        benchmark_(benchmark, nb_bytes, endianness_policy, nb_seeds);
    }

    void benchmark_HUD__1Mo(cppx::EndiannessPolicy auto endianness_policy,
                           std::size_t nb_seeds = rand::bit_balanced_uint64s::enumerators.size())
    {
        constexpr std::size_t nb_bytes = one_Mb + 7;
        const rand::rnrg_benchmark benchmark;
        benchmark_(benchmark, nb_bytes, endianness_policy, nb_seeds);
    }
};

int main()
{
    benchmark_rng64s benchmark;
    benchmark.benchmark_HUD__1Go_neutral(4);
    benchmark.benchmark_D__1Go(cppx::endianness_neutral);
    benchmark.benchmark_D__1Go(cppx::endianness_specific);
    benchmark.benchmark_HUD__1Mo(cppx::endianness_neutral);
    benchmark.benchmark_HUD__1Mo(cppx::endianness_specific);

    std::cout << "EXIT SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
