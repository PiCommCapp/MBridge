#pragma once

#include "../Audio/AudioEngine.h"
#include "../JuceHeader.h"
#include "../UI/MonitoringSlotComponent.h"
#include "Logger.h"

//==============================================================================
/**
 * MainComponent is the primary component for the application.
 * It houses the UI elements and manages the audio processing.
 */
class MainComponent : public juce::Component {
public:
  //==============================================================================
  MainComponent();
  ~MainComponent() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

  /**
   * Applies application settings from properties
   * @param props Pointer to application properties
   */
  void applySettings(juce::PropertiesFile *props);

  /**
   * Saves component settings to properties
   * @param props Pointer to application properties
   */
  void saveSettings(juce::PropertiesFile *props);

private:
  //==============================================================================
  /**
   * Initialize the UI components
   */
  void initializeUI();

  /**
   * Creates the main layout
   */
  void createLayout();

  /**
   * Initialize the audio engine
   */
  void initializeAudio();

  /**
   * Create the monitoring slots
   */
  void createMonitoringSlots();

  //==============================================================================
  // Audio engine
  std::unique_ptr<mcam::AudioEngine> audioEngine;

  // UI Components
  juce::TextButton testButton;
  juce::ComboBox deviceSelector;
  juce::Label deviceLabel;
  juce::Label channelCountLabel;

  // Monitoring slots
  std::array<std::unique_ptr<mcam::MonitoringSlotComponent>, 4> monitoringSlots;
  juce::ToggleButton verticalLayoutButton;

  // Layout management
  std::unique_ptr<juce::ResizableCornerComponent> resizeCorner;
  juce::ComponentBoundsConstrainer resizeConstraints;
  bool isVerticalLayout = false;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
