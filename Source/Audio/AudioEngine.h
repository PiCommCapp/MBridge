#pragma once

#include "../Core/Logger.h"
#include "../JuceHeader.h"
#include "Devices/AudioDeviceManager.h"
#include "Processing/BufferProcessor.h"

namespace mcam {
/**
 * AudioEngine is the central audio system for the application.
 * It manages audio devices and processing.
 */
class AudioEngine {
public:
  /** Constructor */
  AudioEngine();

  /** Destructor */
  ~AudioEngine();

  /**
   * Initializes the audio system
   * @return true if initialization was successful
   */
  bool initialize();

  /**
   * Shuts down the audio system
   */
  void shutdown();

  /**
   * Gets the audio device manager
   * @return Reference to the audio device manager
   */
  AudioDeviceManager &getAudioDeviceManager();

  /**
   * Gets the buffer processor
   * @return Reference to the buffer processor
   */
  BufferProcessor &getBufferProcessor();

  /**
   * Gets a list of available audio devices
   * @return StringArray of device names
   */
  juce::StringArray getAvailableDeviceNames() const;

  /**
   * Sets the current audio device
   * @param deviceName Name of the device to set active
   * @return true if device was set successfully
   */
  bool setAudioDevice(const juce::String &deviceName);

  /**
   * Gets the current audio device name
   * @return name of current device, or empty string if none
   */
  juce::String getCurrentDeviceName() const;

  /**
   * Gets the number of input channels on the current device
   * @return number of input channels
   */
  int getNumInputChannels() const;

private:
  // Audio device manager
  std::unique_ptr<AudioDeviceManager> audioDeviceManager;

  // Buffer processor
  std::unique_ptr<BufferProcessor> bufferProcessor;

  // Initialization state
  bool isInitialized = false;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};

} // namespace mcam
