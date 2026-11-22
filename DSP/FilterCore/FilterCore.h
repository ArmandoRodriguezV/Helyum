#pragma once
#include <JuceHeader.h>

class FilterCore
{
public:
    void prepare(const double spRt, const int samplesPerBlock, const int totalChannels);
    void process(juce::AudioBuffer<float>& buffer);

    void setFrequency(const float freq);
    void setQ(const float q);

private:
    using Filter = juce::dsp::IIR::Filter<float>;
    using Coefficients = juce::dsp::IIR::Coefficients<float>;
    juce::dsp::ProcessorDuplicator<Filter, Coefficients> filter;

    double sampleRate = 44100.0;

    float frequency = 20.0f;
    float q = 0.707f;

    void updateFilter();
};