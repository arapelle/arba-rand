#include <arba/rand/xorshift.hpp>

#include <gtest/gtest.h>

TEST(xorshift_tests, xorshift32__zero__ok)
{
    const uint32_t value = 0;
    const uint32_t xs_value = arba::rand::xorshift32(value);
    EXPECT_EQ(xs_value, value);
    EXPECT_EQ(xs_value, 0);
}

TEST(xorshift_tests, xorshift32__once__ok)
{
    const uint32_t value = 0x11121314;
    const uint32_t xs_value = arba::rand::xorshift32(value);
    EXPECT_NE(xs_value, value);
    EXPECT_EQ(xs_value, 1'030'221'612);
}

TEST(xorshift_tests, xorshift64__zero__ok)
{
    const uint64_t value = 0;
    const uint64_t xs_value = arba::rand::xorshift64(value);
    EXPECT_EQ(xs_value, value);
    EXPECT_EQ(xs_value, 0);
}

TEST(xorshift_tests, xorshift64__once__ok)
{
    const uint64_t value = 0x11121314;
    const uint64_t xs_value = arba::rand::xorshift64(value);
    EXPECT_NE(xs_value, value);
    EXPECT_EQ(xs_value, 309'672'549'199'016'498ULL);
}
