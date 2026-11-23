#pragma once
#include <JuceHeader.h>
#include "../../DSP/FilterCore/FilterCoefficients.h"
#include "../Palette.h"

class FilterGraph: public juce::Component
{
public:
    FilterGraph();
    ~FilterGraph();
    void paint(juce::Graphics &g) override;
private:
    void drawGrid(juce::Graphics& g, const juce::Rectangle<float>& graph);
    void drawResponse(juce::Graphics& g, const juce::Rectangle<float>& graph);
    float mapDecibels(float db, const juce::Rectangle<float>& graph);
};