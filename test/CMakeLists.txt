message("-- Building Metaphase Core Test Suite")

file(GLOB_RECURSE METACORE_TEST_SHARED_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h
                                              ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
list(SORT METACORE_TEST_SHARED_SOURCE)

add_executable(run_metaphase_core_tests ${METACORE_TEST_SHARED_SOURCE} AllTests.cpp)
target_include_directories(run_metaphase_core_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tests)

target_link_libraries(run_metaphase_core_tests
        ${GTEST_LIBRARIES}
        ${JUCE_LIBRARIES}
        ${META_SHARED_LIBS}
        juce
        metaphase_core)

set_target_properties(run_metaphase_core_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/test/shared/metaphase_core)
