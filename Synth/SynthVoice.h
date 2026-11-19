#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
#include "../DSP/Oscillator.h"
#include "../Resources/ParameterContext.h"
#include "../Resources/ParameterIDs.h"
#include "../Resources/Constants.h"

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

    // Properties
    void alwaysProcess();

private:
    juce::Random random;
    Oscillator osc1[2];
    Oscillator osc2[2];

    float frequency = 440.0f;
    float velocity = 0.0f;

    int totalChannels = 2;

    ParameterContext& params;

    // Wavetable IDs loaders
    int lastWavetableIDOsc1 = -1;
    int lastWavetableIDOsc2 = -1;

    // Wavetables
    Wavetable::Table osc1Wavetable;
    Wavetable::Table osc2Wavetable;

    int lastWaveFrameOsc1 = -1;
    int lastWaveFrameOsc2 = -1;

    // Osc always processor
    void osc1Processor();
    void osc2Processor();
};
