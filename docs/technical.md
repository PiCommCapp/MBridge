# Multi-Channel Audio Monitor (MCAM) Technical Documentation

This document serves as the authoritative source for technical decisions, implementations, and considerations for the MCAM project. It will be updated throughout development to reflect the current state of the project.

## Table of Contents
- [Multi-Channel Audio Monitor (MCAM) Technical Documentation](#multi-channel-audio-monitor-mcam-technical-documentation)
  - [Table of Contents](#table-of-contents)
  - [Technology Stack](#technology-stack)
    - [Core Technologies](#core-technologies)
    - [Platform-Specific Technologies](#platform-specific-technologies)
  - [Implementation Details](#implementation-details)
    - [Audio Processing Pipeline](#audio-processing-pipeline)
    - [Threading Model](#threading-model)
    - [Memory Management](#memory-management)
  - [Technical Decisions](#technical-decisions)
  - [Dependencies](#dependencies)
  - [Performance Optimizations](#performance-optimizations)
  - [Known Issues](#known-issues)
  - [Document History](#document-history)

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