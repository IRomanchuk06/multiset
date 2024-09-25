# Create a library or executable from the source files
add_library(multiset
    multiset.cpp
)

# Specify the include directory
target_include_directories(multiset PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
