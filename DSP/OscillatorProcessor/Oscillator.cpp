#include "Oscillator.h"
#include "../../Resources/Constants.h"
#include "../../Resources/Wavetable.h"

void Oscillator::process(float* data, int numSamples)
{
    const auto& fm = wavetable[waveFrame];
    for (int i = 0; i < numSamples; ++i) {
        int index = static_cast<int>(phase * Wavetable::FRAME_SIZE);
        float sample = fm[index] * gain;

        data[i] += sample;

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

void Oscillator::setPhase(const float phs)
{
    if (phase == phs) return;
    phase = phs - std::floor(phs);
}

void Oscillator::reset()
{
    phase = initialPhase;
}

// Setters
void Oscillator::setFrequency(float freq)
{
    if (freq == frequency) return;
    frequency = freq;
    updatePhaseIncrement();
}

void Oscillator::setGain(const float g)
{
    if (gain == g) return;
    gain = g;
}

void Oscillator::setWaveFrame(const float wf)
{
    if (wavetable.empty())
        return;
    waveFrame = static_cast<int>(wf * (wavetable.size() - 1));
}

void Oscillator::setWavetable(const Wavetable::Table& table)
{
    wavetable = table;
}

void Oscillator::setTune(const float tn)
{
    if (tune == tn) return;
    tune = pow(2, tn / 12);
    updatePhaseIncrement();
}

void Oscillator::setTranspose(const float tpse)
{
    if (transpose == tpse) return;
    transpose = std::pow(2, tpse / 12);
    updatePhaseIncrement();
}

// Private
void Oscillator::updatePhaseIncrement()
{
    phaseIncrement = tune * transpose * frequency / sampleRate;
}