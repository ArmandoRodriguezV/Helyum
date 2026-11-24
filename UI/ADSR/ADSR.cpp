#include "ADSR.h"

ADSR::ADSR(ParameterContext& state)
    : state(state), viewer(state)
{
    addAndMakeVisible(viewer);

    addAndMakeVisible(attack);
    attachAttachment = std::make_unique<SliderAttachment>(state.apvts(), ParameterIDs::ADSR_A, attack.getSlider());
    attack.getSlider().onValueChange = [this] () {
        updateSlider();
    };

    addAndMakeVisible(decay);
    decayAttachment = std::make_unique<SliderAttachment>(state.apvts(), ParameterIDs::ADSR_D, decay.getSlider());
    decay.getSlider().onValueChange = [this] () {
        updateSlider();
    };

    addAndMakeVisible(sustain);
    sustainAttachment = std::make_unique<SliderAttachment>(state.apvts(), ParameterIDs::ADSR_S, sustain.getSlider());
    sustain.getSlider().onValueChange = [this] () {
        updateSlider();
    };

    addAndMakeVisible(release);
    releaseAttachment = std::make_unique<SliderAttachment>(state.apvts(), ParameterIDs::ADSR_R, release.getSlider());
    release.getSlider().onValueChange = [this] () {
        updateSlider();
    };

    addAndMakeVisible(label);
}

ADSR::~ADSR()
{
    attack.setLookAndFeel(nullptr);
    decay.setLookAndFeel(nullptr);
    sustain.setLookAndFeel(nullptr);
    release.setLookAndFeel(nullptr);
}

void ADSR::paint(juce::Graphics &g)
{
    g.fillAll(Palette::LIGHT_3);

    const auto attackBounds = attack.getBounds();
    const auto decayBounds = decay.getBounds();

    const auto totalBounds = attackBounds.getUnion(decayBounds).toFloat();
    g.setColour(Palette::DARK_2);
    g.fillRoundedRectangle(totalBounds, 10);
}

void ADSR::resized()
{
    prepareSliders();
    updateSlider();
    juce::Grid grid;
    grid.templateColumns = {
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
    };

    grid.templateRows = {
        juce::Grid::TrackInfo(juce::Grid::Fr(2)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
    };

    grid.setGap(juce::Grid::Px(5));

    grid.items = {
        juce::GridItem(viewer).withArea(1, 1, 1, 6),
        juce::GridItem(attack).withArea(2, 1),
        juce::GridItem(decay).withArea(2, 2),
        juce::GridItem(sustain).withArea(2, 3),
        juce::GridItem(release).withArea(2, 4),
        juce::GridItem(label).withArea(2, 5),
    };

    grid.performLayout(getLocalBounds().reduced(10));
}

void ADSR::prepareSliders()
{
    attack.setLookAndFeel(&s1);
    attack.setTitle("Attack");

    decay.setLookAndFeel(&s1);
    decay.setTitle("Decay");

    sustain.setLookAndFeel(&iS1);
    sustain.setInverseColor(true);
    sustain.setTitle("Sustain");

    release.setLookAndFeel(&iS1);
    release.setInverseColor(true);
    release.setTitle("Release");

    label.setFont(Font::get(24.0f));
    label.setColour(juce::Label::textColourId, Palette::DARK_1);
    label.setJustificationType(juce::Justification::centred);
    label.setText("ADSR", juce::dontSendNotification);
}

void ADSR::updateSlider()
{
    attack.setValue(juce::String(attack.getSlider().getValue()) + "s");
    decay.setValue(juce::String(decay.getSlider().getValue()) + "s");
    sustain.setValue(juce::String(static_cast<int>(sustain.getSlider().getValue() * 100.0f)) + "%");
    release.setValue(juce::String(release.getSlider().getValue()) + "s");

    viewer.setAttackValue(attack.getSlider().getValue());
    viewer.setDecayValue(decay.getSlider().getValue());
    viewer.setSustainValue(sustain.getSlider().getValue());
    viewer.setReleaseValue(release.getSlider().getValue());

    repaint();
}

void ADSR::update()
{
    viewer.repaint();
}
