#pragma once
#include <JuceHeader.h>

enum Stage {
    IDLE,
    ATTACK,
    DECAY,
    SUSTAIN,
    RELEASE
};

class ADSRCore
{
public:
    void prepare(const double sampleRate);
    void process(juce::AudioBuffer<float>& buffer, int startSample, int numSamples);

    void start();
    void end();

    // Params
    void setAttack(const float val);
    void setDecay(const float val);
    void setSustain(const float val);
    void setRelease(const float val);

    // Curves
    void setAttackCurve(const float curve);
    void setDecayCurve(const float curve);
    void setReleaseCurve(const float curve);

    bool active() const;

private:
    double sampleRate = 44100.0;

    float attack = 0.0f;
    float attackCurve = 0.0f;
    int attackSamples = 0;

    float decay = 1.0f;
    float decayCurve = 0.0f;
    int decaySamples = 0;

    float sustain = 1.0f;

    float release = 0.25f;
    float releaseCurve = 0.0f;
    int releaseSamples = 0;

    float releaseStarLevel = 0.0f;
    float currentLevel = 0.0;

    int currentSample = 0;
    Stage stage = ATTACK;

};
