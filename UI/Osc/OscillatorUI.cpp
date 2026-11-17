#include "OscillatorUI.h"
#include "../../Resources/ParameterIDs.h"

OscillatorUI::OscillatorUI(
    ParameterContext& state,
    juce::String gainId,
    juce::String waveFrameId
    )
    : state(state)
{
    addAndMakeVisible(gain);
    gainAttachment = std::make_unique<SliderAttachment>(state.apvts(), gainId, gain);

    addAndMakeVisible(wavetables);

    addAndMakeVisible(pan);

    waveFrameAttachment = std::make_unique<SliderAttachment>(state.apvts(), waveFrameId, waveFrame);
    addAndMakeVisible(waveFrame);
}

OscillatorUI::~OscillatorUI()
{

}

void OscillatorUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    auto area = getLocalBounds().reduced(10);
    constexpr int rows = 5;
    constexpr int columns = 6;
    float cellWidth = area.getWidth() / (float)columns;
    float cellHeight = area.getHeight() / (float)rows;
    g.setColour(juce::Colours::white.withAlpha(0.35f));
    for (int row = 0; row < rows; ++row)
    {
        for (int column = 0; column < columns; ++column)
        {
            juce::Rectangle<float> cell(
                area.getX() + column * cellWidth,
                area.getY() + row * cellHeight,
                cellWidth,
                cellHeight
            );
            g.drawRect(cell, 1.0f);
            g.setColour(juce::Colours::white.withAlpha(0.5f));
            g.drawText(
                juce::String(row + 1) + ", " + juce::String(column + 1),
                cell,
                juce::Justification::centred
            );
            g.setColour(juce::Colours::white.withAlpha(0.35f));
        }
    }
}

void OscillatorUI::resized()
{
    preOptions();
    juce::Grid grid;
    grid.templateRows = {
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
    }; // 5
    grid.templateColumns = {
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
    }; // 6

    grid.items = {
        juce::GridItem(gain).withArea(4, 1, 6, 1),
        juce::GridItem(pan).withArea(4, 2, 6, 2),
        juce::GridItem(waveFrame).withArea(4, 3, 6, 3),
    };

    grid.performLayout(getLocalBounds().reduced(10));
}

void OscillatorUI::preOptions()
{
    gain.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gain.setTextBoxStyle(juce::Slider::NoTextBox, 0, 0, false);

    pan.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    pan.setTextBoxStyle(juce::Slider::NoTextBox, 0, 0, false);

    waveFrame.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    waveFrame.setTextBoxStyle(juce::Slider::NoTextBox, 0, 0, false);
}
