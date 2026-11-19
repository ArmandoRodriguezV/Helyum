#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/Osc/OscillatorUI.h"

class HelyumAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    HelyumAudioProcessorEditor (HelyumAudioProcessor&);
    ~HelyumAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    OscillatorUI osc1, osc2;
    HelyumAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HelyumAudioProcessorEditor)
};
