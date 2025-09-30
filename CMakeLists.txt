cmake_minimum_required(VERSION 3.16)
project(3D_triangles LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(APP_NAME 3D_triangles)

option(ENABLE_SANITIZERS "Enable ASan in Debug builds" ON)

set(ASAN_FLAGS
    -fsanitize=address
    -fsanitize=undefined
    -fsanitize=leak
    -fno-omit-frame-pointer
    -fno-sanitize-recover=all
)

set(WARNING_FLAGS
    -Wall -Wextra -Wpedantic
    -Wshadow -Wconversion -Wsign-conversion
    -Wfloat-equal -Woverloaded-virtual
    -Wnon-virtual-dtor -Wunused -Wuseless-cast
)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(${APP_NAME} ${PROJECT_SOURCES})
target_include_directories(${APP_NAME} PRIVATE include)
target_include_directories(${APP_NAME} PRIVATE ./common)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${APP_NAME} PRIVATE ${WARNING_FLAGS})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND ENABLE_SANITIZERS)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${APP_NAME} PRIVATE ${ASAN_FLAGS})
        target_link_options(${APP_NAME} PRIVATE ${ASAN_FLAGS})
    endif()
endif()

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(geometry_tests tests/geometry_tests.cpp)
target_include_directories(geometry_tests PRIVATE include)
target_link_libraries(geometry_tests PRIVATE GTest::gtest_main)

add_executable(triangles_tests tests/triangles_tests.cpp)
target_include_directories(triangles_tests PRIVATE include)
target_link_libraries(triangles_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(geometry_tests triangles_tests)

