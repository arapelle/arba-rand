include(cmtk/CppLibraryExamples)

add_cpp_library_basic_examples(${PROJECT_TARGET_NAME}
    SOURCES
        example.cpp
        xorshift64_range_engine_example.cpp
)
