#include "FilterCap.h"

FilterCap::FilterCap(ParameterContext &state)
    : state(state)
{
}

void FilterCap::prepare(const double sampleRate, const int samplesPerBlock)
{
    filter.prepare(sampleRate, samplesPerBlock, 2);
}

void FilterCap::process(juce::AudioBuffer<float> &buffer)
{
    filter.setActive(state.get(ParameterIDs::FILTER_ACTIVE));
    filter.setFrequency(state.get(ParameterIDs::FILTER_FREQ));
    filter.setQ(state.get(ParameterIDs::FILTER_Q));
    filter.setGain(state.get(ParameterIDs::FILTER_GAIN));
    filter.setId(state.get(ParameterIDs::FILTER_CHOOSE));
    filter.process(buffer);
}
