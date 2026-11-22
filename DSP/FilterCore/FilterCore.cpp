#include "FilterCore.h"

void FilterCore::prepare(const double spRt, const int samplesPerBlock, const int totalChannels)
{
    sampleRate = spRt;
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = spRt;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = totalChannels;

    filter.prepare(spec);
    updateFilter();
}

void FilterCore::process(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    filter.process(context);
}

// Setters
void FilterCore::setFrequency(const float freq)
{
    if (freq == frequency) return;
    frequency = freq;
    updateFilter();
}

void FilterCore::setQ(const float qua)
{
    if (qua == q) return;
    q = qua;
    updateFilter();
}

void FilterCore::updateFilter()
{
    *filter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, frequency, q);
}
