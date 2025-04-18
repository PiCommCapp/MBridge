#pragma once

#include "../../Core/Logger.h"
#include "../../JuceHeader.h"

namespace mcam {
/**
 * A placeholder component for displaying Real-Time Analyzer (RTA).
 * Will be expanded in later phases.
 */
class RTAComponent : public juce::Component, public juce::Timer {
public:
  /** Constructor */
  RTAComponent();

  /** Destructor */
  ~RTAComponent() override;

  /** Paint the component */
  void paint(juce::Graphics &g) override;

  /** Handle resize events */
  void resized() override;

  /**
   * Update with mock frequency data for placeholder visualization
   */
  void updateMockData();

  /** Set the RTA title */
  void setTitle(const juce::String &title);

  /** Timer callback to update the display */
  void timerCallback() override;

private:
  juce::String rtaTitle;
  juce::Array<float> mockFrequencyData;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RTAComponent)
};

} // namespace mcam
