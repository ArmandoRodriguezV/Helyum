#pragma once
#include <JuceHeader.h>
#include "../Styles/Styles_1.h"
#include "../../Resources/Font.h"

class HSliderRotary: public juce::Component
{
public:
    HSliderRotary();
    ~HSliderRotary();

    void resized() override;
    void paint(juce::Graphics &g) override;

    void setTitle(juce::String val);
    void setValue(juce::String val);
    void setInverseColor(const bool inv) { inverse = inv; }

    juce::Slider& getSlider() { return slider; }

private:
    juce::Slider slider;
    juce::Label label, values;
    bool inverse = false;

    void configure();
};