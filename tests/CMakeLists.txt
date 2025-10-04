cmake_minimum_required(VERSION 3.16)
project(3D_triangles LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_library(includes INTERFACE)
target_include_directories(includes INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/common
)

add_executable(geometry geometry_tests.cpp)
target_link_libraries(geometry PRIVATE includes)
target_link_libraries(geometry PRIVATE GTest::gtest_main)

add_executable(triangles triangles_tests.cpp)
target_link_libraries(triangles PRIVATE includes)
target_link_libraries(triangles PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(geometry triangles)
