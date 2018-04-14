message("-- Building Metaphase Core Test Suite")

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(run_metaphase_core_tests AllTests.cpp)

target_link_libraries(run_metaphase_core_tests
        ${GTEST_LIBRARIES}
        ${JUCE_LIBRARIES}
        ${META_SHARED_LIBS}
        juce
        metaphase_core)

set_target_properties(run_metaphase_core_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/test/shared/metaphase_core)
