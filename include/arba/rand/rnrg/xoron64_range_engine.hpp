#pragma once

#include "xorshift_range_engine.hpp"

#include <arba/rand/algorithm/xoron64_fill.hpp>

#include <arba/cppx/policy/endianness_policy.hpp>
#include <arba/cppx/policy/execution_policy.hpp>

#include <span>

inline namespace arba
{
namespace rand
{

template <class InitRnrgT = xorshift64_range_engine<>,
         std::size_t InitRangeSize = 2048,
         uint64_t AlphaXorMask = 0xA6B6C6D6A6B6C6A6ull,
         uint64_t BetaXorMask = 0x6A6B6C6D6A6B6C6Aull>
class xoron64_range_engine : private InitRnrgT
{
public:
    using integer_type = uint64_t;
    using init_range_engine = InitRnrgT;

    static constexpr std::size_t init_range_size = InitRangeSize;
    static constexpr integer_type alpha_xor_mask = AlphaXorMask;
    static constexpr integer_type beta_xor_mask = BetaXorMask;

    inline explicit xoron64_range_engine(integer_type seed_value) : init_range_engine(seed_value)
    {
    }

    xoron64_range_engine() : init_range_engine(std::random_device{}()) {}

    using init_range_engine::seed;
    using init_range_engine::discard;

    std::span<std::byte> operator()(const std::span<std::byte> bytes, cppx::EndiannessPolicy auto endianness_policy,
                                    cppx::ExecutionPolicy auto execution_policy)
    {
        const std::size_t init_rng_size = std::min(init_range_size, (bytes.size() / sizeof(integer_type)) * sizeof(integer_type));
        const std::span<std::byte> init_range = bytes.first(init_rng_size);
        static_cast<init_range_engine&>(*this)(init_range, endianness_policy);
        return xoron64_fill<init_range_size, alpha_xor_mask, beta_xor_mask>(bytes, endianness_policy, execution_policy);
    }

    std::span<std::byte> operator()(const std::span<std::byte> bytes, cppx::EndiannessPolicy auto endianness_policy)
    {
        return (*this)(bytes, endianness_policy, std::execution::seq);
    }

    std::span<integer_type> operator()(const std::span<integer_type> integers, cppx::EndiannessPolicy auto endianness_policy,
                                       cppx::ExecutionPolicy auto execution_policy)
    {
        (*this)(std::as_writable_bytes(integers), endianness_policy, execution_policy);
        return integers;
    }

    std::span<integer_type> operator()(const std::span<integer_type> integers, cppx::EndiannessPolicy auto endianness_policy)
    {
        return (*this)(integers, endianness_policy, std::execution::seq);
    }
};

}
}
