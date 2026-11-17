#pragma once
#include <JuceHeader.h>
#include "../../Resources/ParameterContext.h"

class OscillatorUI: public juce::Component
{
public:
    explicit OscillatorUI(
        ParameterContext& state,
        juce::String gainId,
        juce::String waveFrameId
        );
    ~OscillatorUI();

    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    ParameterContext& state;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    juce::ComboBox wavetables;

    juce::Slider gain, pan, waveFrame;
    std::unique_ptr<SliderAttachment> gainAttachment, waveFrameAttachment;

    void preOptions();
};
