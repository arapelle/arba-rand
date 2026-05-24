#include <arba/rand/rnrg/xorshift_range_engine.hpp>

#include <arba/core/bit/htow.hpp>
#include <arba/cppx/policy/endianness_policy.hpp>

#include <chrono>
#include <cstdlib>
#include <iostream>

int main()
{
    using duration_type = std::chrono::duration<float, ::std::chrono::milliseconds::period>;
    using clock_type = std::chrono::steady_clock;
    using time_point_type = std::chrono::time_point<clock_type>;

    using random_number_range_generator_t = rand::xorshift64_range_engine<>;
    using integer_t = typename random_number_range_generator_t::integer_type;

    // Init byte storage and seed.
    const std::size_t remaining_size = sizeof(integer_t) - 1;
    std::vector<std::byte> bytes(1024 * 1024 * 1024 + remaining_size, std::byte{ 0 });
    const integer_t seed = 42;

    time_point_type start_time_point = clock_type::now();
    // Run rnrg algorithm.
    random_number_range_generator_t rnrg(seed);
    rnrg(std::as_writable_bytes(std::span(bytes)), cppx::endianness_specific);
    // End run.
    duration_type duration = std::chrono::duration_cast<duration_type>(clock_type::now() - start_time_point);

    std::cout << "Duration 'xorshift64_range_engine': " << duration.count() << "ms" << std::endl;

    std::cout << "EXAMPLE SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
