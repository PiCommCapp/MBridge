# Multi-Channel Audio Monitor (MCAM) Technical Documentation

This document serves as the authoritative source for technical decisions, implementations, and considerations for the MCAM project. It will be updated throughout development to reflect the current state of the project.

## Table of Contents
- [Multi-Channel Audio Monitor (MCAM) Technical Documentation](#multi-channel-audio-monitor-mcam-technical-documentation)
  - [Table of Contents](#table-of-contents)
  - [Technology Stack](#technology-stack)
    - [Core Technologies](#core-technologies)
    - [Platform-Specific Technologies](#platform-specific-technologies)
  - [Implementation Details](#implementation-details)
    - [JUCE Framework Location](#juce-framework-location)
    - [Audio Processing Pipeline](#audio-processing-pipeline)
    - [Threading Model](#threading-model)
    - [Memory Management](#memory-management)
  - [Technical Decisions](#technical-decisions)
  - [Dependencies](#dependencies)
  - [Performance Optimizations](#performance-optimizations)
  - [Known Issues](#known-issues)
  - [Document History](#document-history)
- [Technical Decisions and Implementation Details](#technical-decisions-and-implementation-details)
  - [Logger System](#logger-system)
    - [Design](#design)
    - [Key Features](#key-features)
    - [Implementation Details](#implementation-details-1)
  - [Application Properties Management](#application-properties-management)
    - [Design](#design-1)
    - [Key Features](#key-features-1)
    - [Implementation Details](#implementation-details-2)
  - [Build System](#build-system)
    - [Design](#design-2)
    - [Key Features](#key-features-2)
    - [Implementation Details](#implementation-details-3)
  - [UI Management](#ui-management)
    - [Design](#design-3)
    - [Key Features](#key-features-3)
    - [Implementation Details](#implementation-details-4)
  - [Audio Loopback Implementation](#audio-loopback-implementation)
    - [Design](#design-4)
    - [Key Features](#key-features-4)
    - [Implementation Details](#implementation-details-5)

## Technology Stack

### Core Technologies
- **Language**: C++ (C++17 standard)
- **Framework**: JUCE Framework v7
- **Build System**: CMake 3.15+
- **Version Control**: Git

### Platform-Specific Technologies
- **Windows**: WASAPI and ASIO (via JUCE)
- **macOS**: CoreAudio (via JUCE)
- **Linux**: ALSA and JACK (via JUCE)

## Implementation Details

### JUCE Framework Location
The project uses JUCE Framework installed at `~/JUCE`. This location is referenced in the CMake configuration and needs to be accessible during the build process. If you need to modify this location, update the following:

1. Set the `JUCE_DIR` environment variable to your JUCE installation path
2. Update CMake configuration commands with the appropriate `-DJUCE_DIR` parameter

### Audio Processing Pipeline

The audio processing pipeline follows this sequence:
1. Audio device provides input through JUCE's AudioDeviceManager
2. AudioIODeviceCallback processes incoming audio data
3. Channel routing maps input channels to monitoring slots
4. Audio data is processed by MeterProcessor and FFTAnalyzer components
5. Processed data is sent to UI components for visualization

### Threading Model

The application uses four primary threads:
- **Audio Thread**: High-priority for audio processing
- **Message Thread**: JUCE message thread for UI updates
- **Processing Thread**: Medium-priority for non-critical processing
- **Network Thread**: Low-priority for REST API handling

Thread synchronization is handled via lock-free data structures where possible, particularly in the audio processing path.

### Memory Management

- Audio buffer management uses a fixed-size pre-allocated buffer pool to avoid allocations in the audio thread
- DSP operations use JUCE's dsp module which is optimized for real-time audio processing
- FFT operations use pre-allocated buffers sized to the maximum expected FFT size

## Technical Decisions

| Decision | Rationale | Alternatives Considered | Date |
|----------|-----------|-------------------------|------|
| JUCE Framework | Cross-platform audio and GUI capabilities in a single framework | Qt+PortAudio, SDL+RTAudio | 2023-07-25 |
| CMake Build System | Better cross-platform support than JUCE's Projucer | JUCE's Projucer, Premake | 2023-07-25 |
| C++17 Standard | Balance of modern features and compiler support | C++14, C++20 | 2023-07-25 |
| Lock-free Design | Essential for audio thread performance | Mutex-based synchronization | 2023-07-25 |
| Platform-specific Audio Loopback | Each platform requires different approach for output monitoring | Universal virtual audio cable | 2023-07-26 |

## Dependencies

| Dependency | Version | Purpose | License |
|------------|---------|---------|---------|
| JUCE | 7.0+ | Audio and GUI framework | GPL/Commercial |
| CMake | 3.15+ | Build system | BSD 3-Clause |

## Performance Optimizations

- FFT calculations use JUCE's optimized FFT implementation
- Audio thread avoids memory allocations and system calls
- Meter ballistics calculations use efficient approximations where appropriate
- UI updates are throttled to balance responsiveness with CPU usage
- Windowing functions are pre-calculated and stored

## Known Issues

| Issue | Description | Status | Workaround | Date Identified |
|-------|-------------|--------|------------|-----------------|
| *None documented yet* | | | | |

---

## Document History

| Date | Changes | Author |
|------|---------|--------|
| 2023-07-25 | Initial document creation | MCAM Team |

---

*This document will be updated as development progresses.*

# Technical Decisions and Implementation Details

## Logger System

### Design
We've implemented a flexible and thread-safe logging system for the application. The logger is designed as a singleton to provide a consistent logging interface throughout the application.

### Key Features
- **Severity Levels**: Supports multiple logging levels (Debug, Info, Warning, Error, Critical)
- **Thread Safety**: Uses JUCE's CriticalSection for thread-safe logging
- **File & Console Output**: Logs to both the console and a file
- **Automatic Directory Creation**: Creates the logs directory if it doesn't exist
- **Timestamp**: Includes timestamps on all log entries

### Implementation Details
- Uses JUCE's `FileOutputStream` for log file writing
- Configuration options allow setting minimum log level
- Convenient macros (LOG_INFO, LOG_ERROR, etc.) simplify usage
- Log rotation and size management will be added in future updates

## Application Properties Management

### Design
We use JUCE's ApplicationProperties system to handle persistent application settings across launches. This provides a platform-independent way to store user preferences.

### Key Features
- **Window State Persistence**: Saves and restores window position and size
- **Platform-Specific Storage**: Uses appropriate locations for settings on each platform
- **XML Format**: Settings stored in easily readable XML format
- **Automatic Saving**: Saves settings when application closes

### Implementation Details
- Settings file is stored in the user's application data directory
- Default settings are applied if no settings file exists
- Each component can have its own section in the properties file
- Settings are only saved when changed to avoid unnecessary file writes

## Build System

### Design
We've created a robust build system using CMake for cross-platform compatibility. A build script simplifies the process for developers.

### Key Features
- **Cross-Platform**: Works on Windows, macOS, and Linux
- **Automatic JUCE Detection**: Build script automatically detects JUCE installation
- **Test Integration**: Includes test framework configuration with Catch2
- **Directory Setup**: Creates necessary directories like logs during build

### Implementation Details
- CMake minimum version 3.15 required for JUCE support
- C++17 used for modern language features
- Output directories organized for clarity
- Platform-specific options handled automatically

## UI Management

### Design
The user interface follows JUCE component patterns with careful management of window properties and layout.

### Key Features
- **Window Size/Position Memory**: Remembers window state between sessions
- **Resizable Interface**: Allows user to resize the application window with constraints
- **Component Organization**: Organized UI components for future expansion
- **Consistent Styling**: Foundation for consistent application appearance

### Implementation Details
- Main window is a DocumentWindow with native title bar
- MainComponent is the primary content component
- UI layout divided into sections for future components
- Uses JUCE's LookAndFeel system for styling

## Audio Loopback Implementation

### Design
The application implements platform-specific solutions for audio loopback functionality, allowing users to monitor computer output audio in addition to input sources.

### Key Features
- **Windows Implementation**: Uses WASAPI loopback capture for monitoring output devices
- **macOS Implementation**: Supports virtual audio drivers (BlackHole or similar) for routing output to inputs
- **Linux Implementation**: Leverages PulseAudio, JACK, or PipeWire loopback capabilities
- **Seamless Integration**: Output devices appear alongside input devices in channel selection UI

### Implementation Details
- Extends JUCE's AudioDeviceManager with platform-specific loopback detection
- Implements separate device types for inputs and loopback sources
- Handles potential sample rate and buffer size differences between input and output devices
- Provides clear labeling in UI to distinguish between input sources and loopback sources
