find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include(GoogleTest)

aux_source_directory(./src SRC_LIST)

add_executable(intersection ${SRC_LIST})

target_link_libraries(intersection
                      PRIVATE ${GTEST_LIBRARIES}
                      PRIVATE ${CMAKE_THREAD_LIBS_INIT}
                      PRIVATE m)

target_include_directories(intersection
                      PRIVATE ${TEST_INCLUDE_DIR}
                      PRIVATE ${TEST_INCLUDE_DIR}/primitives
                      PRIVATE ${TEST_INCLUDE_DIR}/intersection)                  

gtest_discover_tests(intersection
                    DISCOVERY_MODE PRE_TEST
                    PROPERTIES LABELS "intersection")

