#include "FilterUI.h"

FilterUI::FilterUI(ParameterContext& state)
{
    addAndMakeVisible(filterSelect);

    filterSelect.onClick = [this] ()
    {
        loadMenus();
    };

    addAndMakeVisible(gain);
    gainA = std::make_unique<SliderAttachment>(state.apvts(), ParameterIDs::FILTER_GAIN, gain.getSlider());

    addAndMakeVisible(freq);
    freqA = std::make_unique<SliderAttachment>(state.apvts(), ParameterIDs::FILTER_FREQ, freq.getSlider());

    addAndMakeVisible(qua);
    quaA = std::make_unique<SliderAttachment>(state.apvts(), ParameterIDs::FILTER_Q, qua.getSlider());

    addAndMakeVisible(res);
    resA = std::make_unique<SliderAttachment>(state.apvts(), ParameterIDs::FILTER_RESONANCE, res.getSlider());
}

FilterUI::~FilterUI()
{
    filterSelect.setLookAndFeel(nullptr);
    filterMenu.setLookAndFeel(nullptr);
    lowMenu.setLookAndFeel(nullptr);
    highMenu.setLookAndFeel(nullptr);
    notchMenu.setLookAndFeel(nullptr);

    gain.setLookAndFeel(nullptr);
    freq.setLookAndFeel(nullptr);
    qua.setLookAndFeel(nullptr);
    res.setLookAndFeel(nullptr);
}

void FilterUI::paint(juce::Graphics& g)
{
    g.fillAll(Palette::DARK_1);

    g.setColour(Palette::DARK_2);
    g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(10), 10);

    gain.setValue(juce::String(gain.getSlider().getValue()) + "Db");
    freq.setValue(juce::String(freq.getSlider().getValue()));
    qua.setValue(juce::String(qua.getSlider().getValue()) + "Db");
    res.setValue(juce::String(static_cast<int>(res.getSlider().getValue() * 100)) + "%");
}

void FilterUI::resized()
{
    configureInputs();
    grid.templateColumns = {
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1))
    };

    grid.templateRows = {
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1))
    };

    grid.setGap(juce::Grid::Px(5));

    grid.items = {
        juce::GridItem(filterSelect).withArea(1, 4, 1, 6),
        juce::GridItem(gain).withArea(2, 4, 4, 4),
        juce::GridItem(freq).withArea(2, 5, 4, 5),
        juce::GridItem(qua).withArea(4, 4, 6, 4),
        juce::GridItem(res).withArea(4, 5, 6, 5),
    };

    grid.performLayout(getLocalBounds().reduced(10));
}

void FilterUI::configureInputs()
{
    filterSelect.setLookAndFeel(&s1);
    filterMenu.setLookAndFeel(&s1);
    lowMenu.setLookAndFeel(&s1);
    highMenu.setLookAndFeel(&s1);
    notchMenu.setLookAndFeel(&s1);

    gain.setTitle("Gain");
    gain.setLookAndFeel(&s2);

    freq.setTitle("Frequency");
    freq.setLookAndFeel(&s1);

    qua.setTitle("Q");
    qua.setInverseColor(true);
    qua.setLookAndFeel(&iS1);

    res.setTitle("Resonance");
    res.setInverseColor(true);
    res.setLookAndFeel(&iS1);
}

void FilterUI::loadMenus()
{
    filterMenu.clear();
    lowMenu.clear();
    highMenu.clear();
    notchMenu.clear();

    for (int i = 0; i < Filter::allCount; i++)
    {
        const auto* filter = &Filter::all[i];
        const int id = filter->id + 1;
        if (filter->category == FilterCategory::LowPass)
            lowMenu.addItem(id, filter->name);
        else if (filter->category == FilterCategory::HighPass)
            highMenu.addItem(id, filter->name);
        else if (filter->category == FilterCategory::Notch)
            notchMenu.addItem(id, filter->name);
    }

    filterMenu.addSubMenu("Low-Pass", lowMenu);
    filterMenu.addSubMenu("High-Pass", highMenu);
    filterMenu.addSubMenu("Notch", notchMenu);

    filterMenu.showMenuAsync(
        juce::PopupMenu::Options().withTargetComponent(&filterSelect),
        [this] (const int index) {

        }
    );
}















