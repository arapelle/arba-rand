#include <arba/rand/bit_balanced_uints.hpp>

#include <gtest/gtest.h>

#include <bit>

TEST(bit_balanced_uints_tests, bit_balanced_uint32s__enum_size__ok)
{
    ASSERT_EQ(rand::bit_balanced_uint32s::all().size(), 20);
    ASSERT_EQ(rand::bit_balanced_uint32s::all_names().size(), 20);
}

TEST(bit_balanced_uints_tests, bit_balanced_uint32s__popcount__between_15_and_17)
{
    for (rand::bit_balanced_uint32 seed : rand::bit_balanced_uint32s::all())
    {
        const uint32_t value = seed.value();
        const unsigned nb_ones = std::popcount(value);
        ASSERT_TRUE(nb_ones >= 15 && nb_ones <= 17);
    }
}

TEST(bit_balanced_uints_tests, bit_balanced_uint64s__enum_size__ok)
{
    ASSERT_EQ(rand::bit_balanced_uint64s::all().size(), 20);
    ASSERT_EQ(rand::bit_balanced_uint64s::all_names().size(), 20);
}

TEST(bit_balanced_uints_tests, bit_balanced_uint64s__popcount__between_31_and_33)
{
    for (rand::bit_balanced_uint64 seed : rand::bit_balanced_uint64s::all())
    {
        const uint64_t value = seed.value();
        const unsigned nb_ones = std::popcount(value);
        ASSERT_TRUE(nb_ones >= 31 && nb_ones <= 33);
    }
}
