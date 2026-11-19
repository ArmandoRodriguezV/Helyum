#pragma once
#include <JuceHeader.h>
#include "../../Resources/ParameterContext.h"
#include "../../Resources/Wavetable.h"
#include "../../Resources/Font.h"
#include "../../Pressets/Dictionary/Dictionary.h"
#include "../Palette.h"
#include "../WavetableViewer/WavetableViewer.h"
#include "../Styles/Styles_1.h"
#include "../Styles/Styles_2.h"
#include "../Styles/IStyles_1.h"
#include "../Components/HSliderRotary.h"

class OscillatorUI: public juce::Component
{
public:
    explicit OscillatorUI(
        ParameterContext& state,
        juce::String enableId,
        juce::String gainId, juce::String panId,
        juce::String waveFrameId, const juce::String &waveSelectorId,
        juce::String transposeId, juce::String tuneId,
        juce::String phaseMult
        );
    ~OscillatorUI();

    void paint(juce::Graphics& g) override;
    void paintOverChildren(juce::Graphics &g) override;
    void resized() override;

    void setWaveform(const Wavetable::Frame& frame);
    void setTitle(const juce::String val);

private:
    ParameterContext& state;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    juce::TextButton wavetables { "Wavetable" };
    juce::Label label;
    juce::ToggleButton enable;

    HSliderRotary gain, pan,
        waveFrame, transpose, tune,
        phaseMult;

    juce::PopupMenu menu, basicW, digitalW, complexW;

    std::unique_ptr<SliderAttachment>
        gainAttachment, panAttachment,
        waveFrameAttachment, transposeAttachment, tuneAttachment,
        phaseMultAttachment;

    std::unique_ptr<ButtonAttachment> enableAttachment;

    juce::String waveSelectorId;

    void preOptions();
    void createWavetableMenu();
    WavetableViewer wV;

    Style_1 s1;
    Style_2 s2;
    IStyle_1 iS1;
};
