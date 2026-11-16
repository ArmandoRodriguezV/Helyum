#pragma once
#include <JuceHeader.h>

class ParameterContext
{
public:
    ParameterContext(juce::AudioProcessorValueTreeState& state):
        params(state) {};

    auto& get(const juce::String& paramId) const
    {
        auto& val = *params.getRawParameterValue(paramId);
        return val;
    }

    juce::AudioProcessorValueTreeState& apvts()
    {
        return params;
    }

private:
    juce::AudioProcessorValueTreeState& params;
};