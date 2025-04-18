# Tests CMakeLists.txt
cmake_minimum_required(VERSION 3.15)

# Always use FetchContent for Catch2
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0  # or whichever version you want
)
FetchContent_MakeAvailable(Catch2)

# Create test executable
add_executable(MCAMTests
    # Add test source files here
    TestMain.cpp
    Core/CoreTests.cpp
    # Include the Logger implementation for testing
    ${CMAKE_SOURCE_DIR}/Source/Core/Logger.cpp
    # Add more test files as they are created
)

# Link to the main project and catch2
target_link_libraries(MCAMTests
    PRIVATE
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
        Catch2::Catch2WithMain
)

# Include directories
target_include_directories(MCAMTests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/Source
)

# Add tests to CTest
include(CTest)
include(Catch)
catch_discover_tests(MCAMTests)
