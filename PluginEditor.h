#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/Osc/OscillatorUI.h"

class HelyumAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    HelyumAudioProcessorEditor (HelyumAudioProcessor&);
    ~HelyumAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    OscillatorUI osc1;
    HelyumAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HelyumAudioProcessorEditor)
};
