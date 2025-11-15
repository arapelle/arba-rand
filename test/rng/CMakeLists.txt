add_cpp_library_basic_tests(${PROJECT_TARGET_NAME} GTest::gtest_main
    SOURCES
        urng_tests.cpp
        xorshift32_engine_tests.cpp
        xorshift64_engine_tests.cpp
)
