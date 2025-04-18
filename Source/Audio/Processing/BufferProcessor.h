#pragma once

#include "../../Core/Logger.h"
#include "../../JuceHeader.h"
#include "../AudioCallback.h"

namespace mcam {
/**
 * BufferProcessor handles audio buffer processing and provides a framework
 * for routing audio data to various monitoring slots.
 */
class BufferProcessor : public AudioCallback {
public:
  /** Number of monitoring slots available */
  static constexpr int NUM_MONITOR_SLOTS = 4;

  /** Maximum number of channels that can be processed */
  static constexpr int MAX_CHANNELS = 32;

  /** Constructor */
  BufferProcessor();

  /** Destructor */
  ~BufferProcessor() override;

  /**
   * Sets the input channel for a specific monitoring slot
   * @param slotIndex The slot index (0-3)
   * @param channelIndex The input channel index to route to this slot
   * @return true if successful
   */
  bool setMonitorChannel(int slotIndex, int channelIndex);

  /**
   * Gets the current input channel for a specific monitoring slot
   * @param slotIndex The slot index (0-3)
   * @return The input channel index, or -1 if not set
   */
  int getMonitorChannel(int slotIndex) const;

  /**
   * Gets the most recent buffer for a specific monitoring slot
   * @param slotIndex The slot index (0-3)
   * @return Pointer to the audio buffer, or nullptr if not available
   */
  const juce::AudioBuffer<float> *getMonitorBuffer(int slotIndex) const;

  /**
   * Adds a listener that will be notified when new audio data is available
   * @param callback Function to call when new data is available
   */
  using BufferCallback =
      std::function<void(int slotIndex, const juce::AudioBuffer<float> &)>;
  void addBufferCallback(BufferCallback callback);

protected:
  /** Overridden from AudioCallback */
  void prepareToPlay(double sampleRate, int bufferSize) override;
  void releaseResources() override;
  void processAudio(const float *const *inputChannelData, int numInputChannels,
                    int numSamples) override;

private:
  // The input channel assigned to each monitoring slot
  std::array<int, NUM_MONITOR_SLOTS> monitorChannels;

  // Buffer for each monitoring slot
  std::array<juce::AudioBuffer<float>, NUM_MONITOR_SLOTS> monitorBuffers;

  // Lock for thread-safe buffer access
  mutable juce::CriticalSection bufferLock;

  // Callbacks for new audio data
  std::vector<BufferCallback> bufferCallbacks;

  // Lock for callback list access
  mutable juce::CriticalSection callbackLock;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BufferProcessor)
};

} // namespace mcam
