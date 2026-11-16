#pragma once
#include <JuceHeader.h>

class SynthSound: public juce::SynthesiserSound
{
public:
    bool appliesToChannel(int midiChannel) override;
    bool appliesToNote(int midiNoteNumber) override;

};