#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() {
  LOG_INFO("Initializing MainComponent");

  // Set the initial size
  setSize(1024, 768);

  // Initialize UI components
  initializeUI();

  // Create layout
  createLayout();

  // Initialize audio
  initializeAudio();

  // Create monitoring slots
  createMonitoringSlots();

  LOG_INFO("MainComponent initialized");
}

MainComponent::~MainComponent() {
  LOG_INFO("MainComponent being destroyed");
  // Audio engine will be cleaned up automatically
}

//==============================================================================
void MainComponent::paint(juce::Graphics &g) {
  LOG_DEBUG("MainComponent::paint called");

  // Fill the background
  g.fillAll(
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

  // Draw a title
  g.setFont(juce::Font(20.0f, juce::Font::bold));
  g.setColour(juce::Colours::white);
  g.drawText("Multi-Channel Audio Monitor", getLocalBounds().removeFromTop(40),
             juce::Justification::centred, true);

  // Draw version information
  g.setFont(juce::Font(12.0f));
  g.setColour(juce::Colours::lightgrey);
  g.drawText("Version " +
                 juce::JUCEApplication::getInstance()->getApplicationVersion(),
             getLocalBounds().removeFromBottom(20),
             juce::Justification::centredRight, true);
}

void MainComponent::resized() {
  LOG_DEBUG("MainComponent::resized called - new size: " +
            juce::String(getWidth()) + "x" + juce::String(getHeight()));

  // Layout component positioning
  auto area = getLocalBounds();

  // Reserve areas for UI elements
  auto topSection = area.removeFromTop(60);
  auto bottomSection = area.removeFromBottom(40);

  // Place device selector in top section
  deviceLabel.setBounds(topSection.removeFromLeft(150).reduced(10));
  deviceSelector.setBounds(topSection.removeFromLeft(250).reduced(10));
  channelCountLabel.setBounds(topSection.removeFromLeft(200).reduced(10));
  verticalLayoutButton.setBounds(topSection.removeFromLeft(120).reduced(10));

  // Place test button in the bottom section
  testButton.setBounds(bottomSection.removeFromRight(100).reduced(10));

  // Position resize corner
  if (resizeCorner != nullptr) {
    resizeCorner->setBounds(getWidth() - 16, getHeight() - 16, 16, 16);
  }

  // Position monitoring slots based on layout
  if (isVerticalLayout) {
    // Vertical layout (slots stacked)
    int slotHeight = area.getHeight() / 4;

    for (int i = 0; i < 4; ++i) {
      if (monitoringSlots[i] != nullptr) {
        monitoringSlots[i]->setBounds(
            area.removeFromTop(slotHeight).reduced(10));
      }
    }
  } else {
    // Horizontal layout (slots side by side)
    int slotWidth = area.getWidth() / 2;
    int slotHeight = area.getHeight() / 2;

    for (int row = 0; row < 2; ++row) {
      auto rowBounds = area.removeFromTop(slotHeight);

      for (int col = 0; col < 2; ++col) {
        int slotIndex = row * 2 + col;
        if (monitoringSlots[slotIndex] != nullptr) {
          monitoringSlots[slotIndex]->setBounds(
              rowBounds.removeFromLeft(slotWidth).reduced(10));
        }
      }
    }
  }
}

void MainComponent::applySettings(juce::PropertiesFile *props) {
  if (props == nullptr) {
    LOG_WARNING("applySettings called with nullptr");
    return;
  }

  LOG_INFO("Applying settings from properties");

  // Load layout setting
  isVerticalLayout = props->getBoolValue("verticalLayout", false);
  verticalLayoutButton.setToggleState(isVerticalLayout,
                                      juce::dontSendNotification);

  // Update layout
  resized();
}

void MainComponent::saveSettings(juce::PropertiesFile *props) {
  if (props == nullptr) {
    LOG_WARNING("saveSettings called with nullptr");
    return;
  }

  LOG_INFO("Saving settings to properties");

  // Save layout setting
  props->setValue("verticalLayout", isVerticalLayout);
}

void MainComponent::initializeUI() {
  LOG_INFO("Initializing UI components");

  // Setup test button
  testButton.setButtonText("Test");
  testButton.onClick = [this]() {
    LOG_INFO("Test button clicked");

    // Generate a test tone for monitoring slot 0
    if (audioEngine != nullptr) {
      auto &processor = audioEngine->getBufferProcessor();

      // Update channel routing based on selected channel
      int channel = 0; // Use first channel for test
      processor.setMonitorChannel(0, channel);

      juce::AlertWindow::showMessageBoxAsync(
          juce::AlertWindow::InfoIcon, "Test Button",
          "Monitoring channel " + juce::String(channel) + " on slot 0", "OK");
    }
  };
  addAndMakeVisible(testButton);

  // Setup device selector
  deviceLabel.setText("Audio Device:", juce::dontSendNotification);
  deviceLabel.setJustificationType(juce::Justification::right);
  addAndMakeVisible(deviceLabel);

  deviceSelector.setTextWhenNothingSelected("Select Audio Device");
  deviceSelector.onChange = [this]() {
    if (audioEngine != nullptr && deviceSelector.getSelectedItemIndex() >= 0) {
      auto deviceName = deviceSelector.getText();
      LOG_INFO("Device selected: " + deviceName);

      audioEngine->setAudioDevice(deviceName);

      // Update channel count label
      int channels = audioEngine->getNumInputChannels();
      channelCountLabel.setText("Input Channels: " + juce::String(channels),
                                juce::dontSendNotification);
    }
  };
  addAndMakeVisible(deviceSelector);

  // Setup channel count label
  channelCountLabel.setText("Input Channels: 0", juce::dontSendNotification);
  addAndMakeVisible(channelCountLabel);

  // Setup layout toggle
  verticalLayoutButton.setButtonText("Vertical Layout");
  verticalLayoutButton.setToggleState(isVerticalLayout,
                                      juce::dontSendNotification);
  verticalLayoutButton.onClick = [this]() {
    isVerticalLayout = verticalLayoutButton.getToggleState();
    LOG_INFO("Layout changed to " +
             juce::String(isVerticalLayout ? "vertical" : "horizontal"));
    resized();
  };
  addAndMakeVisible(verticalLayoutButton);

  // Setup resize corner
  resizeCorner.reset(
      new juce::ResizableCornerComponent(this, &resizeConstraints));
  addAndMakeVisible(resizeCorner.get());

  // Setup resize constraints
  resizeConstraints.setSizeLimits(800, 600, 3000, 2000);

  LOG_INFO("UI components initialized");
}

void MainComponent::createLayout() {
  LOG_INFO("Creating main layout");

  // Main layout is handled in the resized() method

  LOG_INFO("Main layout created");
}

void MainComponent::initializeAudio() {
  LOG_INFO("Initializing audio");

  // Create audio engine
  audioEngine = std::make_unique<mcam::AudioEngine>();

  // Initialize audio engine
  if (audioEngine->initialize()) {
    LOG_INFO("Audio engine initialized successfully");

    // Populate device selector
    auto deviceNames = audioEngine->getAvailableDeviceNames();

    deviceSelector.clear();
    for (int i = 0; i < deviceNames.size(); ++i) {
      deviceSelector.addItem(deviceNames[i], i + 1);
    }

    // Select current device if any
    auto currentDevice = audioEngine->getCurrentDeviceName();
    if (currentDevice.isNotEmpty()) {
      deviceSelector.setText(currentDevice, juce::dontSendNotification);

      // Update channel count label
      int channels = audioEngine->getNumInputChannels();
      channelCountLabel.setText("Input Channels: " + juce::String(channels),
                                juce::dontSendNotification);
    }
  } else {
    LOG_ERROR("Failed to initialize audio engine");

    juce::AlertWindow::showMessageBoxAsync(
        juce::AlertWindow::WarningIcon, "Audio Initialization Error",
        juce::String("Failed to initialize audio engine. Some features may not "
                     "work correctly."),
        "OK");
  }
}

void MainComponent::createMonitoringSlots() {
  LOG_INFO("Creating monitoring slots");

  // Create the four monitoring slots
  for (int i = 0; i < 4; ++i) {
    monitoringSlots[i] = std::make_unique<mcam::MonitoringSlotComponent>(i);
    monitoringSlots[i]->setTitle("Monitor " + juce::String(i + 1));

    // Connect to buffer processor if audio engine is initialized
    if (audioEngine != nullptr) {
      monitoringSlots[i]->connectToBufferProcessor(
          &audioEngine->getBufferProcessor());
    }

    addAndMakeVisible(*monitoringSlots[i]);
  }

  LOG_INFO("Monitoring slots created");
}
