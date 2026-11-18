#pragma once
#include <JuceHeader.h>
#include "SynthVoice.h"
#include "SynthSound.h"
#include "../Resources/ParameterContext.h"

class ElyumSynth
{
public:
    ElyumSynth(ParameterContext& context);
    ~ElyumSynth();
    void prepare(double sP);
    void process(juce::AudioBuffer<float>& buffer, const juce::MidiBuffer& midiMessages);

private:
    std::vector<SynthVoice*> voices;
    juce::Synthesiser synth;
};