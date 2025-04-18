# Multi-Channel Audio Monitor (MCAM) Architecture

## Overview

The Multi-Channel Audio Monitor (MCAM) is a cross-platform native application for monitoring up to four selectable audio channels from a set of available input channels (up to 32). The application features VU/PPM meters and Real-Time Analyzers (RTAs) for each selected channel and includes a REST API for external control.

## Core Architecture

<pre>
┌────────────────────────────────────────────────────────────────────────┐
│                             MCAM Application                            │
├────────────┬───────────────┬───────────────────┬───────────────────────┤
│            │               │                   │                       │
│  Audio     │  Processing   │  User Interface   │  REST API             │
│  Pipeline  │  Engine       │  Components       │  Server               │
│            │               │                   │                       │
└────────────┴───────────────┴───────────────────┴───────────────────────┘
</pre>

### Technology Stack

- **Language**: C++
- **Framework**: JUCE Framework (v7+)
- **Build System**: CMake (3.15+)
- **Version Control**: Git
- **License**: GPL (for JUCE compatibility)

## Component Breakdown

### 1. Audio Pipeline

Responsible for interfacing with audio hardware and managing audio data flow.

#### Key Components:
- **AudioDeviceManager**: Manages available audio devices
- **AudioIODeviceCallback**: Handles audio data callbacks
- **ChannelRouterManager**: Routes selected input channels to monitoring slots
- **AudioBufferManager**: Manages thread-safe access to audio data
- **LoopbackManager**: Manages audio loopback capabilities for monitoring output channels

#### Dependencies:
- JUCE Audio Basics, Audio Devices
- System audio APIs (via JUCE abstraction)
- Platform-specific loopback APIs (WASAPI loopback for Windows, virtual audio drivers for macOS, PulseAudio/JACK for Linux)

### 2. Processing Engine

Handles all signal processing and analysis.

#### Key Components:
- **MeterProcessor**: Calculates VU and PPM values
  - **VUMeterCalculator**: Implements RMS with 300ms integration time
  - **PPMMeterCalculator**: Implements peak detection with appropriate attack/release
- **FFTAnalyzer**: Performs spectral analysis for RTA
  - **WindowingFunctions**: Various windowing functions for FFT
  - **FFTProcessor**: Performs FFT calculations
- **ProcessingQueue**: Manages processing order and synchronization

#### Dependencies:
- JUCE DSP module
- JUCE Audio Basics

### 3. User Interface Components

Provides visual representation and user controls.

#### Key Components:
- **MainComponent**: Primary application window and layout
- **ChannelSelectorComponent**: Dropdown for channel selection
- **MeterTypeSelector**: Control for switching meter types
- **MeterComponent**:
  - **VUMeterComponent**: Visual representation of VU meter
  - **PPMMeterComponent**: Visual representation of PPM meter
- **RTAComponent**: Visualization of frequency spectrum
- **SettingsComponent**: Configuration interface for app preferences

#### Dependencies:
- JUCE GUI modules
- Processing Engine (for data)

### 4. REST API Server

Enables external control of the application.

#### Key Components:
- **HTTPServer**: Manages HTTP connections
- **APIEndpointHandler**: Routes and processes API requests
- **ChannelRoutingEndpoints**: Handles GET/PUT requests for channel routing
- **JSONParser**: Processes JSON requests and responses

#### Dependencies:
- JUCE Networking modules
- Audio Pipeline (for controlling channel routing)

## Data Flow

<pre>
┌───────────────┐    ┌───────────────┐    ┌───────────────┐    ┌───────────────┐
│ Audio Device  │───▶│Channel Router │───▶│ Processing    │───▶│   UI          │
│ (up to 32 ch) │    │ (4 slots)     │    │   Engine      │    │ Components    │
└───────────────┘    └───────────────┘    └───────────────┘    └───────────────┘
                             ▲                                        │
                             │                                        │
                             │                                        ▼
                      ┌──────────────┐                        ┌───────────────┐
                      │  REST API    │◀───────────────────────│  User Input   │
                      │   Server     │                        │               │
                      └──────────────┘                        └───────────────┘
</pre>

