#include "OscillatorUI.h"
#include "../../Resources/ParameterIDs.h"

OscillatorUI::OscillatorUI(ParameterContext& state)
    : state(state)
{
    addAndMakeVisible(gain);
    gainAttachment = std::make_unique<SliderAttachment>(state.apvts(), ParameterIDs::OSC1_VOLUME, gain);
}

OscillatorUI::~OscillatorUI()
{

}

void OscillatorUI::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);

}

void OscillatorUI::resized()
{
    const auto w = getWidth();
    const auto h = getHeight();

    gain.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gain.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    gain.setBounds(10, 10, 60, 60);
}
