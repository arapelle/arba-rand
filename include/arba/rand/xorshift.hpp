#pragma once

#include <cstdint>
#include <cstdlib>

inline namespace arba
{
namespace rand
{

inline uint32_t xorshift32(uint32_t x)
{
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

inline uint64_t xorshift64(uint64_t x)
{
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return x;
}

}
}
