#include "Oscillator.h"
#include "../Resources/Constants.h"
#include "../Resources/Wavetable.h"

void Oscillator::process(float* data, int numSamples)
{
    const auto& frame = wavetable[waveframe];

    for (int i = 0; i < numSamples; ++i) {
        int index = static_cast<int>(phase * Wavetable::FRAME_SIZE);
        float sample = frame[index] * gain;

        data[i] = sample;

        phase += phaseIncrement;

        if (phase >= 1.0)
            phase -= 1.0;
    }
}

void Oscillator::prepare(const double sP)
{
    sampleRate = sP;
    setWaveform(0);
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
    waveframe = static_cast<int>(wf * (wavetable.size() - 1));
}

void Oscillator::setWaveform(const int id)
{
    const auto wt = WavetableLibrary::find(id);
    if (wt == nullptr)
        return;

    wavetable = Wavetable::loadWavetable(wt->data, wt->size);
}

// Private
void Oscillator::updatePhaseIncrement()
{
    phaseIncrement = frequency / sampleRate;
}