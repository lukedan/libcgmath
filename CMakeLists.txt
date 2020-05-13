cmake_minimum_required(VERSION 3.9)
project(cgmath)


add_library(cgmath INTERFACE)

target_include_directories(cgmath INTERFACE include/)
target_compile_features(cgmath INTERFACE cxx_std_17)


include(CTest)
include(GoogleTest)
find_package(GTest REQUIRED)
add_executable(unit_test)

target_sources(unit_test PRIVATE test/unit.cpp)
target_link_libraries(unit_test cgmath GTest::GTest GTest::Main)
gtest_discover_tests(unit_test)
