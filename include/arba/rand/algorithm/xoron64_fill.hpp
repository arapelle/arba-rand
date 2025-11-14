#pragma once

#include <arba/core/bit/htow_when.hpp>
#include <arba/core/container/span.hpp>
#include <arba/cppx/policy/endianness_policy.hpp>
#include <arba/cppx/policy/execution_policy.hpp>

#include <array>
#include <span>
#include <algorithm>

inline namespace arba
{
namespace rand
{

template <std::size_t InitRangeSize = 2048, uint64_t AlphaXorMask = 0xA6B6C6D6A6B6C6A6ull, uint64_t BetaXorMask = 0x6A6B6C6D6A6B6C6Aull>
std::span<std::byte> xoron64_fill(const std::span<std::byte> bytes,
                                  cppx::EndiannessPolicy auto ep,
                                  cppx::ExecutionPolicy auto execution_policy)
{
    using integer_type = uint64_t;

    const std::span<integer_type> ints = core::as_writable_span<integer_type>(bytes);
    std::size_t current_byte_size = std::min(InitRangeSize / sizeof(integer_type), ints.size()) * sizeof(integer_type);
    if (current_byte_size < ints.size_bytes())
    {
        std::size_t offset = 1;
        const std::array primes = { 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 53, 59, 61 };
        for (auto primes_iter = primes.cbegin();
             current_byte_size < ints.size_bytes() && primes_iter != primes.cend();
             ++primes_iter, ++offset)
        {
            if (offset % sizeof(integer_type) == 0)
                ++offset;
            const int rot = *primes_iter;
            const std::size_t max_size_to_copy = (current_byte_size - offset) / sizeof(integer_type);
            std::span output_ints = ints.subspan(current_byte_size / sizeof(integer_type));
            std::size_t size_to_copy = std::min(output_ints.size(), max_size_to_copy);
            std::span input_ints = core::as_writable_span<integer_type>(bytes.subspan(offset)).first(size_to_copy);
            std::transform(execution_policy, input_ints.begin(), input_ints.end(), output_ints.begin(),
                           [=, &ep](integer_type value)
                           {
                               return core::htow_when(std::rotl(AlphaXorMask^core::wtoh_when(value, ep), rot), ep);
                           });
            if (current_byte_size += input_ints.size_bytes(); current_byte_size < ints.size_bytes())
            {
                output_ints = output_ints.subspan(size_to_copy);
                size_to_copy = std::min(output_ints.size(), max_size_to_copy);
                input_ints = core::as_writable_span<integer_type>(bytes.subspan(offset)).first(size_to_copy);
                std::transform(execution_policy, input_ints.begin(), input_ints.end(), output_ints.begin(),
                               [=, &ep](integer_type value)
                               {
                                   return core::htow_when(~std::rotr(BetaXorMask^core::wtoh_when(value, ep), rot), ep);
                               });
                current_byte_size += input_ints.size_bytes();
            }
        }
    }
    if (current_byte_size < bytes.size())
    {
        const std::span output_bytes = bytes.subspan(current_byte_size);
        const std::span input_bytes = bytes.first(output_bytes.size());
        std::ranges::transform(input_bytes, output_bytes.begin(), [=](std::byte value)
                               { return std::byte{std::rotr(static_cast<uint8_t>(value), 2)}; });
    }
    return bytes;
}

}
}
