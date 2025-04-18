#pragma once

#include "../Core/Logger.h"
#include "../JuceHeader.h"

namespace mcam {
/**
 * AudioCallback is a base class for components that need to process audio data.
 * It implements the JUCE AudioIODeviceCallback and can be registered with our
 * AudioDeviceManager.
 */
class AudioCallback : public juce::AudioIODeviceCallback {
public:
  /** Constructor */
  AudioCallback();

  /** Destructor */
  ~AudioCallback() override;

  /**
   * Called when an audio device is about to start playback. Base implementation
   * prepares internal buffers based on device specs.
   */
  void audioDeviceAboutToStart(juce::AudioIODevice *device) override;

  /**
   * Called when an audio device has stopped. Base implementation
   * clears internal buffers.
   */
  void audioDeviceStopped() override;

  /**
   * Called when an audio error occurs. Base implementation
   * logs the error.
   */
  void audioDeviceError(const juce::String &errorMessage) override;

  /**
   * This is the main audio processing callback. Override this in derived
   * classes to process the audio data.
   */
  void audioDeviceIOCallbackWithContext(
      const float *const *inputChannelData, int numInputChannels,
      float *const *outputChannelData, int numOutputChannels, int numSamples,
      const juce::AudioIODeviceCallbackContext &context) override;

protected:
  /**
   * Override this method to process audio data in derived classes.
   * This method is called from audioDeviceIOCallback after basic setup.
   * @param inputChannelData Array of input channel data
   * @param numInputChannels Number of input channels
   * @param numSamples Number of samples in the buffer
   */
  virtual void processAudio(const float *const *inputChannelData,
                            int numInputChannels, int numSamples);

  /**
   * Called when audio properties change, such as sample rate or buffer size.
   * Override this in derived classes to handle the changes.
   * @param sampleRate New sample rate in Hz
   * @param bufferSize New buffer size in samples
   */
  virtual void prepareToPlay(double sampleRate, int bufferSize);

  /**
   * Called before audio processing stops.
   * Override this in derived classes to clean up resources.
   */
  virtual void releaseResources();

  // Current audio specs
  double currentSampleRate = 0.0;
  int currentBufferSize = 0;
  bool isProcessingActive = false;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioCallback)
};

} // namespace mcam
