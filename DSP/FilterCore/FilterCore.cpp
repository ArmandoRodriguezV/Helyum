#include "FilterCore.h"

void FilterCore::prepare(const double spRt, const int samplesPerBlock, const int totalChannels)
{
    sampleRate = spRt;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = spRt;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = totalChannels;

    filter.prepare(spec);
    FilterCoefficients::sampleRate = sampleRate;

    updateFilter();
}

void FilterCore::process(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    if (!active) return;

    FilterCoefficients::coefficients = filter.state.get();
    FilterCoefficients::sampleRate = sampleRate;

    filter.process(context);
    gainValue.process(context);
}

// Setters
void FilterCore::setActive(const bool a)
{
    if (active == a) return;
    active = a;
}

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

void FilterCore::setId(const int id)
{
    if (id == filterId) return;
    filterId = id;
    updateFilter();
}

void FilterCore::setGain(const float g)
{
    if (g == gain) return;
    gain = g;
    gainValue.setGainDecibels(gain);
    FilterCoefficients::gain = gain;
}

void FilterCore::updateFilter() const {
    const float safeFrequency = juce::jlimit(1.0f, static_cast<float>(sampleRate * 0.49), frequency);
    const float safeQ = juce::jmax(0.001f, q);

    switch (filterId)
    {
        case 0:
            *filter.state = *Coefficients::makeLowPass(sampleRate, safeFrequency, safeQ);
            break;

        case 1:
            *filter.state = *Coefficients::makeHighPass(sampleRate, safeFrequency, safeQ);
            break;

        case 2:
            *filter.state = *Coefficients::makeNotch(sampleRate, safeFrequency, safeQ);
            break;
        default:
            break;
    }
}
