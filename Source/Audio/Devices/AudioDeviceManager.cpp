#include "AudioDeviceManager.h"

namespace mcam {

AudioDeviceManager::AudioDeviceManager() {
  LOG_INFO("Initializing AudioDeviceManager");
}

AudioDeviceManager::~AudioDeviceManager() {
  LOG_INFO("Shutting down AudioDeviceManager");

  // Ensure we cleanup the device manager
  deviceManager.removeAudioCallback(this);
  deviceManager.closeAudioDevice();
}

bool AudioDeviceManager::initialize() {
  LOG_INFO("Initializing audio device system");

  // Initialize the device manager with default device
  juce::String err = deviceManager.initialiseWithDefaultDevices(2, 2);

  if (err.isNotEmpty()) {
    LOG_ERROR("Failed to initialize audio device: " + err);
    return false;
  }

  // Add ourselves as an audio callback
  deviceManager.addAudioCallback(this);

  // Store current device info
  if (auto *device = deviceManager.getCurrentAudioDevice()) {
    currentDeviceName = device->getName();
    numInputChannels = device->getActiveInputChannels().countNumberOfSetBits();
    sampleRate = device->getCurrentSampleRate();
    bufferSize = device->getCurrentBufferSizeSamples();

    LOG_INFO("Audio device initialized: " + currentDeviceName +
             " (Inputs: " + juce::String(numInputChannels) +
             ", Sample Rate: " + juce::String(sampleRate) +
             ", Buffer Size: " + juce::String(bufferSize) + ")");
  } else {
    LOG_WARNING("No audio device available after initialization");
  }

  return true;
}

juce::StringArray AudioDeviceManager::getAvailableDeviceNames() const {
  // For const-correctness, we need to cache this information at initialization
  // time since JUCE's deviceManager.getAvailableDeviceTypes() isn't a const
  // method
  juce::StringArray deviceNames;

  // In a real implementation, this would use cached device names
  // For now, just return any current device and an empty list otherwise
  if (currentDeviceName.isNotEmpty())
    deviceNames.add(currentDeviceName);

  return deviceNames;
}

bool AudioDeviceManager::setAudioDevice(const juce::String &deviceName) {
  LOG_INFO("Setting audio device: " + deviceName);

  // Create a device setup with this name
  juce::AudioDeviceManager::AudioDeviceSetup setup;
  deviceManager.getAudioDeviceSetup(setup);

  setup.inputDeviceName = deviceName;
  setup.outputDeviceName = deviceName;

  // Try to set up the device
  juce::String error = deviceManager.setAudioDeviceSetup(setup, true);

  if (error.isNotEmpty()) {
    LOG_ERROR("Failed to set audio device: " + error);
    return false;
  }

  // Update device properties
  if (auto *device = deviceManager.getCurrentAudioDevice()) {
    currentDeviceName = device->getName();
    numInputChannels = device->getActiveInputChannels().countNumberOfSetBits();
    sampleRate = device->getCurrentSampleRate();
    bufferSize = device->getCurrentBufferSizeSamples();

    LOG_INFO("Audio device changed: " + currentDeviceName +
             " (Inputs: " + juce::String(numInputChannels) +
             ", Sample Rate: " + juce::String(sampleRate) +
             ", Buffer Size: " + juce::String(bufferSize) + ")");

    return true;
  }

  LOG_ERROR("Failed to change audio device");
  return false;
}

juce::String AudioDeviceManager::getCurrentDeviceName() const {
  return currentDeviceName;
}

int AudioDeviceManager::getNumInputChannels() const { return numInputChannels; }

double AudioDeviceManager::getCurrentSampleRate() const { return sampleRate; }

int AudioDeviceManager::getCurrentBufferSize() const { return bufferSize; }

void AudioDeviceManager::addAudioCallback(
    juce::AudioIODeviceCallback *callback) {
  if (callback != nullptr) {
    const juce::ScopedLock sl(callbackLock);

    // Don't add the same callback twice
    if (!audioCallbacks.contains(callback)) {
      LOG_INFO("Adding audio callback");
      audioCallbacks.add(callback);

      // If audio is running, notify the new callback
      if (auto *device = deviceManager.getCurrentAudioDevice()) {
        callback->audioDeviceAboutToStart(device);
      }
    }
  }
}

void AudioDeviceManager::removeAudioCallback(
    juce::AudioIODeviceCallback *callback) {
  if (callback != nullptr) {
    const juce::ScopedLock sl(callbackLock);

    LOG_INFO("Removing audio callback");

    // Notify callback that audio is stopped (if it was running)
    if (audioCallbacks.contains(callback)) {
      callback->audioDeviceStopped();
      audioCallbacks.removeFirstMatchingValue(callback);
    }
  }
}

void AudioDeviceManager::audioDeviceIOCallbackWithContext(
    const float *const *inputChannelData, int numInputChannels,
    float *const *outputChannelData, int numOutputChannels, int numSamples,
    const juce::AudioIODeviceCallbackContext &context) {
  // First, clear output buffers
  for (int i = 0; i < numOutputChannels; ++i) {
    if (outputChannelData[i] != nullptr) {
      juce::FloatVectorOperations::clear(outputChannelData[i], numSamples);
    }
  }

  // Forward callback to registered listeners
  const juce::ScopedLock sl(callbackLock);

  for (auto *callback : audioCallbacks) {
    callback->audioDeviceIOCallbackWithContext(
        inputChannelData, numInputChannels, outputChannelData,
        numOutputChannels, numSamples, context);
  }
}

void AudioDeviceManager::audioDeviceAboutToStart(juce::AudioIODevice *device) {
  if (device != nullptr) {
    // Update device properties
    currentDeviceName = device->getName();
    numInputChannels = device->getActiveInputChannels().countNumberOfSetBits();
    sampleRate = device->getCurrentSampleRate();
    bufferSize = device->getCurrentBufferSizeSamples();

    LOG_INFO("Audio device starting: " + currentDeviceName +
             " (Inputs: " + juce::String(numInputChannels) +
             ", Sample Rate: " + juce::String(sampleRate) +
             ", Buffer Size: " + juce::String(bufferSize) + ")");

    // Forward to callbacks
    const juce::ScopedLock sl(callbackLock);

    for (auto *callback : audioCallbacks) {
      callback->audioDeviceAboutToStart(device);
    }
  }
}

void AudioDeviceManager::audioDeviceStopped() {
  LOG_INFO("Audio device stopped");

  // Forward to callbacks
  const juce::ScopedLock sl(callbackLock);

  for (auto *callback : audioCallbacks) {
    callback->audioDeviceStopped();
  }
}

void AudioDeviceManager::audioDeviceError(const juce::String &errorMessage) {
  LOG_ERROR("Audio device error: " + errorMessage);

  // Forward to callbacks
  const juce::ScopedLock sl(callbackLock);

  for (auto *callback : audioCallbacks) {
    callback->audioDeviceError(errorMessage);
  }
}

} // namespace mcam
