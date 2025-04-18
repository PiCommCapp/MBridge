#include "MonitoringSlotComponent.h"

namespace mcam {

MonitoringSlotComponent::MonitoringSlotComponent(int slotIndex)
    : slotIndex(slotIndex), bufferProcessor(nullptr) {
  LOG_DEBUG("MonitoringSlotComponent constructor - slot: " +
            juce::String(slotIndex));

  // Set default title
  slotTitle = "Monitor " + juce::String(slotIndex + 1);

  // Setup channel selector
  channelLabel.setText("Channel:", juce::dontSendNotification);
  channelLabel.setJustificationType(juce::Justification::right);
  addAndMakeVisible(channelLabel);

  channelSelector.setTextWhenNothingSelected("None");
  channelSelector.onChange = [this, slot = this->slotIndex]() {
    if (bufferProcessor != nullptr &&
        channelSelector.getSelectedItemIndex() >= 0) {
      int channelIndex =
          channelSelector.getSelectedItemIndex() - 1; // -1 = None
      LOG_INFO("Channel selected for slot " + juce::String(slot) + ": " +
               juce::String(channelIndex));

      // Update buffer processor
      bufferProcessor->setMonitorChannel(slot, channelIndex);
    }
  };
  addAndMakeVisible(channelSelector);

  // Setup meter
  meter.setTitle("Level");
  addAndMakeVisible(meter);

  // Setup RTA
  rta.setTitle("Spectrum");
  addAndMakeVisible(rta);

  // Random test level for now
  startTimerHz(15); // Update at 15 Hz
}

MonitoringSlotComponent::~MonitoringSlotComponent() {
  LOG_DEBUG("MonitoringSlotComponent destructor - slot: " +
            juce::String(slotIndex));
  stopTimer();
}

void MonitoringSlotComponent::paint(juce::Graphics &g) {
  auto bounds = getLocalBounds();

  // Draw background
  g.setColour(juce::Colours::darkgrey.darker(0.2f));
  g.fillRoundedRectangle(bounds.toFloat(), 8.0f);

  // Draw border
  g.setColour(juce::Colours::grey);
  g.drawRoundedRectangle(bounds.toFloat().reduced(0.5f), 8.0f, 1.0f);

  // Draw title
  g.setColour(juce::Colours::white);
  g.setFont(juce::Font(16.0f, juce::Font::bold));
  g.drawText(slotTitle, bounds.removeFromTop(30).reduced(5, 0),
             juce::Justification::centred, true);
}

void MonitoringSlotComponent::resized() {
  auto bounds = getLocalBounds();

  // Title area
  bounds.removeFromTop(30);

  // Channel selector area
  auto controlsArea = bounds.removeFromTop(30);
  channelLabel.setBounds(controlsArea.removeFromLeft(80).reduced(5, 0));
  channelSelector.setBounds(controlsArea.reduced(5, 0));

  // Equal space for meter and RTA
  auto meterWidth = getWidth() / 4;
  meter.setBounds(bounds.removeFromLeft(meterWidth).reduced(10));
  rta.setBounds(bounds.reduced(10));
}

void MonitoringSlotComponent::setTitle(const juce::String &title) {
  slotTitle = title;
  repaint();
}

void MonitoringSlotComponent::setLevel(float level) { meter.setLevel(level); }

void MonitoringSlotComponent::connectToBufferProcessor(
    BufferProcessor *processor) {
  bufferProcessor = processor;

  if (bufferProcessor != nullptr) {
    // Populate channel selector
    channelSelector.clear();

    // Add "None" option
    channelSelector.addItem("None", 1);

    // Add all possible channels (up to 32)
    for (int i = 0; i < 32; ++i) {
      channelSelector.addItem("Channel " + juce::String(i + 1), i + 2);
    }

    // Select current channel for this slot
    int currentChannel = bufferProcessor->getMonitorChannel(slotIndex);

    if (currentChannel >= 0) {
      channelSelector.setSelectedItemIndex(currentChannel + 1);
    } else {
      channelSelector.setSelectedItemIndex(0); // None
    }

    // Register for buffer updates
    bufferProcessor->addBufferCallback(
        [this](int slot, const juce::AudioBuffer<float> &buffer) {
          if (slot == slotIndex && buffer.getNumSamples() > 0) {
            // Calculate RMS level for meter
            float sum = 0.0f;

            auto *data = buffer.getReadPointer(0);
            for (int i = 0; i < buffer.getNumSamples(); ++i) {
              sum += data[i] * data[i];
            }

            float rms = std::sqrt(sum / buffer.getNumSamples());

            // Convert to dB for more musical display
            float db = juce::Decibels::gainToDecibels(rms);
            // Map -60dB to 0dB to 0.0-1.0 range
            float level = juce::jmap(db, -60.0f, 0.0f, 0.0f, 1.0f);

            // Update on message thread
            juce::MessageManager::callAsync(
                [this, level]() { this->setLevel(level); });
          }
        });
  }
}

void MonitoringSlotComponent::timerCallback() {
  // For testing, generate random levels
  if (bufferProcessor == nullptr) {
    float testLevel = static_cast<float>(rand()) / RAND_MAX;
    setLevel(testLevel * 0.8f); // Scale down a bit
  }
}

} // namespace mcam
