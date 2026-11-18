#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HelyumAudioProcessorEditor::HelyumAudioProcessorEditor (HelyumAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    osc1(p.context,
        ParameterIDs::OSC1_VOLUME, ParameterIDs::OSC1_PAN,
        ParameterIDs::OSC1_WAVE_FRAME_ID, ParameterIDs::OSC1_WAVE_CHOOSE,
        ParameterIDs::OSC1_TUNE
        )
{
    startTimerHz(60);
    addAndMakeVisible(osc1);
    setSize(1080, 720);
}

HelyumAudioProcessorEditor::~HelyumAudioProcessorEditor()
{
}

//==============================================================================
void HelyumAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(128, 128, 128));
}

void HelyumAudioProcessorEditor::resized()
{
    juce::Grid grid;
    grid.templateColumns = {
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1))
    };

    grid.templateRows = {
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1))
    };

    grid.items = {
        juce::GridItem(osc1).withArea(1, 1, 2, 2),
    };

    grid.performLayout(getLocalBounds());
}

void HelyumAudioProcessorEditor::timerCallback()
{
    osc1.repaint();
}
