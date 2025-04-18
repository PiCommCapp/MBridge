#pragma once

#include "../JuceHeader.h"
#include "Logger.h"

//==============================================================================
/**
 * MainComponent is the primary component for the application.
 * It houses the UI elements and manages the audio processing.
 */
class MainComponent : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    /**
     * Applies application settings from properties
     * @param props Pointer to application properties
     */
    void applySettings(juce::PropertiesFile* props);

    /**
     * Saves component settings to properties
     * @param props Pointer to application properties
     */
    void saveSettings(juce::PropertiesFile* props);

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

    //==============================================================================
    // UI Components
    juce::TextButton testButton;

    // Layout management
    std::unique_ptr<juce::ResizableCornerComponent> resizeCorner;
    juce::ComponentBoundsConstrainer resizeConstraints;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
