# Find the GTest package (assuming you're using Google Test)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executable
add_executable(multiset_tests multiset_tests.cpp)

add_test(NAME MultiSetTests COMMAND multiset_tests --gtest_output=pretty)

# Link the multiset library and GTest to the test executable
target_link_libraries(multiset_tests PRIVATE multiset ${GTEST_LIBRARIES} pthread)
