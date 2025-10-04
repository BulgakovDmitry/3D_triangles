cmake_minimum_required(VERSION 3.16)
project(3D_triangles LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

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
    -Wnon-virtual-dtor -Wuseless-cast
)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/BVH.cpp"
    "${CMAKE_SOURCE_DIR}/src/driver.cpp"
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
)

add_library(includes INTERFACE)
target_include_directories(includes INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/common
)

add_executable(${APP_NAME} ${PROJECT_SOURCES})
target_link_libraries(${APP_NAME} PRIVATE includes)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${APP_NAME} PRIVATE ${WARNING_FLAGS})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND ENABLE_SANITIZERS)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${APP_NAME} PRIVATE ${ASAN_FLAGS})
        target_link_options(${APP_NAME} PRIVATE ${ASAN_FLAGS})
    endif()
endif()

add_subdirectory(tests)