1. **Audio Input**: System audio devices provide multi-channel audio to the application
2. **Channel Routing**: Selected channels are routed to monitoring slots
3. **Processing**: Audio data is processed for metering and spectral analysis
4. **Visualization**: Processed data is displayed in the UI
5. **Control**: User can control routing via UI or external systems via REST API

## Technical Specifications

### Audio Processing

- **Sample Rate**: Support for standard sample rates (44.1kHz, 48kHz, 96kHz)
- **Bit Depth**: Support for 16-bit, 24-bit, and 32-bit float
- **Latency**: Target < 50ms from input to display
- **CPU Usage**: Target < 10% on modern CPUs
- **Audio Loopback**: Support for monitoring system output channels via platform-specific mechanisms

### Metering Standards

- **VU Meter**:
  - Integration Time: 300ms
  - Reference Level: -20 dBFS = 0 VU
  - Scale: Standard VU scale

- **PPM Meter**:
  - Attack Time: 10ms (or per EBU standard)
  - Release Time: 1.5s (or per EBU standard)
  - Peak Hold: Configurable duration
  - Scale: EBU PPM scale

### RTA Specifications

- **Frequency Range**: 20Hz to 20kHz
- **Resolution**: 1/3 octave default, configurable
- **Display Scale**: Logarithmic frequency, dB amplitude
- **Windowing**: Hann window default, other options available
- **Update Rate**: 30Hz default, configurable

### REST API

- **Protocol**: HTTP
- **Format**: JSON
- **Port**: Configurable, default 8080
- **Authentication**: Optional basic auth (planned for future)

## Directory Structure

```
MCAM/
├── Source/
│   ├── Core/                  # Core application classes
│   ├── Audio/                 # Audio pipeline components
│   │   ├── Devices/           # Audio device management
│   │   └── Routing/           # Channel routing
│   ├── Processing/            # Signal processing
│   │   ├── Metering/          # VU and PPM calculation
│   │   └── Analysis/          # FFT and spectral analysis
│   ├── UI/                    # User interface components
│   │   ├── Meters/            # Meter visualizations
│   │   ├── RTA/               # RTA visualizations
│   │   └── Controls/          # User controls
│   └── Network/               # REST API implementation
├── Resources/                 # Application resources
├── JuceLibraryCode/           # JUCE library code
├── Tests/                     # Unit and integration tests
├── docs/                      # Documentation
└── build/                     # Build output (gitignored)
```

## Threading Model

- **Audio Thread**: High-priority thread for audio processing
- **Message Thread**: JUCE message thread for UI updates
- **Processing Thread**: Medium-priority thread for non-critical processing
- **Network Thread**: Low-priority thread for REST API handling

## Implementation Priorities

1. **Core Audio Pipeline**: Audio device detection, channel routing
2. **Basic UI Framework**: Application window, layout structure
3. **Metering Implementation**: VU and PPM processors and visualization
4. **RTA Implementation**: FFT analysis and visualization
5. **UI Refinement**: Complete user interface and controls
6. **REST API**: External control interface
7. **Cross-Platform Testing**: Ensure compatibility across platforms

## Performance Considerations

- **Lock-Free Design**: Use lock-free algorithms where possible for audio processing
- **Buffer Management**: Careful buffer management to avoid allocations in audio thread
- **Thread Synchronization**: Minimize contention between threads
- **CPU Optimization**: Consider SIMD instructions for DSP operations
- **Memory Usage**: Monitor and optimize memory usage, especially for FFT operations

## Security Considerations

- **Input Validation**: Validate all REST API inputs
- **Error Handling**: Robust error handling for all operations
- **Resource Protection**: Prevent resource exhaustion from malicious requests
- **Cross-Platform Security**: Address platform-specific security concerns

## Future Expansion

- **Additional Meter Types**: Support for additional metering standards
- **Advanced RTA Options**: More configuration options for spectral analysis
- **Presets System**: Save and load configuration presets
- **Remote Monitoring**: Stream audio data over network
- **Direct AES67 Support**: Direct connection to AES67 streams without intermediary software

---

This document serves as the architectural blueprint for the MCAM project and will be updated as the project evolves.

Last Updated: 2023-07-25
