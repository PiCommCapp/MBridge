# MCAM Development Guide

This document provides instructions for setting up a development environment for the Multi-Channel Audio Monitor (MCAM) project on different platforms.

## Prerequisites

The MCAM project requires the following dependencies:

- C++ compiler with C++17 support
- JUCE Framework v7.0 or newer
- CMake 3.15 or newer
- Git

## Windows Setup

### 1. Install a C++ Compiler

#### Option A: Visual Studio (Recommended)
1. Download and install [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/)
2. During installation, select the "Desktop development with C++" workload
3. Make sure the following components are included:
   - MSVC C++ build tools
   - Windows 10/11 SDK
   - C++ CMake tools for Windows

#### Option B: MinGW-w64
1. Download the [MinGW-w64 installer](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/)
2. Install with settings:
   - Version: 8.1.0 or higher
   - Architecture: x86_64
   - Threads: posix
   - Exception: seh
3. Add MinGW's `bin` directory to your system PATH

### 2. Install CMake
1. Download the [CMake installer](https://cmake.org/download/)
2. During installation, choose to add CMake to the system PATH

### 3. Install Git
1. Download and install [Git for Windows](https://git-scm.com/download/win)
2. Use the default settings or customize as needed

### 4. Install JUCE Framework
1. Clone the JUCE repository:
   ```bash
   git clone https://github.com/juce-framework/JUCE.git
   cd JUCE
   git checkout master  # or a specific tag for version 7
   ```
2. Open the `JUCE` directory in Visual Studio or build using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build . --config Release
   ```
3. Make note of the JUCE path as you'll need it later

### 5. Audio Drivers
- For WASAPI: Included with Windows, no additional installation needed
- For ASIO: Download and install [ASIO drivers](https://www.steinberg.net/developers/) if needed for your audio interface

## macOS Setup

### 1. Install Xcode and Command Line Tools
1. Install Xcode from the [App Store](https://apps.apple.com/us/app/xcode/id497799835)
2. Install the Command Line Tools:
   ```bash
   xcode-select --install
   ```

### 2. Install Homebrew
1. Install [Homebrew](https://brew.sh/):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

### 3. Install CMake and Git
```bash
brew install cmake git
```

### 4. Install JUCE Framework
1. Clone the JUCE repository:
   ```bash
   git clone https://github.com/juce-framework/JUCE.git
   cd JUCE
   git checkout master  # or a specific tag for version 7
   ```
2. Build JUCE using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```
3. Make note of the JUCE path as you'll need it later

### 5. Audio Drivers
- CoreAudio is included with macOS, no additional installation needed

## Linux Setup

### 1. Install Essential Build Tools and Dependencies

#### For Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install build-essential git cmake pkg-config libasound2-dev libjack-jackd2-dev \
                     ladspa-sdk libcurl4-openssl-dev libfreetype6-dev libx11-dev libxcomposite-dev \
                     libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
                     libwebkit2gtk-4.0-dev libglu1-mesa-dev mesa-common-dev
```

#### For Fedora:
```bash
sudo dnf install gcc-c++ git cmake alsa-lib-devel jack-audio-connection-kit-devel \
                 freetype-devel libX11-devel libXcomposite-devel libXcursor-devel \
                 libXext-devel libXinerama-devel libXrandr-devel libXrender-devel \
                 webkit2gtk3-devel mesa-libGLU-devel
```

#### For Arch Linux:
```bash
sudo pacman -S gcc git cmake pkgconfig alsa-lib jack freetype2 libx11 libxcomposite \
               libxcursor libxext libxinerama libxrandr libxrender webkit2gtk mesa glu
```

### 2. Install JUCE Framework
1. Clone the JUCE repository:
   ```bash
   git clone https://github.com/juce-framework/JUCE.git
   cd JUCE
   git checkout master  # or a specific tag for version 7
   ```
2. Build JUCE using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```
3. Make note of the JUCE path as you'll need it later

### 3. Audio Drivers
- ALSA is typically included with most Linux distributions
- For JACK:
  ```bash
  # Ubuntu/Debian
  sudo apt-get install jackd2 qjackctl
  
  # Fedora
  sudo dnf install jack-audio-connection-kit qjackctl
  
  # Arch Linux
  sudo pacman -S jack2 qjackctl
  ```

## Building MCAM

### 1. Clone the Repository
```bash
git clone https://github.com/yourusername/mcam.git
cd mcam
```

### 2. Configure the Build
```bash
mkdir build
cd build
```

#### Windows (Visual Studio)
```bash
cmake -G "Visual Studio 16 2019" -A x64 -DJUCE_DIR=/path/to/JUCE ..
```

#### Windows (MinGW)
```bash
cmake -G "MinGW Makefiles" -DJUCE_DIR=/path/to/JUCE ..
```

#### macOS/Linux
```bash
cmake -DJUCE_DIR=/path/to/JUCE ..
```

### 3. Build the Project
```bash
cmake --build .
```

### 4. Run the Application
The built application will be in the `build` directory, typically in a subdirectory corresponding to the build configuration.

## Development Workflow

### Code Formatting
We use clang-format for code formatting:

```bash
# Install clang-format
# Ubuntu/Debian
sudo apt-get install clang-format

# macOS
brew install clang-format

# Windows
# Download from LLVM releases or install via Visual Studio
```

Format your code before committing:
```bash
clang-format -i Source/*.cpp Source/*.h
```

### Running Tests
```bash
cd build
ctest
```

### Creating Builds for Distribution
Follow platform-specific instructions:

#### Windows
```bash
cmake --build . --config Release
```

#### macOS
```bash
cmake --build . --config Release
# To create a macOS app bundle:
cmake --build . --target package
```

#### Linux
```bash
cmake --build . --config Release
# To create distribution packages:
cmake --build . --target package
```

## Troubleshooting

### Windows
- **CMake can't find JUCE**: Make sure you've specified the correct JUCE_DIR path
- **Build errors with Visual Studio**: Ensure you have the correct Windows SDK version installed
- **ASIO driver issues**: Check that you're using the latest ASIO SDK and drivers

### macOS
- **Xcode command line tools not found**: Run `xcode-select --install` again
- **Permission issues**: Ensure you have write permissions to the build directory

### Linux
- **Missing dependencies**: Install additional dev packages as needed
- **JACK connection issues**: Make sure JACK server is running with `qjackctl`
- **Build fails with missing X11 headers**: Install missing X11 development packages

## Additional Resources

- [JUCE Framework Documentation](https://juce.com/learn/documentation)
- [CMake Documentation](https://cmake.org/documentation/)
- [JUCE Forum](https://forum.juce.com/)

---

Last Updated: 2023-07-25 