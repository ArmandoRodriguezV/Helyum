#pragma once
#include <JuceHeader.h>
#include "../../Pressets/Filter/Filter.h"
#include "FilterCoefficients.h"

class FilterCore
{
public:
    void prepare(const double spRt, const int samplesPerBlock, const int totalChannels);
    void process(juce::AudioBuffer<float>& buffer);

    void setActive(const bool a);
    void setId(const int id);
    void setFrequency(const float freq);
    void setQ(const float q);
    void setGain(const float g);

private:
    using FilterIIR = juce::dsp::IIR::Filter<float>;
    using Coefficients = juce::dsp::IIR::Coefficients<float>;
    using Duplicator = juce::dsp::ProcessorDuplicator<FilterIIR, Coefficients>;

    Duplicator filter;

    juce::dsp::Gain<float> gainValue;

    double sampleRate = 44100.0;

    bool active = false;
    int filterId = 0;
    float gain = 1.0f;
    float frequency = 20.0f;
    float q = 0.707f;

    void updateFilter() const;
};