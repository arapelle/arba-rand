include(cmtk/CppLibraryTests)
include(GoogleTest)

find_package(GTest 1.14 CONFIG REQUIRED)

add_cpp_library_basic_tests(${PROJECT_TARGET_NAME} GTest::gtest_main
    SOURCES
        project_version_tests.cpp
        rand_tests.cpp
        xorshift_tests.cpp
        bit_balanced_uints_tests.cpp
)

add_subdirectory(rng)
add_subdirectory(rnrg)
