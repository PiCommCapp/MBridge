#include "../../Source/JuceHeader.h"
#include "../Utilities/TestUtils.h"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

// These imports will need to be adjusted as Processing components are
// implemented #include "../../Source/Processing/Metering/VUMeterCalculator.h"
// #include "../../Source/Processing/Metering/PPMMeterCalculator.h"

// These test cases will be filled in as the processing components are
// implemented
TEST_CASE("RMS calculation for VU meter", "[processing][metering]") {
  SECTION("RMS calculation of sine wave") {
    // Create test buffer
    juce::AudioBuffer<float> buffer(1, 44100);

    // Generate sine wave (0dB full scale)
    TestUtils::generateSineWave(buffer, 1000.0f, 44100.0f);

    // Calculate RMS value - sine wave RMS should be 1/sqrt(2) of peak
    float calculatedRMS = TestUtils::calculateRMS(buffer, 0);
    float expectedRMS = 1.0f / std::sqrt(2.0f);

    // Check that calculated RMS matches expected value
    REQUIRE(calculatedRMS == Catch::Approx(expectedRMS).margin(0.001f));

    // Once VUMeterCalculator is implemented:
    // VUMeterCalculator vuMeter;
    // vuMeter.processBlock(buffer);
    // REQUIRE(vuMeter.getMeterLevel() ==
    // Catch::Approx(expectedRMS).margin(0.001f));
  }

  SECTION("RMS calculation of square wave") {
    // Create test buffer
    juce::AudioBuffer<float> buffer(1, 44100);

    // Generate square wave (0dB full scale)
    TestUtils::generateSquareWave(buffer, 1000.0f, 44100.0f);

    // Calculate RMS value - square wave RMS should equal amplitude
    float calculatedRMS = TestUtils::calculateRMS(buffer, 0);

    // Check that calculated RMS matches expected value
    REQUIRE(calculatedRMS == Catch::Approx(1.0f).margin(0.001f));

    // Once VUMeterCalculator is implemented:
    // VUMeterCalculator vuMeter;
    // vuMeter.processBlock(buffer);
    // REQUIRE(vuMeter.getMeterLevel() == Catch::Approx(1.0f).margin(0.001f));
  }
}

TEST_CASE("Peak detection for PPM meter", "[processing][metering]") {
  SECTION("Peak detection of sine wave") {
    // Create test buffer
    juce::AudioBuffer<float> buffer(1, 44100);

    // Generate sine wave (0dB full scale)
    TestUtils::generateSineWave(buffer, 1000.0f, 44100.0f);

    // Calculate peak value
    float peak = TestUtils::calculatePeak(buffer, 0);

    // Check that peak is approximately 1.0
    REQUIRE(peak == Catch::Approx(1.0f).margin(0.001f));

    // Once PPMMeterCalculator is implemented:
    // PPMMeterCalculator ppmMeter;
    // ppmMeter.processBlock(buffer);
    // REQUIRE(ppmMeter.getMeterLevel() == Catch::Approx(1.0f).margin(0.001f));
  }

  SECTION("Peak detection of impulse") {
    // Create test buffer
    juce::AudioBuffer<float> buffer(1, 44100);

    // Generate impulse signal
    TestUtils::generateImpulse(buffer);

    // Calculate peak value
    float peak = TestUtils::calculatePeak(buffer, 0);

    // Check that peak is 1.0
    REQUIRE(peak == 1.0f);

    // Once PPMMeterCalculator is implemented, test attack time response:
    // PPMMeterCalculator ppmMeter;
    // ppmMeter.processBlock(buffer);
    // float meterLevel = ppmMeter.getMeterLevel();
    // Test according to PPM specs (should have fast attack)
  }
}

TEST_CASE("Crest factor calculations", "[processing][analysis]") {
  SECTION("Sine wave crest factor") {
    juce::AudioBuffer<float> buffer(1, 44100);
    TestUtils::generateSineWave(buffer, 1000.0f, 44100.0f);

    float crestFactor = TestUtils::calculateCrestFactor(buffer, 0);
    float expectedCF = std::sqrt(2.0f); // Sine wave crest factor is sqrt(2)

    REQUIRE(crestFactor == Catch::Approx(expectedCF).margin(0.01f));
  }

  SECTION("Square wave crest factor") {
    juce::AudioBuffer<float> buffer(1, 44100);
    TestUtils::generateSquareWave(buffer, 1000.0f, 44100.0f);

    float crestFactor = TestUtils::calculateCrestFactor(buffer, 0);

    // Square wave has a crest factor of 1 (peak equals RMS)
    REQUIRE(crestFactor == Catch::Approx(1.0f).margin(0.01f));
  }
}

// Additional test cases for VU and PPM meter integration, ballistics, and
// scaling will be added as the processing components are implemented
