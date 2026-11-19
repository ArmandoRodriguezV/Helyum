#include "HSliderRotary.h"

HSliderRotary::HSliderRotary()
{
    addAndMakeVisible(values);
    addAndMakeVisible(slider);
    addAndMakeVisible(label);
}

HSliderRotary::~HSliderRotary()
{
    slider.setLookAndFeel(nullptr);
}

void HSliderRotary::paint(juce::Graphics &g)
{
    if (!inverse)
        return;
    const auto bounds = getLocalBounds().toFloat();

    g.setColour(Palette::LIGHT_2);
    g.fillRoundedRectangle(bounds, 10);

}

void HSliderRotary::resized()
{
    configure();
    juce::Grid grid;
    grid.templateRows = {
        juce::Grid::TrackInfo(juce::Grid::Fr(5)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
    };

    grid.templateColumns = {
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
    };

    grid.setGap(juce::Grid::Px(5));

    grid.items = {
        juce::GridItem(slider).withArea(1, 1),
        juce::GridItem(values).withArea(1, 1),
        juce::GridItem(label).withArea(2, 1)
    };

    grid.performLayout(getLocalBounds().reduced(5));
}

void HSliderRotary::setTitle(juce::String val)
{
    label.setText(val, juce::dontSendNotification);
}

void HSliderRotary::setValue(juce::String val)
{
    values.setText(val, juce::dontSendNotification);
}

void HSliderRotary::configure()
{
    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, false);

    label.setFont(Font::get());
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, inverse ? Palette::DARK_1 : Palette::LIGHT_1);

    values.setFont(Font::get(14.0f));
    values.setJustificationType(juce::Justification::centred);
    values.setColour(juce::Label::textColourId, inverse ? Palette::DARK_1 : Palette::LIGHT_1);
}
