cmake_minimum_required(VERSION 3.16)

project(3D_triangles LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_compile_definitions(PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

target_include_directories(${PROJECT_NAME} PRIVATE include)

add_custom_target(end_to_end
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/end_to_end/end_to_end.sh)

add_custom_target(unit
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build/tests/primitives/primitives
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build/tests/intersection/intersection
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build/tests/BVH/BVH
)

enable_testing()
add_subdirectory(tests)

option(GRAPHICS "Switch on graph mode" OFF)
message(STATUS "GRAPHICS enabled: ${GRAPHICS}")

if(GRAPHICS)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)

    find_library(GLAD_LIBRARY glad)
    find_path(GLAD_INCLUDE_DIR NAMES glad/glad.h glad/gl.h)

    add_executable(Graphics src/main_graphics.cpp)

    target_include_directories(Graphics PRIVATE include
                                                ${GLAD_INCLUDE_DIR})

    target_link_libraries(Graphics OpenGL::GL glfw ${GLAD_LIBRARY})
endif()
