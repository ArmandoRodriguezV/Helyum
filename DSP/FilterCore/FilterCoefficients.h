#pragma once
#include <JuceHeader.h>

class FilterCoefficients
{
public:
    inline static juce::dsp::IIR::Coefficients<float>* coefficients;
    inline static double sampleRate;
    inline static float gain;

    static float getMagnitude(const float freq)
    {
        if (coefficients == nullptr || sampleRate <= 0.0) return 0.0f;

        const float magnitudes = juce::Decibels::gainToDecibels(
            coefficients->getMagnitudeForFrequency(freq, sampleRate)
        );

        return magnitudes + gain;
    }
};