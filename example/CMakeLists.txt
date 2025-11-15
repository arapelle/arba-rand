include(cmtk/CppLibraryExamples)

add_cpp_library_basic_examples(${PROJECT_TARGET_NAME}
    SOURCES
        rand_i64_example.cpp
        xorshift64_range_engine_example.cpp
        benchmark_rnrg64s.cpp
)
