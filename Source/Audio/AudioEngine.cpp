#include "AudioEngine.h"

namespace mcam {

AudioEngine::AudioEngine() {
  LOG_INFO("Creating AudioEngine");

  // Create audio device manager
  audioDeviceManager = std::make_unique<AudioDeviceManager>();

  // Create buffer processor
  bufferProcessor = std::make_unique<BufferProcessor>();
}

AudioEngine::~AudioEngine() {
  LOG_INFO("Destroying AudioEngine");

  // Shutdown if still running
  if (isInitialized) {
    shutdown();
  }
}

bool AudioEngine::initialize() {
  LOG_INFO("Initializing AudioEngine");

  if (isInitialized) {
    LOG_WARNING("AudioEngine already initialized");
    return true;
  }

  // Initialize audio device manager
  if (!audioDeviceManager->initialize()) {
    LOG_ERROR("Failed to initialize audio device manager");
    return false;
  }

  // Register buffer processor with device manager
  audioDeviceManager->addAudioCallback(bufferProcessor.get());

  isInitialized = true;
  LOG_INFO("AudioEngine initialized successfully");

  return true;
}

void AudioEngine::shutdown() {
  LOG_INFO("Shutting down AudioEngine");

  if (!isInitialized) {
    LOG_WARNING("AudioEngine not initialized");
    return;
  }

  // Remove callbacks
  if (audioDeviceManager && bufferProcessor) {
    audioDeviceManager->removeAudioCallback(bufferProcessor.get());
  }

  isInitialized = false;
  LOG_INFO("AudioEngine shut down successfully");
}

AudioDeviceManager &AudioEngine::getAudioDeviceManager() {
  return *audioDeviceManager;
}

BufferProcessor &AudioEngine::getBufferProcessor() { return *bufferProcessor; }

juce::StringArray AudioEngine::getAvailableDeviceNames() const {
  return audioDeviceManager->getAvailableDeviceNames();
}

bool AudioEngine::setAudioDevice(const juce::String &deviceName) {
  return audioDeviceManager->setAudioDevice(deviceName);
}

juce::String AudioEngine::getCurrentDeviceName() const {
  return audioDeviceManager->getCurrentDeviceName();
}

int AudioEngine::getNumInputChannels() const {
  return audioDeviceManager->getNumInputChannels();
}

} // namespace mcam
