
add_cpp_library_basic_tests(${PROJECT_TARGET_NAME} GTest::gtest_main
    SOURCES
        xorshift32_range_engine_tests.cpp
        xorshift64_range_engine_tests.cpp
        xoron64_range_engine_tests.cpp
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_cpp_library_basic_tests(${PROJECT_TARGET_NAME} GTest::gtest_main
        SOURCES
            benchmark_rnrg64s_tests.cpp
    )
endif()
