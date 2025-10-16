#pragma once

#include "xorshift.hpp"

#include <random>

inline namespace arba
{
namespace rand
{

class xorshift32_engine
{
public:
    using result_type = uint32_t;

    inline explicit xorshift32_engine(result_type seed) : state_(seed)
    {
        if (state_ == 0) [[unlikely]]
            --state_;
    }

    xorshift32_engine() : xorshift32_engine(result_type(std::random_device{}())) {}

    inline result_type operator()()
    {
        return (state_ = xorshift32(state_));
    }

    static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }

    static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }

    [[nodiscard]] inline result_type seed() const { return state_; }

    inline void seed(result_type value)
    {
        state_ = xorshift32_engine(value).seed();
    }

    inline void discard(unsigned long long times)
    {
        for (; times > 0; --times)
            state_ = xorshift32(state_);
    }

private:
    result_type state_;
};

class xorshift64_engine
{
public:
    using result_type = uint64_t;

    inline explicit xorshift64_engine(result_type seed) : state_(seed)
    {
        if (state_ == 0) [[unlikely]]
            --state_;
    }

    xorshift64_engine() : xorshift64_engine(std::random_device{}()) {}

    inline result_type operator()()
    {
        return (state_ = xorshift64(state_));
    }

    static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }

    static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }

    [[nodiscard]] inline result_type seed() const { return state_; }

    inline void seed(result_type value)
    {
        state_ = xorshift64_engine(value).seed();
    }

    inline void discard(unsigned long long times)
    {
        for (; times > 0; --times)
            state_ = xorshift64(state_);
    }

private:
    result_type state_;
};

}
}
