#pragma once
#include <JuceHeader.h>
#include "../../Resources/Wavetable.h"
#include "../Palette.h"

class WavetableViewer: public juce::Component
{
public:
    WavetableViewer();
    ~WavetableViewer();
    void paint(juce::Graphics &g) override;
    void resized() override;

    void setFrame(const Wavetable::Frame &fm);
private:
    Wavetable::Frame frame;
};