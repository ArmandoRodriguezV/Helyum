#pragma once
#include <JuceHeader.h>
#include "../../Pressets/Filter/Filter.h"
#include "../../Resources/ParameterContext.h"
#include "../../Resources/ParameterIDs.h"
#include "../Components/HSliderRotary.h"
#include "../Styles/IStyles_1.h"
#include "../Styles/IStyles_2.h"
#include "../Styles/Styles_1.h"
#include "../Styles/Styles_2.h"

class FilterUI: public juce::Component
{
public:
    FilterUI(ParameterContext& state);
    ~FilterUI();

    void paint(juce::Graphics &g) override;
    void resized() override;

    void setViewerData(
        const double sampleRate,
        const juce::dsp::IIR::Coefficients<float>& newCoefficients,
        const juce::dsp::IIR::Coefficients<float>& newResonanceCoefficients
    );

private:
    ParameterContext& state;
    juce::Grid grid;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    juce::TextButton filterSelect { "Filter" };
    void configureInputs();

    juce::PopupMenu filterMenu, lowMenu, highMenu, notchMenu;
    void loadMenus();

    HSliderRotary gain, freq, qua, res;
    std::unique_ptr<SliderAttachment> gainA, freqA, quaA, resA;

    Style_1 s1;
    Style_2 s2;
    IStyle_1 iS1;
    IStyle_2 iS2;
};