#pragma once

#include "../../Core/Logger.h"
#include "../../JuceHeader.h"

namespace mcam {
/**
 * A placeholder component for displaying audio meters (VU/PPM).
 * Will be expanded in later phases.
 */
class MeterComponent : public juce::Component {
public:
  /** Constructor */
  MeterComponent();

  /** Destructor */
  ~MeterComponent() override;

  /** Paint the component */
  void paint(juce::Graphics &g) override;

  /** Handle resize events */
  void resized() override;

  /**
   * Update meter with new level
   * @param level New level value (0.0 to 1.0)
   */
  void setLevel(float level);

  /** Set the meter title */
  void setTitle(const juce::String &title);

private:
  float currentLevel = 0.0f;
  juce::String meterTitle;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MeterComponent)
};

} // namespace mcam
