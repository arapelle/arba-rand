#pragma once

#include <random>

inline namespace arba
{
namespace rand
{

namespace private_
{

template <typename Type>
struct integer_type_;

template <typename Type>
    requires std::is_integral_v<Type>
struct integer_type_<Type>
{
    using type = Type;
};

template <typename Type>
    requires std::is_enum_v<Type>
struct integer_type_<Type>
{
    using type = std::underlying_type_t<Type>;
};

template <typename Type>
using integer_type_t_ = typename integer_type_<Type>::type;

template <std::uniform_random_bit_generator RNG, class IntType, IntType... IntParams>
class uniform_engine_impl_;

template <std::uniform_random_bit_generator RNG, class IntType, IntType MinValue, IntType MaxValue>
class uniform_engine_impl_<RNG, IntType, MinValue, MaxValue> : public RNG
{
public:
    using integer_type = integer_type_t_<IntType>;
    using result_type = IntType;
    using distribution_type = std::uniform_int_distribution<integer_type>;

    explicit uniform_engine_impl_(typename RNG::result_type seed) : RNG(seed) {}

    uniform_engine_impl_() : uniform_engine_impl_(typename RNG::result_type(std::random_device{}())) {}

    result_type operator()()
    {
        if constexpr (std::is_same_v<integer_type, typename RNG::result_type>
                      && static_cast<integer_type>(min()) == std::numeric_limits<integer_type>::min()
                      && static_cast<integer_type>(max()) == std::numeric_limits<integer_type>::max())
        {
            return result_type(static_cast<RNG&>(*this)());
        }
        else
        {
            return result_type(distribution()(static_cast<RNG&>(*this)));
        }
    }

    static constexpr result_type min() { return MinValue; }

    static constexpr result_type max() { return MaxValue; }

    static constexpr distribution_type distribution()
    {
        return std::uniform_int_distribution<integer_type>(static_cast<integer_type>(min()),
                                                           static_cast<integer_type>(max()));
    }
};

template <std::uniform_random_bit_generator RNG, class IntType>
class uniform_engine_impl_<RNG, IntType> : public RNG
{
public:
    using integer_type = integer_type_t_<IntType>;
    using result_type = IntType;
    using distribution_type = std::uniform_int_distribution<integer_type>;

    explicit uniform_engine_impl_(typename RNG::result_type seed, distribution_type dist = distribution_type())
        : RNG(seed), dist_(std::move(dist))
    {
    }

    inline uniform_engine_impl_() : uniform_engine_impl_(typename RNG::result_type(std::random_device{}())) {}

    uniform_engine_impl_(typename RNG::result_type seed, result_type min, result_type max)
        : uniform_engine_impl_(seed, distribution_type(min, max))
    {
    }

    explicit uniform_engine_impl_(distribution_type dist) : uniform_engine_impl_(std::random_device{}(), dist) {}

    uniform_engine_impl_(result_type min, result_type max) : uniform_engine_impl_(distribution_type(min, max)) {}

    result_type operator()() { return result_type(dist_(static_cast<RNG&>(*this))); }

    const distribution_type& distribution() const { return dist_; }
    distribution_type& distribution() { return dist_; }

private:
    distribution_type dist_;
};

} // namespace private_

template <std::uniform_random_bit_generator RNG, class IntType, IntType... IntParams>
class uniform_engine : public private_::uniform_engine_impl_<RNG, IntType, IntParams...>
{
    using base_ = private_::uniform_engine_impl_<RNG, IntType, IntParams...>;

public:
    inline uniform_engine() : base_() {}

    inline uniform_engine(typename RNG::result_type seed) : base_(seed) {}

    template <typename... _Up>
    inline explicit uniform_engine(_Up... args) : base_(args...)
    {
    }
};

template <std::uint_fast8_t... IntParams>
using urng_u8 = uniform_engine<std::mt19937, std::uint_fast8_t, IntParams...>;

template <std::int_fast8_t... IntParams>
using urng_i8 = uniform_engine<std::mt19937, std::int_fast8_t, IntParams...>;

template <std::uint_fast16_t... IntParams>
using urng_u16 = uniform_engine<std::mt19937, std::uint_fast16_t, IntParams...>;

template <std::int_fast16_t... IntParams>
using urng_i16 = uniform_engine<std::mt19937, std::int_fast16_t, IntParams...>;

template <std::uint_fast32_t... IntParams>
using urng_u32 = uniform_engine<std::mt19937, std::uint_fast32_t, IntParams...>;

template <std::int_fast32_t... IntParams>
using urng_i32 = uniform_engine<std::mt19937, std::int_fast32_t, IntParams...>;

template <std::uint_fast64_t... IntParams>
using urng_u64 = uniform_engine<std::mt19937_64, std::uint_fast64_t, IntParams...>;

template <std::int_fast64_t... IntParams>
using urng_i64 = uniform_engine<std::mt19937_64, std::int_fast64_t, IntParams...>;

template <std::byte... IntParams>
using urng_byte = uniform_engine<std::mt19937, std::byte, IntParams...>;

} // namespace rand
} // namespace arba
