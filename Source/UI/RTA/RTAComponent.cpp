#include "RTAComponent.h"

namespace mcam {

RTAComponent::RTAComponent() {
  LOG_DEBUG("RTAComponent constructor");

  // Set default title
  rtaTitle = "RTA";

  // Initialize with some mock data
  updateMockData();

  // Start a timer to animate the mock data
  startTimerHz(15);
}

RTAComponent::~RTAComponent() {
  LOG_DEBUG("RTAComponent destructor");
  stopTimer();
}

void RTAComponent::paint(juce::Graphics &g) {
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
  g.drawText(rtaTitle, bounds.removeFromTop(20), juce::Justification::centred,
             true);

  // Draw RTA
  auto rtaBounds = bounds.reduced(4);

  // Background
  g.setColour(juce::Colours::black);
  g.fillRect(rtaBounds);

  // Grid lines
  g.setColour(juce::Colours::grey.withAlpha(0.5f));

  // Vertical grid lines (frequency)
  int numVerticalLines = 10;
  float verticalLineSpacing = rtaBounds.getWidth() / (float)numVerticalLines;

  for (int i = 0; i <= numVerticalLines; ++i) {
    float x = rtaBounds.getX() + i * verticalLineSpacing;
    g.drawLine(x, rtaBounds.getY(), x, rtaBounds.getBottom(), 0.5f);
  }

  // Horizontal grid lines (amplitude)
  int numHorizontalLines = 8;
  float horizontalLineSpacing =
      rtaBounds.getHeight() / (float)numHorizontalLines;

  for (int i = 0; i <= numHorizontalLines; ++i) {
    float y = rtaBounds.getY() + i * horizontalLineSpacing;
    g.drawLine(rtaBounds.getX(), y, rtaBounds.getRight(), y, 0.5f);

    // Draw dB scale
    float db = -i * 6.0f;
    g.setFont(10.0f);
    g.drawText(juce::String(db), rtaBounds.getX() - 25, y - 5, 25, 10,
               juce::Justification::right, false);
  }

  // Draw frequency scale
  g.setFont(10.0f);
  const char *freqLabels[] = {"20", "50", "100", "200", "500",
                              "1k", "2k", "5k",  "10k", "20k"};

  for (int i = 0; i < numVerticalLines; ++i) {
    float x = rtaBounds.getX() + i * verticalLineSpacing;
    g.drawText(freqLabels[i], x - 10, rtaBounds.getBottom() + 2, 20, 10,
               juce::Justification::centred, false);
  }

  // Draw the frequency data if we have enough points
  if (mockFrequencyData.size() > 1) {
    g.setColour(juce::Colours::cyan);

    float barWidth = rtaBounds.getWidth() / (float)mockFrequencyData.size();

    for (int i = 0; i < mockFrequencyData.size(); ++i) {
      float x = rtaBounds.getX() + i * barWidth;
      float barHeight = mockFrequencyData[i] * rtaBounds.getHeight();
      float y = rtaBounds.getBottom() - barHeight;

      // Draw bar
      g.fillRect(x, y, barWidth - 1.0f, barHeight);
    }
  }
}

void RTAComponent::resized() {
  // Nothing specific needed for resize
}

void RTAComponent::updateMockData() {
  // Create some mock frequency data for visualization
  // Real data will be based on FFT in later phases

  const int numBands = 30;

  // Initialize data array if needed
  if (mockFrequencyData.size() != numBands) {
    mockFrequencyData.clear();
    for (int i = 0; i < numBands; ++i) {
      mockFrequencyData.add(0.0f);
    }
  }

  // Update with some random variations
  for (int i = 0; i < numBands; ++i) {
    // Create a frequency roll-off effect (higher frequencies have less energy)
    float baseLine =
        juce::jmap((float)i, 0.0f, (float)numBands - 1, 0.8f, 0.1f);

    // Add some random variation
    float randomVariation = juce::Random::getSystemRandom().nextFloat() * 0.2f;

    // Combine and limit
    mockFrequencyData.set(i,
                          juce::jlimit(0.0f, 1.0f, baseLine + randomVariation));
  }

  repaint();
}

void RTAComponent::setTitle(const juce::String &title) {
  rtaTitle = title;
  repaint();
}

void RTAComponent::timerCallback() {
  // Update the mock data to create animation effect
  updateMockData();
}

} // namespace mcam
