#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    LOG_INFO("Initializing MainComponent");

    // Set the initial size
    setSize(800, 600);

    // Initialize UI components
    initializeUI();

    // Create layout
    createLayout();

    LOG_INFO("MainComponent initialized");
}

MainComponent::~MainComponent()
{
    LOG_INFO("MainComponent being destroyed");
    // Clean up resources
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    LOG_DEBUG("MainComponent::paint called");

    // Fill the background
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Draw a title
    g.setFont(juce::Font(20.0f, juce::Font::bold));
    g.setColour(juce::Colours::white);
    g.drawText("Multi-Channel Audio Monitor", getLocalBounds().removeFromTop(40),
               juce::Justification::centred, true);

    // Draw version information
    g.setFont(juce::Font(12.0f));
    g.setColour(juce::Colours::lightgrey);
    g.drawText("Version " + juce::JUCEApplication::getInstance()->getApplicationVersion(),
               getLocalBounds().removeFromBottom(20),
               juce::Justification::centredRight, true);
}

void MainComponent::resized()
{
    LOG_DEBUG("MainComponent::resized called - new size: " +
              juce::String(getWidth()) + "x" + juce::String(getHeight()));

    // Layout component positioning
    auto area = getLocalBounds();

    // Reserve areas for UI elements
    auto topSection = area.removeFromTop(60);
    auto bottomSection = area.removeFromBottom(40);

    // Place test button in the bottom section
    testButton.setBounds(bottomSection.removeFromRight(100).reduced(10));

    // Position resize corner
    if (resizeCorner != nullptr)
    {
        resizeCorner->setBounds(getWidth() - 16, getHeight() - 16, 16, 16);
    }
}

void MainComponent::applySettings(juce::PropertiesFile* props)
{
    if (props == nullptr)
    {
        LOG_WARNING("applySettings called with nullptr");
        return;
    }

    LOG_INFO("Applying settings from properties");

    // Apply any saved settings
    // For now, we don't have any specific settings to apply
}

void MainComponent::saveSettings(juce::PropertiesFile* props)
{
    if (props == nullptr)
    {
        LOG_WARNING("saveSettings called with nullptr");
        return;
    }

    LOG_INFO("Saving settings to properties");

    // Save any component settings
    // For now, we don't have any specific settings to save
}

void MainComponent::initializeUI()
{
    LOG_INFO("Initializing UI components");

    // Setup test button
    testButton.setButtonText("Test");
    testButton.onClick = [this]() {
        LOG_INFO("Test button clicked");
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                                               "Test Button",
                                               "The application is working correctly!",
                                               "OK");
    };
    addAndMakeVisible(testButton);

    // Setup resize corner
    resizeCorner.reset(new juce::ResizableCornerComponent(this, &resizeConstraints));
    addAndMakeVisible(resizeCorner.get());

    // Setup resize constraints
    resizeConstraints.setSizeLimits(400, 300, 3000, 2000);

    LOG_INFO("UI components initialized");
}

void MainComponent::createLayout()
{
    LOG_INFO("Creating main layout");

    // Currently we just have a simple layout with a test button
    // This will be expanded in future tasks

    LOG_INFO("Main layout created");
}
