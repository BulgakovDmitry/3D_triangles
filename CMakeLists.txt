cmake_minimum_required(VERSION 3.16)

project(3D_triangles LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE 
                                                include
                                                common)

enable_testing()
add_subdirectory(tests)
