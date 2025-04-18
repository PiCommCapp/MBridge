#pragma once

#include "../Audio/Processing/BufferProcessor.h"
#include "../Core/Logger.h"
#include "../JuceHeader.h"
#include "Meters/MeterComponent.h"
#include "RTA/RTAComponent.h"

namespace mcam {
/**
 * A component that represents a single monitoring slot.
 * Contains a meter and RTA display for one audio channel.
 */
class MonitoringSlotComponent : public juce::Component, public juce::Timer {
public:
  /** Constructor */
  MonitoringSlotComponent(int slotIndex);

  /** Destructor */
  ~MonitoringSlotComponent() override;

  /** Paint the component */
  void paint(juce::Graphics &g) override;

  /** Handle resize events */
  void resized() override;

  /**
   * Set the title for this monitoring slot
   * @param title The new title
   */
  void setTitle(const juce::String &title);

  /**
   * Set the audio level for the meter
   * @param level The level (0.0 to 1.0)
   */
  void setLevel(float level);

  /**
   * Connect to a buffer processor to receive audio data
   * @param processor The buffer processor to connect to
   */
  void connectToBufferProcessor(BufferProcessor *processor);

  /** Timer callback for animations and updates */
  void timerCallback() override;

private:
  int slotIndex;
  juce::String slotTitle;

  // Channel selection
  juce::ComboBox channelSelector;
  juce::Label channelLabel;

  // UI Components
  MeterComponent meter;
  RTAComponent rta;

  // Pointer to the buffer processor (may be nullptr)
  BufferProcessor *bufferProcessor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MonitoringSlotComponent)
};

} // namespace mcam
