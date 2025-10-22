cmake_minimum_required(VERSION 3.14)
project(HW3D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

find_package(GTest REQUIRED)

# Main executable
add_executable(triangle_main src/main.cpp src/triangle.cpp)

# Test executable  
add_executable(triangle_tests test/triangle_test.cpp src/triangle.cpp)
target_link_libraries(triangle_tests GTest::gtest GTest::gtest_main)

# Enable testing
include(GoogleTest)
gtest_discover_tests(triangle_tests)
