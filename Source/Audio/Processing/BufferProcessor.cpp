#include "BufferProcessor.h"

namespace mcam {

BufferProcessor::BufferProcessor() {
  LOG_INFO("Initializing BufferProcessor");

  // Initialize monitor channels to -1 (no channel assigned)
  for (int i = 0; i < NUM_MONITOR_SLOTS; ++i) {
    monitorChannels[i] = -1;
  }
}

BufferProcessor::~BufferProcessor() {
  LOG_INFO("Shutting down BufferProcessor");
}

bool BufferProcessor::setMonitorChannel(int slotIndex, int channelIndex) {
  // Validate slot index
  if (slotIndex < 0 || slotIndex >= NUM_MONITOR_SLOTS) {
    LOG_ERROR("Invalid slot index: " + juce::String(slotIndex));
    return false;
  }

  // Validate channel index (allow -1 to clear the slot)
  if (channelIndex < -1 || channelIndex >= MAX_CHANNELS) {
    LOG_ERROR("Invalid channel index: " + juce::String(channelIndex));
    return false;
  }

  LOG_INFO("Setting monitor slot " + juce::String(slotIndex) + " to channel " +
           juce::String(channelIndex));

  // Set the channel for this slot
  const juce::ScopedLock sl(bufferLock);
  monitorChannels[slotIndex] = channelIndex;

  return true;
}

int BufferProcessor::getMonitorChannel(int slotIndex) const {
  // Validate slot index
  if (slotIndex < 0 || slotIndex >= NUM_MONITOR_SLOTS) {
    LOG_ERROR("Invalid slot index: " + juce::String(slotIndex));
    return -1;
  }

  const juce::ScopedLock sl(bufferLock);
  return monitorChannels[slotIndex];
}

const juce::AudioBuffer<float> *
BufferProcessor::getMonitorBuffer(int slotIndex) const {
  // Validate slot index
  if (slotIndex < 0 || slotIndex >= NUM_MONITOR_SLOTS) {
    LOG_ERROR("Invalid slot index: " + juce::String(slotIndex));
    return nullptr;
  }

  // Check if this slot has a channel assigned
  const juce::ScopedLock sl(bufferLock);
  if (monitorChannels[slotIndex] < 0) {
    return nullptr;
  }

  return &monitorBuffers[slotIndex];
}

void BufferProcessor::addBufferCallback(BufferCallback callback) {
  if (callback) {
    const juce::ScopedLock sl(callbackLock);
    bufferCallbacks.push_back(std::move(callback));
    LOG_INFO("Added buffer callback");
  }
}

void BufferProcessor::prepareToPlay(double sampleRate, int bufferSize) {
  LOG_INFO("Preparing for playback - Sample Rate: " + juce::String(sampleRate) +
           " Hz, Buffer Size: " + juce::String(bufferSize) + " samples");

  const juce::ScopedLock sl(bufferLock);

  // Prepare monitoring buffers
  for (int i = 0; i < NUM_MONITOR_SLOTS; ++i) {
    monitorBuffers[i].setSize(1, bufferSize);
    monitorBuffers[i].clear();
  }
}

void BufferProcessor::releaseResources() {
  LOG_INFO("Releasing resources");

  const juce::ScopedLock sl(bufferLock);

  // Clear monitoring buffers
  for (int i = 0; i < NUM_MONITOR_SLOTS; ++i) {
    monitorBuffers[i].clear();
  }
}

void BufferProcessor::processAudio(const float *const *inputChannelData,
                                   int numInputChannels, int numSamples) {
  // Process audio for each monitoring slot
  const juce::ScopedLock sl(bufferLock);

  for (int slotIndex = 0; slotIndex < NUM_MONITOR_SLOTS; ++slotIndex) {
    int channelIndex = monitorChannels[slotIndex];

    // Skip slots with no channel assigned
    if (channelIndex < 0 || channelIndex >= numInputChannels)
      continue;

    // Get the buffer for this slot
    juce::AudioBuffer<float> &buffer = monitorBuffers[slotIndex];

    // Ensure buffer is the right size
    if (buffer.getNumSamples() != numSamples) {
      buffer.setSize(1, numSamples, false, true, true);
    }

    // Copy the input channel data to our buffer
    juce::FloatVectorOperations::copy(
        buffer.getWritePointer(0), inputChannelData[channelIndex], numSamples);

    // Notify callbacks about the new data
    const juce::ScopedLock callbackLock(this->callbackLock);
    for (const auto &callback : bufferCallbacks) {
      callback(slotIndex, buffer);
    }
  }
}

} // namespace mcam
