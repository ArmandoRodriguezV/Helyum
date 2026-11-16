#pragma once
#include <JuceHeader.h>

class Oscillator
{
public:
    // Core
    void process(float* data, int numSamples);
    void prepare(double sP);
    void reset();

    // Setter
    void setFrequency(const float freq);
    void setGain(const float g);
    void setWaveFrame(const float wf);

private:
    juce::Random random;
    double sampleRate = 44100.0;

    float frequency = 440.0;
    float gain = 0.5f;

    float phase = 0.0f;
    float phaseIncrement = 0.0f;

    std::vector<std::vector<float>> wavetable;
    int waveframe = 0;

    void updatePhaseIncrement();
};
