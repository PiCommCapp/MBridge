#include "MeterComponent.h"

namespace mcam {

MeterComponent::MeterComponent() {
  LOG_DEBUG("MeterComponent constructor");

  // Set default title
  meterTitle = "Meter";
}

MeterComponent::~MeterComponent() { LOG_DEBUG("MeterComponent destructor"); }

void MeterComponent::paint(juce::Graphics &g) {
  auto bounds = getLocalBounds();

  // Draw background
  g.setColour(juce::Colours::darkgrey);
  g.fillRoundedRectangle(bounds.toFloat(), 4.0f);

  // Draw border
  g.setColour(juce::Colours::grey);
  g.drawRoundedRectangle(bounds.toFloat().reduced(0.5f), 4.0f, 1.0f);

  // Draw title
  g.setColour(juce::Colours::white);
  g.setFont(14.0f);
  g.drawText(meterTitle, bounds.removeFromTop(20), juce::Justification::centred,
             true);

  // Draw meter
  auto meterBounds = bounds.reduced(4);

  // Background
  g.setColour(juce::Colours::black);
  g.fillRect(meterBounds);

  // Calculate level height
  int levelHeight = static_cast<int>(currentLevel * meterBounds.getHeight());
  auto levelBounds = meterBounds.removeFromBottom(levelHeight);

  // Draw level (gradient from green to red)
  juce::ColourGradient gradient(
      juce::Colours::green, meterBounds.getBottomLeft().toFloat(),
      juce::Colours::red, meterBounds.getTopLeft().toFloat(), false);

  gradient.addColour(0.7, juce::Colours::yellow);

  g.setGradientFill(gradient);
  g.fillRect(levelBounds);

  // Draw scale marks
  g.setColour(juce::Colours::grey);
  float dbMarks[] = {0.0f,   -3.0f,  -6.0f,  -12.0f,
                     -18.0f, -24.0f, -36.0f, -48.0f};

  for (auto db : dbMarks) {
    float normLevel = juce::Decibels::decibelsToGain(db);
    int y = meterBounds.getBottom() - normLevel * meterBounds.getHeight();
    g.drawLine(meterBounds.getX(), y, meterBounds.getRight(), y, 1.0f);

    g.setFont(10.0f);
    g.drawText(juce::String(static_cast<int>(db)), meterBounds.getRight() + 2,
               y - 5, 20, 10, juce::Justification::left, false);
  }
}

void MeterComponent::resized() {
  // Nothing specific needed for resize
}

void MeterComponent::setLevel(float level) {
  // Ensure level is within valid range
  currentLevel = juce::jlimit(0.0f, 1.0f, level);
  repaint();
}

void MeterComponent::setTitle(const juce::String &title) {
  meterTitle = title;
  repaint();
}

} // namespace mcam
