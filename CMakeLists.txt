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
