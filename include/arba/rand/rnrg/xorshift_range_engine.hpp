#pragma once

#include <arba/rand/xorshift.hpp>
#include <arba/core/bit/htow_when.hpp>
#include <arba/core/container/span.hpp>
#include <arba/cppx/policy/endianness_policy.hpp>

#include <array>
#include <span>
#include <algorithm>
#include <random>

inline namespace arba
{
namespace rand
{

template <std::size_t SeedCount = 8, int RotationFactor = 3>
    requires (SeedCount > 0 && (SeedCount & 1) == 0 && RotationFactor != 0)
class xorshift32_range_engine
{
public:
    using integer_type = uint32_t;
    static constexpr std::size_t number_of_seeds = SeedCount;
    static constexpr int rotation_factor = RotationFactor;

    inline explicit xorshift32_range_engine(integer_type seed) : state_(seed)
    {
        if (state_ == 0) [[unlikely]]
            --state_;
    }

    xorshift32_range_engine() : xorshift32_range_engine(std::random_device{}()) {}

    [[nodiscard]] inline integer_type seed() const { return state_; }

    inline void seed(integer_type value)
    {
        state_ = xorshift32_range_engine(value).seed();
    }

    inline void discard(unsigned long long times)
    {
        for (; times > 0; --times)
            state_ = xorshift32(state_);
    }

    std::span<std::byte> operator()(const std::span<std::byte> bytes, cppx::EndiannessPolicy auto endianness_policy);

    std::span<integer_type> operator()(const std::span<integer_type> integers, cppx::EndiannessPolicy auto endianness_policy)
    {
        (*this)(std::as_writable_bytes(integers), endianness_policy);
        return integers;
    }

private:
    integer_type state_;
};

template <std::size_t SeedCount, int RotationFactor>
    requires (SeedCount > 0 && (SeedCount & 1) == 0 && RotationFactor != 0)
std::span<std::byte> xorshift32_range_engine<SeedCount, RotationFactor>::operator()(const std::span<std::byte> bytes,
                                                                                    cppx::EndiannessPolicy auto endianness_policy)
{
    const std::size_t seed_count =  SeedCount;
    integer_type states[seed_count];
    std::size_t i = 0;
    for (std::size_t end_i = seed_count / 2; i < end_i; ++i)
        states[i] = std::rotl(state_, i * RotationFactor);
    for (std::size_t j = 0; i < seed_count; ++i, ++j)
        states[i] = std::rotr(~state_, j * RotationFactor);

    i = 0;
    const std::span uints = core::as_writable_span<integer_type>(bytes);
    auto xorshift_fn = [&](integer_type& val)
    {
        uint32_t& state = states[i++ % seed_count];
        state = xorshift32(state);
        val = core::htow_when(state, endianness_policy);
    };
    std::for_each(uints.begin(), uints.end(), xorshift_fn);

    state_ = xorshift32(states[0]);

    if (std::size_t remaining_bytes = bytes.size() & (sizeof(integer_type) - 1); remaining_bytes > 0)
    {
        const std::span output_bytes = bytes.last(remaining_bytes);
        integer_type state = core::htow_when(state_, endianness_policy);
        const std::span input_bytes = core::as_writable_bytes(state).first(remaining_bytes);
        std::ranges::copy(input_bytes, output_bytes.begin());
        state_ = xorshift32(state_);
    }

    return bytes;
}

inline std::span<std::byte> generate_random_xorshift32(std::span<std::byte> bytes,
                                                       xorshift32_range_engine<>::integer_type seed,
                                                       cppx::EndiannessPolicy auto endianness_policy = cppx::endianness_specific)
{
    xorshift32_range_engine<> rnrg(seed);
    return rnrg(bytes, endianness_policy);
}

inline std::span<std::byte> generate_random_xorshift32(std::span<std::byte> bytes,
                                                       cppx::EndiannessPolicy auto endianness_policy = cppx::endianness_specific)
{
    return generate_random_xorshift32(bytes, std::random_device{}(), endianness_policy);
}

inline std::span<xorshift32_range_engine<>::integer_type>
generate_random_xorshift32(std::span<xorshift32_range_engine<>::integer_type> ints,
                           xorshift32_range_engine<>::integer_type seed,
                           cppx::EndiannessPolicy auto endianness_policy = cppx::endianness_specific)
{
    generate_random_xorshift32(std::as_writable_bytes(ints), seed, endianness_policy);
    return ints;
}

inline std::span<xorshift32_range_engine<>::integer_type>
generate_random_xorshift32(std::span<xorshift32_range_engine<>::integer_type> ints,
                           cppx::EndiannessPolicy auto endianness_policy = cppx::endianness_specific)
{
    return generate_random_xorshift32(ints, std::random_device{}(), endianness_policy);
}

template <std::size_t SeedCount = 8, int RotationFactor = 3>
    requires (SeedCount > 0 && (SeedCount & 1) == 0 && RotationFactor != 0)
class xorshift64_range_engine
{
public:
    using integer_type = uint64_t;
    static constexpr std::size_t number_of_seeds = SeedCount;
    static constexpr int rotation_factor = RotationFactor;

    inline explicit xorshift64_range_engine(integer_type seed) : state_(seed)
    {
        if (state_ == 0) [[unlikely]]
            --state_;
    }

    xorshift64_range_engine() : xorshift64_range_engine(std::random_device{}()) {}

    [[nodiscard]] inline integer_type seed() const { return state_; }

    inline void seed(integer_type value)
    {
        state_ = xorshift64_range_engine(value).seed();
    }

    inline void discard(unsigned long long times)
    {
        for (; times > 0; --times)
            state_ = xorshift64(state_);
    }

    std::span<std::byte> operator()(const std::span<std::byte> bytes, cppx::EndiannessPolicy auto endianness_policy);

    std::span<integer_type> operator()(const std::span<integer_type> integers, cppx::EndiannessPolicy auto endianness_policy)
    {
        (*this)(std::as_writable_bytes(integers), endianness_policy);
        return integers;
    }

private:
    integer_type state_;
};

template <std::size_t SeedCount, int RotationFactor>
requires (SeedCount > 0 && (SeedCount & 1) == 0 && RotationFactor != 0)
std::span<std::byte> xorshift64_range_engine<SeedCount, RotationFactor>::operator()(const std::span<std::byte> bytes,
                                                                                    cppx::EndiannessPolicy auto endianness_policy)
{
    const std::size_t seed_count =  SeedCount;
    integer_type states[seed_count];
    std::size_t i = 0;
    for (std::size_t end_i = seed_count / 2; i < end_i; ++i)
        states[i] = std::rotl(state_, i * RotationFactor);
    for (std::size_t j = 0; i < seed_count; ++i, ++j)
        states[i] = std::rotr(~state_, j * RotationFactor);

    i = 0;
    const std::span uints = core::as_writable_span<integer_type>(bytes);
    auto xorshift_fn = [&](integer_type& val)
    {
        uint64_t& state = states[i++ % seed_count];
        state = xorshift64(state);
        val = core::htow_when(state, endianness_policy);
    };
    std::for_each(uints.begin(), uints.end(), xorshift_fn);

    state_ = xorshift64(states[0]);

    if (std::size_t remaining_bytes = bytes.size() & (sizeof(integer_type) - 1); remaining_bytes > 0)
    {
        const std::span output_bytes = bytes.last(remaining_bytes);
        integer_type state = core::htow_when(state_, endianness_policy);
        const std::span input_bytes = core::as_writable_bytes(state).first(remaining_bytes);
        std::ranges::copy(input_bytes, output_bytes.begin());
        state_ = xorshift64(state_);
    }

    return bytes;
}

inline std::span<std::byte> generate_random_xorshift64(std::span<std::byte> bytes,
                                                       xorshift64_range_engine<>::integer_type seed,
                                                       cppx::EndiannessPolicy auto endianness_policy = cppx::endianness_specific)
{
    xorshift64_range_engine<> rnrg(seed);
    return rnrg(bytes, endianness_policy);
}

inline std::span<std::byte> generate_random_xorshift64(std::span<std::byte> bytes,
                                                       cppx::EndiannessPolicy auto endianness_policy = cppx::endianness_specific)
{
    return generate_random_xorshift64(bytes, std::random_device{}(), endianness_policy);
}

inline std::span<xorshift64_range_engine<>::integer_type>
generate_random_xorshift64(std::span<xorshift64_range_engine<>::integer_type> ints,
                           xorshift64_range_engine<>::integer_type seed,
                           cppx::EndiannessPolicy auto endianness_policy = cppx::endianness_specific)
{
    generate_random_xorshift64(std::as_writable_bytes(ints), seed, endianness_policy);
    return ints;
}

inline std::span<xorshift64_range_engine<>::integer_type>
generate_random_xorshift64(std::span<xorshift64_range_engine<>::integer_type> ints,
                           cppx::EndiannessPolicy auto endianness_policy = cppx::endianness_specific)
{
    return generate_random_xorshift64(ints, std::random_device{}(), endianness_policy);
}

}
}
