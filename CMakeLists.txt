cmake_minimum_required(VERSION 3.15)
project(MCAM VERSION 0.1.0)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Export compile commands for language servers
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include JUCE directly as a subdirectory
add_subdirectory(JUCE)

# Define JUCE modules used by the project
set(JUCE_MODULES
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)

# Check if icon file exists
set(ICON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Resources/icon.png")
if(EXISTS "${ICON_PATH}")
    set(APP_ICON_BIG "ICON_BIG" "${ICON_PATH}")
else()
    set(APP_ICON_BIG "")
    message(STATUS "Icon file not found at ${ICON_PATH}. Using default icon.")
endif()

# Add the executable
juce_add_gui_app(MCAM
    PRODUCT_NAME "Multi-Channel Audio Monitor"
    COMPANY_NAME "MCAM Team"
    VERSION "0.1.0"
    ${APP_ICON_BIG}
    BUNDLE_ID "com.mcam.app"
)

# Add JUCE modules
target_compile_definitions(MCAM
    PRIVATE
        JUCE_WEB_BROWSER=0  # If you don't need web browser
        JUCE_USE_CURL=0     # If you don't need CURL
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:MCAM,PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:MCAM,VERSION>"
        # Add any other definitions
)

# Add source files
target_sources(MCAM
    PRIVATE
        # Main application files
        Source/Core/Main.cpp
        Source/Core/MainComponent.cpp
        Source/Core/Logger.cpp
        Source/JuceHeader.h

        # Audio Pipeline
        Source/Audio/AudioCallback.cpp
        Source/Audio/AudioEngine.cpp
        Source/Audio/Devices/AudioDeviceManager.cpp
        Source/Audio/Processing/BufferProcessor.cpp

        # UI Components
        Source/UI/Meters/MeterComponent.cpp
        Source/UI/RTA/RTAComponent.cpp
        Source/UI/MonitoringSlotComponent.cpp
        # Add other source files as they are created
)

# Link JUCE modules
target_link_libraries(MCAM
    PRIVATE
        # JUCE modules
        ${JUCE_MODULES}

    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Include directories
target_include_directories(MCAM
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Platform-specific settings
if(WIN32)
    target_compile_definitions(MCAM PRIVATE _USE_MATH_DEFINES=1)
elseif(APPLE)
    set_target_properties(MCAM PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "${PRODUCT_NAME}"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.mcam.app"
        MACOSX_BUNDLE_ICON_FILE "icon.icns"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION}"
    )
endif()

# Create logs directory
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/logs)

# Add tests (only if BUILD_TESTING is ON)
option(BUILD_TESTING "Build the testing tree" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(Tests)
endif()

# Install targets
install(TARGETS MCAM
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION bin
    LIBRARY DESTINATION lib
)

# Install logs directory
install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/logs
    DESTINATION bin
)
