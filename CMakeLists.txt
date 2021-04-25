message("-- Gathering Metaphase Core source files")
set(META_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE META_SHARED_SRC ${META_PATH} src/*.cpp inc/meta/*.h)
list(SORT META_SHARED_SRC)


add_library(metaphase_core STATIC)
include_directories(${META_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/inc/ )

target_include_directories(metaphase_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_sources(metaphase_core PRIVATE ${META_SHARED_SRC})
target_link_libraries(metaphase_core juce::juce_audio_utils gtest)
target_compile_definitions(metaphase_core
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
    )

# this is a required compiler flag for any project that may be build into a
# shared library or DLL.
set_property(TARGET metaphase_core PROPERTY POSITION_INDEPENDENT_CODE ON)


if (BUILD_TESTS)
    add_subdirectory(${META_PATH}/test)
endif()
