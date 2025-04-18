# Multi-Channel Audio Monitor (MCAM)

A cross-platform native application for monitoring up to four selectable audio channels with VU/PPM meters and Real-Time Analyzers (RTAs).

![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)

## Overview

MCAM is designed for audio professionals who need to monitor multiple audio channels simultaneously. The application accepts input from standard system audio interfaces and allows users to independently select up to four channels for monitoring. Each channel displays a configurable meter (VU or PPM) and a real-time frequency analyzer.

## Features

- **Multi-Channel Monitoring**: Monitor up to 4 channels simultaneously from a larger set of available inputs (up to 32 channels)
- **Flexible Metering**: Choose between VU and PPM meter types with standard ballistics
- **Real-Time Analysis**: Frequency spectrum display for each monitored channel
- **REST API**: Control channel selection via an HTTP API for integration with external control systems
- **Cross-Platform**: Runs on Windows, macOS, and Linux

## Requirements

- C++ compiler with C++17 support
- JUCE Framework v7.0 or newer
- CMake 3.15 or newer
- Platform-specific audio drivers:
  - Windows: WASAPI or ASIO
  - macOS: CoreAudio
  - Linux: ALSA or JACK

## Building from Source

### Prerequisites

1. Install CMake (3.15+)
2. Install JUCE Framework v7 or newer
3. Install a compatible C++ compiler:
   - Windows: Visual Studio 2019+ or MinGW-w64
   - macOS: Xcode 11+ (with Command Line Tools)
   - Linux: GCC 9+ or Clang 10+

### Build Steps

```bash
# Clone the repository
git clone https://github.com/yourusername/mcam.git
cd mcam

# Create a build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .
```

## Usage

1. Launch the application
2. Select audio input device from the settings menu
3. Use the dropdown menus to select which channels to monitor in each slot
4. Choose between VU or PPM meter types for each slot as needed

### REST API

MCAM provides a REST API for external control:

- GET `/api/v1/channels` - Returns current channel routing
- PUT `/api/v1/channels` - Set channel routing

Example to change channel routing:
```bash
curl -X PUT -H "Content-Type: application/json" -d '{"slot1": 5, "slot3": 10}' http://localhost:8080/api/v1/channels
```

## Documentation

Comprehensive documentation is available in the `docs` directory:
- [Project Architecture](docs/architecture.md)
- [Technical Documentation](docs/technical.md)
- [Development Guide](docs/development.md)
- [Task List](docs/tasks.md)
- [Project Status](docs/status.md)

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the GNU General Public License v3.0 - see the LICENSE file for details.

## Acknowledgments

- JUCE Framework for audio and GUI capabilities
- The open-source audio community for standards and inspiration