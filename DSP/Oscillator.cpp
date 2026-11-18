#include "Oscillator.h"
#include "../Resources/Constants.h"
#include "../Resources/Wavetable.h"

void Oscillator::process(float* data, int numSamples)
{
    const auto& fm = wavetable[waveFrame];
    for (int i = 0; i < numSamples; ++i) {
        int index = static_cast<int>(phase * Wavetable::FRAME_SIZE);
        float sample = fm[index] * gain;

        data[i] = sample;

        phase += phaseIncrement;

        if (phase >= 1.0)
            phase -= 1.0;
    }
}

void Oscillator::prepare(const double sP)
{
    sampleRate = sP;

    updatePhaseIncrement();
    reset();
}

void Oscillator::reset()
{
    phase = 0.0f;
}

// Setters
void Oscillator::setFrequency(float freq)
{
    frequency = freq;
    updatePhaseIncrement();
}

void Oscillator::setGain(const float g)
{
    gain = g;
}

void Oscillator::setWaveFrame(const float wf)
{
    if (wavetable.size() <= 0)
        return;
    waveFrame = static_cast<int>(wf * (wavetable.size() - 1));
}

void Oscillator::setWavetable(const Wavetable::Table& table)
{
    wavetable = table;
}

void Oscillator::setTune(const float tn)
{
    tune = std::pow(2, tn / 12);
    updatePhaseIncrement();
}

// Private
void Oscillator::updatePhaseIncrement()
{
    phaseIncrement = tune * frequency / sampleRate;
}