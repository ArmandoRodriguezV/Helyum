#pragma once
#include <JuceHeader.h>
#include "../../Resources/ParameterContext.h"

class OscillatorUI: public juce::Component
{
public:
    explicit OscillatorUI(ParameterContext& state);
    ~OscillatorUI();

    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    ParameterContext& state;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    juce::Slider gain;
    std::unique_ptr<SliderAttachment> gainAttachment;

};
