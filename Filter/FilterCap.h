#pragma once
#include <JuceHeader.h>
#include "../Resources/ParameterIDs.h"
#include "../Resources/ParameterContext.h"
#include "../DSP/FilterCore/FilterCore.h"

class FilterCap
{
public:
    FilterCap(ParameterContext& state);
    void process(juce::AudioBuffer<float>& buffer);
    void prepare(const double sampleRate, const int samplesPerBlock);

private:
    FilterCore filter;
    ParameterContext& state;
};