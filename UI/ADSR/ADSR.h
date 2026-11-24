#pragma once
#include <JuceHeader.h>
#include "../../Resources/ParameterContext.h"
#include "../../Resources/ParameterIDs.h"
#include "../Components/HSliderRotary.h"
#include "../Palette.h"
#include "../../Resources/Font.h"
#include "../Styles/Styles_1.h"
#include "../Styles/IStyles_1.h"
#include "ADSRViewer.h"

class ADSR: public juce::Component
{
public:
    ADSR(ParameterContext& state);
    ~ADSR();

    void paint(juce::Graphics &g) override;
    void resized() override;
    void update();

private:
    ParameterContext& state;
    void prepareSliders();
    void updateSlider();
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    HSliderRotary attack, decay, sustain, release;
    juce::Label label;

    std::unique_ptr<SliderAttachment>
        attachAttachment, decayAttachment, sustainAttachment, releaseAttachment;

    ADSRViewer viewer;

    Style_1 s1;
    IStyle_1 iS1;
};