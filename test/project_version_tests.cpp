#include <arba/rand/version.hpp>

#include <gtest/gtest.h>

TEST(project_version_tests, test_version_core)
{
    constexpr unsigned major = 0;
    constexpr unsigned minor = 3;
    constexpr unsigned patch = 0;
    static_assert(arba::rand::version.core() == arba::cppx::numver(major, minor, patch));
}
