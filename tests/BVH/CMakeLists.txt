find_package(GTest REQUIRED)
include(GoogleTest)

aux_source_directory(./src SRC_LIST)

add_executable(BVH ${SRC_LIST})

target_link_libraries(BVH
                      PRIVATE ${GTEST_LIBRARIES}
                      PRIVATE ${CMAKE_THREAD_LIBS_INIT}
                      PRIVATE m)

target_include_directories(BVH
                      PRIVATE ${TEST_INCLUDE_DIR}
                      PRIVATE ${TEST_INCLUDE_DIR}/primitives
                      PRIVATE ${TEST_INCLUDE_DIR}/BVH)                  

gtest_discover_tests(BVH
                    DISCOVERY_MODE PRE_TEST
                    PROPERTIES LABELS "BVH")


