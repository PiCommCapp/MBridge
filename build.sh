#!/bin/bash
# Build script for MCAM (Multi-Channel Audio Monitor)

# Function to handle errors
error_exit() {
  echo "Error: $1" >&2
  exit 1
}

# Check if JUCE_DIR is set, otherwise try to find it
if [ -z "$JUCE_DIR" ]; then
  echo "JUCE_DIR environment variable is not set."

  # Try to find JUCE in common locations
  potential_locations=(
    "$HOME/JUCE"
    "/usr/local/JUCE"
    "/opt/JUCE"
    "$HOME/Development/JUCE"
  )

  for location in "${potential_locations[@]}"; do
    if [ -d "$location" ]; then
      export JUCE_DIR="$location"
      echo "Found JUCE at $JUCE_DIR"
      break
    fi
  done

  if [ -z "$JUCE_DIR" ]; then
    echo "Please set JUCE_DIR environment variable to your JUCE installation directory."
    echo "For example: export JUCE_DIR=$HOME/JUCE"
    exit 1
  fi
fi

# Create build directory if it doesn't exist
mkdir -p build || error_exit "Failed to create build directory"

# Move to build directory
cd build || error_exit "Failed to change to build directory"

# Run CMake
echo "Configuring with CMake..."
cmake .. || error_exit "CMake configuration failed"

# Build with make
echo "Building..."
make -j4 || error_exit "Build failed"

echo "Build completed successfully!"

# Determine the executable path based on OS
if [ "$(uname)" == "Darwin" ]; then
  # macOS: executable is inside .app bundle
  EXECUTABLE="MCAM_artefacts/Multi-Channel Audio Monitor.app/Contents/MacOS/Multi-Channel Audio Monitor"
else
  # Linux/Windows
  EXECUTABLE="bin/MCAM"
fi

echo "Executable is located at: $PWD/$EXECUTABLE"

# Check if we should run the tests
if [ "$1" == "--test" ]; then
  echo "Running tests..."
  ctest --output-on-failure || error_exit "Tests failed"
fi

# Check if we should run the application
if [ "$1" == "--run" ]; then
  echo "Running application..."
  if [ -f "$EXECUTABLE" ]; then
    "./$EXECUTABLE"
  else
    error_exit "Executable not found at $EXECUTABLE"
  fi
fi
