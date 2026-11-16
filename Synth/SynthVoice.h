#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
#include "../DSP/Oscillator.h"
#include "../Resources/ParameterContext.h"
#include "../Resources/ParameterIDs.h"

class SynthVoice: public juce::SynthesiserVoice
{
public:
    SynthVoice(ParameterContext& params);
    bool canPlaySound(juce::SynthesiserSound *) override;
    void setCurrentPlaybackSampleRate(double newRate) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;

private:
    Oscillator osc1[2];

    float frequency = 440.0f;
    float velocity = 0.0f;

    int totalChannels = 2;

    ParameterContext& params;
};
