#include "../../Source/Audio/AudioCallback.h"
#include "../../Source/Audio/AudioEngine.h"
#include "../../Source/JuceHeader.h"
#include "../Utilities/TestUtils.h"
#include <catch2/catch_test_macros.hpp>

// Mock audio device for testing
class MockAudioDevice : public juce::AudioIODevice {
public:
  MockAudioDevice() : juce::AudioIODevice("Mock Audio Device", "Mock") {}
  ~MockAudioDevice() override = default;

  // AudioIODevice interface implementation
  juce::StringArray getOutputChannelNames() override {
    return {"Out 1", "Out 2"};
  }
  juce::StringArray getInputChannelNames() override {
    return {"In 1", "In 2", "In 3", "In 4"};
  }
  juce::Array<double> getAvailableSampleRates() override {
    return {44100.0, 48000.0};
  }
  juce::Array<int> getAvailableBufferSizes() override {
    return {128, 256, 512, 1024};
  }
  int getDefaultBufferSize() override { return 512; }

  juce::String open(const juce::BigInteger &inputs,
                    const juce::BigInteger &outputs, double sampleRate,
                    int bufferSize) override {
    isOpen = true;
    activeInputChannels = inputs;
    activeOutputChannels = outputs;
    currentSampleRate = sampleRate;
    currentBufferSize = bufferSize;
    return {};
  }

  void close() override { isOpen = false; }
  bool isOpen() override { return isOpen; }
  bool isPlaying() override { return isPlaying_; }

  void start(juce::AudioIODeviceCallback *callback) override {
    if (!isOpen)
      return;

    audioCallback = callback;
    isPlaying_ = true;
  }

  void stop() override {
    isPlaying_ = false;
    audioCallback = nullptr;
  }

  juce::String getLastError() override { return {}; }
  int getCurrentBufferSizeSamples() override { return currentBufferSize; }
  double getCurrentSampleRate() override { return currentSampleRate; }
  int getCurrentBitDepth() override { return 24; }
  juce::BigInteger getActiveOutputChannels() const override {
    return activeOutputChannels;
  }
  juce::BigInteger getActiveInputChannels() const override {
    return activeInputChannels;
  }
  int getOutputLatencyInSamples() override { return 0; }
  int getInputLatencyInSamples() override { return 0; }

  // Simulate audio callback with test data
  void simulateCallback(int numSamples, bool useSineWave = true) {
    if (!isPlaying_ || audioCallback == nullptr)
      return;

    // Create test buffers
    juce::AudioBuffer<float> inputBuffer(
        activeInputChannels.countNumberOfSetBits(), numSamples);
    juce::AudioBuffer<float> outputBuffer(
        activeOutputChannels.countNumberOfSetBits(), numSamples);

    // Fill input buffer with test data
    if (useSineWave) {
      TestUtils::generateSineWave(inputBuffer, 1000.0f, currentSampleRate);
    } else {
      TestUtils::generateSquareWave(inputBuffer, 1000.0f, currentSampleRate);
    }

    // Call the audio callback
    juce::MidiBuffer midiBuffer;
    audioCallback->audioDeviceIOCallback(
        inputBuffer.getArrayOfReadPointers(), inputBuffer.getNumChannels(),
        outputBuffer.getArrayOfWritePointers(), outputBuffer.getNumChannels(),
        numSamples);
  }

private:
  bool isOpen = false;
  bool isPlaying_ = false;
  juce::BigInteger activeInputChannels;
  juce::BigInteger activeOutputChannels;
  double currentSampleRate = 44100.0;
  int currentBufferSize = 512;
  juce::AudioIODeviceCallback *audioCallback = nullptr;
};

TEST_CASE("Audio device detection", "[audio]") {
  SECTION("Mock audio device properties") {
    MockAudioDevice mockDevice;
    REQUIRE(mockDevice.getInputChannelNames().size() == 4);
    REQUIRE(mockDevice.getOutputChannelNames().size() == 2);
    REQUIRE(mockDevice.getAvailableSampleRates().contains(44100.0));
    REQUIRE(mockDevice.getAvailableSampleRates().contains(48000.0));
  }
}

TEST_CASE("Audio engine initialization", "[audio]") {
  SECTION("Audio engine creation") {
    // Test basic construction of AudioEngine
    AudioEngine audioEngine;
    REQUIRE_FALSE(audioEngine.isAudioInitialized());
  }
}

TEST_CASE("Audio device simulation", "[audio]") {
  SECTION("Sine wave generation") {
    juce::AudioBuffer<float> buffer(2, 1000);
    TestUtils::generateSineWave(buffer, 1000.0f, 44100.0f);

    float peak = TestUtils::calculatePeak(buffer, 0);
    float rms = TestUtils::calculateRMS(buffer, 0);

    // Sine wave should have peak of 1.0 and RMS of 1/sqrt(2)
    REQUIRE(peak == Catch::Approx(1.0f).margin(0.001f));
    REQUIRE(rms == Catch::Approx(1.0f / std::sqrt(2.0f)).margin(0.001f));
  }
}

// Additional tests will be implemented once the audio components are more
// developed
