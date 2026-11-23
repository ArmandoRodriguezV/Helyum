#pragma once
#include <JuceHeader.h>
#include "../../Pressets/Filter/Filter.h"
#include "../../Resources/ParameterContext.h"
#include "../../Resources/ParameterIDs.h"
#include "FilterGraph.h"
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

    void paintOverChildren(juce::Graphics &g) override;
    void paint(juce::Graphics &g) override;
    void resized() override;

private:
    ParameterContext& state;
    juce::Grid grid;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    juce::TextButton filterSelect { "Filter" };
    void configureInputs();

    juce::PopupMenu filterMenu, lowMenu, highMenu, notchMenu;
    void loadMenus();

    juce::ToggleButton active;

    HSliderRotary gain, freq, qua;
    std::unique_ptr<SliderAttachment> gainA, freqA, quaA;
    std::unique_ptr<ButtonAttachment> activeAttachment;

    Style_1 s1;
    Style_2 s2;
    IStyle_1 iS1;
    IStyle_2 iS2;

    FilterGraph graph;

};