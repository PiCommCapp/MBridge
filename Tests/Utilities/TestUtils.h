#pragma once

#include "../../Source/JuceHeader.h"

namespace TestUtils {
// Audio generation utilities

// Generate sine wave test data
inline void generateSineWave(juce::AudioBuffer<float> &buffer, float frequency,
                             float sampleRate, float amplitude = 1.0f) {
  const float period = sampleRate / frequency;
  for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
    auto *data = buffer.getWritePointer(channel);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
      data[sample] =
          amplitude *
          std::sin(2.0f * juce::MathConstants<float>::pi * (sample / period));
    }
  }
}

// Generate square wave test data
inline void generateSquareWave(juce::AudioBuffer<float> &buffer,
                               float frequency, float sampleRate,
                               float amplitude = 1.0f) {
  const float period = sampleRate / frequency;
  for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
    auto *data = buffer.getWritePointer(channel);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
      float phase = std::fmod(sample, period) / period;
      data[sample] = phase < 0.5f ? amplitude : -amplitude;
    }
  }
}

// Generate sawtooth wave test data
inline void generateSawtoothWave(juce::AudioBuffer<float> &buffer,
                                 float frequency, float sampleRate,
                                 float amplitude = 1.0f) {
  const float period = sampleRate / frequency;
  for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
    auto *data = buffer.getWritePointer(channel);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
      float phase = std::fmod(sample, period) / period;
      data[sample] = amplitude * (2.0f * phase - 1.0f);
    }
  }
}

// Generate impulse test data (single spike at start)
inline void generateImpulse(juce::AudioBuffer<float> &buffer,
                            float amplitude = 1.0f) {
  for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
    auto *data = buffer.getWritePointer(channel);
    data[0] = amplitude;
    for (int sample = 1; sample < buffer.getNumSamples(); ++sample) {
      data[sample] = 0.0f;
    }
  }
}

// Generate white noise
inline void generateWhiteNoise(juce::AudioBuffer<float> &buffer,
                               float amplitude = 1.0f) {
  juce::Random random;
  for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
    auto *data = buffer.getWritePointer(channel);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
      data[sample] = amplitude * (random.nextFloat() * 2.0f - 1.0f);
    }
  }
}

// Audio analysis utilities

// Calculate RMS of buffer
inline float calculateRMS(const juce::AudioBuffer<float> &buffer, int channel) {
  float sum = 0.0f;
  const auto *data = buffer.getReadPointer(channel);

  for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
    sum += data[sample] * data[sample];
  }

  return std::sqrt(sum / buffer.getNumSamples());
}

// Calculate peak value
inline float calculatePeak(const juce::AudioBuffer<float> &buffer,
                           int channel) {
  float peak = 0.0f;
  const auto *data = buffer.getReadPointer(channel);

  for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
    peak = std::max(peak, std::abs(data[sample]));
  }

  return peak;
}

// Calculate crest factor (peak to RMS ratio)
inline float calculateCrestFactor(const juce::AudioBuffer<float> &buffer,
                                  int channel) {
  float rms = calculateRMS(buffer, channel);
  float peak = calculatePeak(buffer, channel);

  return peak / (rms > 0.0f ? rms : 1.0e-10f);
}

// Verify if buffer contains silence
inline bool isSilence(const juce::AudioBuffer<float> &buffer, int channel,
                      float threshold = 1.0e-6f) {
  const auto *data = buffer.getReadPointer(channel);

  for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
    if (std::abs(data[sample]) > threshold)
      return false;
  }

  return true;
}
} // namespace TestUtils
