#include "FilterUI.h"

FilterUI::FilterUI(ParameterContext& state)
    : state(state)
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

    addAndMakeVisible(active);
    activeAttachment = std::make_unique<ButtonAttachment>(state.apvts(), ParameterIDs::FILTER_ACTIVE, active);

    addAndMakeVisible(graph);

    active.onStateChange = [this] () { repaint(); };
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
    active.setLookAndFeel(nullptr);
}

void FilterUI::paint(juce::Graphics& g)
{
    g.fillAll(Palette::DARK_1);

    g.setColour(Palette::DARK_2);
    g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(10), 10);

    gain.setValue(juce::String(static_cast<int>(gain.getSlider().getValue())) + "Db");
    freq.setValue(juce::String(static_cast<int>(freq.getSlider().getValue())) + "Hz");
    qua.setValue(juce::String(qua.getSlider().getValue()) + "Db");
    graph.repaint();
}

void FilterUI::paintOverChildren(juce::Graphics& g)
{
    if (active.getToggleState())
        return;

    const auto bounds = getLocalBounds().toFloat();
    const float w = bounds.getWidth();
    const float h = bounds.getHeight();

    constexpr float btnSize = 30.0f;
    constexpr float pad = 10.0f;

    auto hole = active.getBounds().toFloat().expanded(5);

    juce::Path overlay;
    overlay.addRectangle(bounds);
    overlay.addRectangle(hole);

    g.setColour(Palette::DARK_1.withAlpha(0.5f));
    g.fillPath(overlay, juce::AffineTransform());

    g.saveState();
    g.reduceClipRegion(getLocalBounds());
    g.excludeClipRegion(hole.toNearestInt());

    g.setColour(Palette::LIGHT_1.withAlpha(0.25f));

    constexpr float spacing = 18.0f;
    constexpr float thickness = 5.0f;

    for (float x = -h; x < w + h; x += spacing)
        g.drawLine(x, h, x + h, 0.0f, thickness);

    g.restoreState();
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
        juce::GridItem(active).withArea(4, 5, 6, 5),
        juce::GridItem(graph).withArea(1, 1, 6, 4),
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

    active.setLookAndFeel(&iS1);
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
            const int filterId = index - 1;
            const auto filterToRepo = Filter::getFilterById(filterId);

            const auto filterChoose = state.apvts().getParameter(ParameterIDs::FILTER_CHOOSE);
            filterChoose->setValueNotifyingHost(filterChoose->convertTo0to1(filterId));

            filterSelect.setButtonText(filterToRepo->name);
        }
    );
}







