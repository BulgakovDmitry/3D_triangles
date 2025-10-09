find_package(GTest REQUIRED)
include(GoogleTest)

aux_source_directory(./src SRC_LIST)

add_executable(primitives ${SRC_LIST})

target_link_libraries(primitives
                      PRIVATE ${GTEST_LIBRARIES}
                      PRIVATE ${CMAKE_THREAD_LIBS_INIT}
                      PRIVATE m)

target_include_directories(primitives
                      PRIVATE ${TEST_INCLUDE_DIR}
                      PRIVATE ${TEST_INCLUDE_DIR}/primitives)                  

gtest_discover_tests(primitives
                    DISCOVERY_MODE PRE_TEST
                    PROPERTIES LABELS "primitives")


