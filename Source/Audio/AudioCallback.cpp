#include "AudioCallback.h"

namespace mcam {

AudioCallback::AudioCallback() { LOG_DEBUG("AudioCallback initialized"); }

AudioCallback::~AudioCallback() { LOG_DEBUG("AudioCallback destroyed"); }

void AudioCallback::audioDeviceAboutToStart(juce::AudioIODevice *device) {
  if (device == nullptr) {
    LOG_WARNING("audioDeviceAboutToStart called with null device");
    return;
  }

  currentSampleRate = device->getCurrentSampleRate();
  currentBufferSize = device->getCurrentBufferSizeSamples();
  isProcessingActive = true;

  LOG_DEBUG("Audio device starting: " + juce::String(currentSampleRate) +
            "Hz, " + juce::String(currentBufferSize) + " samples");

  prepareToPlay(currentSampleRate, currentBufferSize);
}

void AudioCallback::audioDeviceStopped() {
  LOG_DEBUG("Audio device stopped");
  isProcessingActive = false;
  releaseResources();
}

void AudioCallback::audioDeviceError(const juce::String &errorMessage) {
  LOG_ERROR("Audio device error: " + errorMessage);
}

void AudioCallback::audioDeviceIOCallbackWithContext(
    const float *const *inputChannelData, int numInputChannels,
    float *const *outputChannelData, int numOutputChannels, int numSamples,
    const juce::AudioIODeviceCallbackContext &context) {
  // Clear output buffers first (silence)
  for (int channel = 0; channel < numOutputChannels; ++channel) {
    if (outputChannelData[channel] != nullptr) {
      juce::FloatVectorOperations::clear(outputChannelData[channel],
                                         numSamples);
    }
  }

  // Process audio data through our callback chain
  if (isProcessingActive && inputChannelData != nullptr) {
    processAudio(inputChannelData, numInputChannels, numSamples);
  }
}

void AudioCallback::processAudio(const float *const *inputChannelData,
                                 int numInputChannels, int numSamples) {
  // Base implementation does nothing - derived classes should override this
  juce::ignoreUnused(inputChannelData, numInputChannels, numSamples);
}

void AudioCallback::prepareToPlay(double sampleRate, int bufferSize) {
  // Base implementation just stores the values - derived classes should
  // override this
  currentSampleRate = sampleRate;
  currentBufferSize = bufferSize;
  Logger::writeToLog("AudioCallback::prepareToPlay - Sample rate: " +
                     juce::String(sampleRate) + "Hz, Buffer size: " +
                     juce::String(bufferSize) + " samples");
}

void AudioCallback::releaseResources() {
  // Base implementation resets state - derived classes should override this
  currentSampleRate = 0.0;
  currentBufferSize = 0;
  isProcessingActive = false;
  Logger::writeToLog("AudioCallback::releaseResources - Resources released");
}

} // namespace mcam
