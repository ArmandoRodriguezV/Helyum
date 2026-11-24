#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HelyumAudioProcessorEditor::HelyumAudioProcessorEditor (HelyumAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    osc1(p.context,
        ParameterIDs::OSC1_ENABLE,
        ParameterIDs::OSC1_VOLUME, ParameterIDs::OSC1_PAN,
        ParameterIDs::OSC1_WAVE_FRAME_ID, ParameterIDs::OSC1_WAVE_CHOOSE,
        ParameterIDs::OSC1_TRANSPOSE, ParameterIDs::OSC1_TUNE,
        ParameterIDs::OSC1_PHASE_MULT
        ),
    osc2(p.context,
        ParameterIDs::OSC2_ENABLE,
        ParameterIDs::OSC2_VOLUME, ParameterIDs::OSC2_PAN,
        ParameterIDs::OSC2_WAVE_FRAME_ID, ParameterIDs::OSC2_WAVE_CHOOSE,
        ParameterIDs::OSC2_TRANSPOSE, ParameterIDs::OSC2_TUNE,
        ParameterIDs::OSC2_PHASE_MULT
        ),
    filter(p.context),
    adsr(p.context)
{
    startTimerHz(50);
    addAndMakeVisible(osc1);
    addAndMakeVisible(osc2);
    addAndMakeVisible(adsr);
    addAndMakeVisible(filter);
    setSize(1200, 800);
}

HelyumAudioProcessorEditor::~HelyumAudioProcessorEditor()
{
}

//==============================================================================
void HelyumAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(100, 100, 128));
}

void HelyumAudioProcessorEditor::resized()
{
    osc1.setTitle("Osc 1");
    osc2.setTitle("Osc 2");

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
        juce::GridItem(osc1).withArea(1, 1),
        juce::GridItem(osc2).withArea(2, 1),
        juce::GridItem(filter).withArea(3, 1),
        juce::GridItem(adsr).withArea(1, 2),
    };

    grid.performLayout(getLocalBounds());
}

void HelyumAudioProcessorEditor::timerCallback()
{
    osc1.setWaveform(Wavetable::wavetableOsc1);
    osc2.setWaveform(Wavetable::wavetableOsc2);
    adsr.update();
}
