# Technical Context: Multi-Channel Audio Monitor

## Technology Stack

### Core Technologies
- **Programming Language**: C++
- **Audio Framework**: JUCE Framework v7+
- **Build System**: TBD (likely CMake for cross-platform)
- **Version Control**: Git

### JUCE Components
- **Audio I/O**: AudioDeviceManager, AudioIODeviceCallback
- **GUI**: JUCE component-based GUI system
- **DSP**: JUCE dsp module (FFT, windowing, RMS/peak calculation)
- **Networking**: ServerSocket, Socket, URL classes for REST API

## Technical Standards

### Audio Standards
- **VU Meter**: Standard ballistics (300ms integration time)
- **PPM Meter**: EBU standard (Fast attack ~10ms, slow release ~1.5s)
- **Calibration**: -20 dBFS = 0 VU reference
- **RTA**: Logarithmic frequency scale, multiple windowing function options

### API Standards
- **Format**: JSON
- **Protocol**: HTTP REST
- **Endpoints**: Structured as /api/v1/[resource]
- **Authentication**: None planned for initial version

## Cross-Platform Considerations
- **Windows**: Support for WASAPI, ASIO audio drivers
- **macOS**: Support for CoreAudio
- **Linux**: Support for ALSA, JACK audio systems

## Architecture Patterns
- **Audio Thread**: Separate high-priority thread for audio processing
- **UI Thread**: Main thread for user interface
- **Network Thread**: Separate thread for API handling
- **Component-Based**: Modular design for meters and RTAs

## Performance Considerations
- **DSP Optimization**: Efficient FFT processing for RTAs
- **Low Latency**: Minimal processing delay for live monitoring
- **UI Responsiveness**: Separate audio and UI rendering to maintain UI smoothness 