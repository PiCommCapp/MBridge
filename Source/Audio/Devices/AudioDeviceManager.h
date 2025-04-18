#pragma once

#include "../../Core/Logger.h"
#include "../../JuceHeader.h"

namespace mcam {
/**
 * AudioDeviceManager wraps the JUCE AudioDeviceManager and provides
 * additional functionality specific to MCAM.
 */
class AudioDeviceManager : private juce::AudioIODeviceCallback {
public:
  /** Constructor */
  AudioDeviceManager();

  /** Destructor */
  ~AudioDeviceManager() override;

  /**
   * Initializes the audio device system and scans for available devices
   * @return true if initialization was successful
   */
  bool initialize();

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

  /**
   * Gets the sample rate of the current device
   * @return current sample rate in Hz
   */
  double getCurrentSampleRate() const;

  /**
   * Gets the buffer size of the current device
   * @return current buffer size in samples
   */
  int getCurrentBufferSize() const;

  /**
   * Add a callback to be notified of audio data
   * @param callback The callback to add
   */
  void addAudioCallback(juce::AudioIODeviceCallback *callback);

  /**
   * Remove a previously-added callback
   * @param callback The callback to remove
   */
  void removeAudioCallback(juce::AudioIODeviceCallback *callback);

private:
  /** juce::AudioIODeviceCallback implementation */
  void audioDeviceIOCallbackWithContext(
      const float *const *inputChannelData, int numInputChannels,
      float *const *outputChannelData, int numOutputChannels, int numSamples,
      const juce::AudioIODeviceCallbackContext &context) override;

  void audioDeviceAboutToStart(juce::AudioIODevice *device) override;
  void audioDeviceStopped() override;
  void audioDeviceError(const juce::String &errorMessage) override;

  // The JUCE audio device manager
  juce::AudioDeviceManager deviceManager;

  // Registered callbacks
  juce::Array<juce::AudioIODeviceCallback *> audioCallbacks;

  // Current device properties
  juce::String currentDeviceName;
  int numInputChannels = 0;
  double sampleRate = 0.0;
  int bufferSize = 0;

  // Mutex for thread safety when handling callbacks
  juce::CriticalSection callbackLock;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioDeviceManager)
};

} // namespace mcam
