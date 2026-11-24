
#include "ADSRCore.h"

void ADSRCore::prepare(const double sampleRate)
{
    this->sampleRate = sampleRate;
}

void ADSRCore::process(juce::AudioBuffer<float>& buffer, int startSample, int numSamples)
{
    for (int sp = 0; sp < numSamples; ++sp)
    {
        float envelope = 0.0f;
        switch (stage)
        {
            case ATTACK:
            {
                float progress = static_cast<float>(currentSample) / static_cast<float>(attackSamples);
                progress = juce::jlimit(0.0f, 1.0f, progress);

                float exponent = std::pow(2.0f, attackCurve);
                envelope = std::pow(progress, exponent);

                currentLevel = envelope;
                currentSample++;
                if (currentSample >= attackSamples)
                {
                    stage = DECAY;
                    currentSample = 0;
                }
                break;
            }
            case DECAY:
            {
                float progress = static_cast<float>(currentSample) / static_cast<float>(decaySamples);
                progress = juce::jlimit(0.0f, 1.0f, progress);
                float exponent = std::pow(2.0f, decayCurve);
                envelope = std::pow(progress, exponent);

                currentLevel = juce::jmap(envelope, 1.0f, sustain);
                envelope = currentLevel;

                currentSample++;
                if (currentSample >= decaySamples)
                {
                    stage = SUSTAIN;
                    currentSample = 0;
                }
                break;
            }
            case SUSTAIN:
            {
                envelope = sustain;
                currentLevel = envelope;
                break;
            }
            case RELEASE:
            {
                if (release <= 0 || releaseSamples <= 0)
                {
                    envelope = 0.0f;
                    currentLevel = 0.0f;
                    stage = IDLE;
                    break;
                }

                float progress = static_cast<float>(currentSample) / static_cast<float>(releaseSamples);
                envelope = juce::jlimit(0.0f, 1.0f, progress);
                float exponent = std::pow(2.0f, releaseCurve);
                envelope = std::pow(progress, exponent);

                currentLevel = juce::jmap(envelope, releaseStarLevel, 0.0f);
                envelope = currentLevel;
                currentSample++;

                if (currentSample >= releaseSamples)
                {
                    envelope = 0.0f;
                    currentLevel = 0.0f;
                    stage = IDLE;
                    currentSample = 0;
                }
                break;
            }
            case IDLE:
                envelope = 0.0f;
                break;
        }

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            auto* data = buffer.getWritePointer(ch);
            data[startSample + sp] *= envelope;
        }
    }
}

void ADSRCore::start()
{
    currentSample = 0;
    stage = ATTACK;
}

void ADSRCore::end()
{
    releaseStarLevel = currentLevel;
    currentSample = 0;
    stage = RELEASE;
}

void ADSRCore::setAttack(const float val)
{
    if (attack == val) return;
    attack = val;
    attackSamples = static_cast<int>(val * sampleRate);
}

void ADSRCore::setDecay(const float val)
{
    if (decay == val) return;
    decay = val;
    decaySamples = static_cast<int>(decay * sampleRate);
}

void ADSRCore::setSustain(const float val)
{
    if (sustain == val) return;
    sustain = val;
}

void ADSRCore::setRelease(const float val)
{
    if (release == val) return;
    release = val;
    releaseSamples = static_cast<int>(release * sampleRate);
}

void ADSRCore::setAttackCurve(const float curve)
{
    if (attackCurve == curve) return;
    attackCurve = curve;
}

void ADSRCore::setDecayCurve(const float curve)
{
    if (decayCurve == curve) return;
    decayCurve = curve;
}

void ADSRCore::setReleaseCurve(const float curve)
{
    if (releaseCurve == curve) return;
    releaseCurve = curve;
}

bool ADSRCore::active() const
{
    return stage != IDLE;
}
