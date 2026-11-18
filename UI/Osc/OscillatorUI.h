#pragma once
#include <JuceHeader.h>
#include "../../Resources/ParameterContext.h"
#include "../../Pressets/Dictionary/Dictionary.h"
#include "../Palette.h"
#include "../WavetableViewer/WavetableViewer.h"

class OscillatorUI: public juce::Component
{
public:
    explicit OscillatorUI(
        ParameterContext& state,
        juce::String gainId, juce::String panId,
        juce::String waveFrameId, juce::String waveSelectorId,
        juce::String tuneId
        );
    ~OscillatorUI();

    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    ParameterContext& state;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    juce::TextButton wavetables { "Wavetable" };
    juce::PopupMenu basic, digital;

    juce::Slider
        gain, pan,
        waveFrame, tune;

    std::unique_ptr<SliderAttachment>
        gainAttachment, panAttachment,
        waveFrameAttachment, tuneAttachment;

    juce::String waveSelectorId;

    void preOptions();
    void createWavetableMenu();
    WavetableViewer wV;
};
