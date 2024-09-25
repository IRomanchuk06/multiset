cmake_minimum_required(VERSION 3.10)
project(MultiSet)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set coverage flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")

# Add subdirectory for the source code
add_subdirectory(src)

# Add subdirectory for tests
add_subdirectory(tests)

# Define a custom target for clang-format
add_custom_target(
    clang-format
    COMMAND clang-format -i ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/tests/*.cpp
    COMMENT "Running clang-format on source and test files"
)

# Ensure clang-format runs before building the source and tests
add_custom_target(run_clang_format ALL DEPENDS clang-format)
