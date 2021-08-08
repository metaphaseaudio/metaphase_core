cmake_minimum_required(VERSION 3.12)
project(META_FILE_VIEWER VERSION 0.0.1)

juce_add_gui_app(MetaFileViewer
    # VERSION ...                       # Set this if the app version is different to the project version
    # ICON_BIG ...                      # ICON_* arguments specify a path to an image file to use as an icon
    # ICON_SMALL ...
    # DOCUMENT_EXTENSIONS ...           # Specify file extensions that should be associated with this app
    COMPANY_NAME "Metaphase"
    PRODUCT_NAME "Audio File Viewer")

FILE(GLOB_RECURSE FV_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)
LIST(SORT FV_SOURCE)

target_sources(MetaFileViewer PRIVATE ${FV_SOURCE})
target_include_directories(MetaFileViewer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc )

target_compile_definitions(MetaFileViewer
    PRIVATE
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:MetaFileViewer,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:MetaFileViewer,JUCE_VERSION>")

target_link_libraries(MetaFileViewer
    PRIVATE
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        metaphase_core)
