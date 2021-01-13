message("-- Gathering Metaphase Core source files")
set(META_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE META_SHARED_SRC ${META_PATH} src/*.cpp inc/meta/*.h)
list(SORT META_SHARED_SRC)

if (LINUX)
    find_package(OpenSSL REQUIRED)
    find_package(ZLIB REQUIRED)
    find_package(CURL REQUIRED)

    set(META_SHARED_LIBS
        ${META_SHARED_LIBS}
        ${OPENSSL_LIBRARIES}
        ${CURL_LIBRARIES}
        ${ZLIB_LIBRARIES})
endif (LINUX)

if (APPLE)
    find_library(SECURITY_LIBRARY Security REQUIRED)
    find_library(SSH2_LIBRARY     ssh2     REQURIED)
    find_library(CRYPTO_LIBRARY   crypto   REQUIRED)
    find_library(ICONV_LIBRARY    iconv    REQUIRED)

    set(META_SHARED_LIBS
        ${META_SHARED_LIBS}
    	${SSH2_LIBRARY}
        ${SECURITY_LIBRARY}
        ${CRYPTO_LIBRARY}
        ${ICONV_LIBRARY}
    )
endif(APPLE)

add_library(metaphase_core STATIC ${META_SHARED_SRC})
include_directories(${META_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/inc/ )

target_include_directories(metaphase_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_link_libraries(metaphase_core juce::juce_audio_utils ${META_SHARED_LIBS})

# this is a required compiler flag for any project that may be build into a
# shared library or DLL.
set_property(TARGET metaphase_core PROPERTY POSITION_INDEPENDENT_CODE ON)


if (BUILD_TESTS)
    add_subdirectory(${META_PATH}/test)
endif()
