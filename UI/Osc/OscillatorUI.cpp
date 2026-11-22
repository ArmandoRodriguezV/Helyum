#include "OscillatorUI.h"
#include "../../Resources/ParameterIDs.h"
#include "../../Resources/Wavetable.h"

OscillatorUI::OscillatorUI(
    ParameterContext& state,
    juce::String enableId,
    juce::String gainId, juce::String panId,
    juce::String waveFrameId, const juce::String &waveSelectorId,
    juce::String transposeId, juce::String tuneId,
    juce::String phaseMultId
    )
    : state(state), waveSelectorId(waveSelectorId)
{
    addAndMakeVisible(label);

    addAndMakeVisible(enable);
    enableAttachment = std::make_unique<ButtonAttachment>(state.apvts(), enableId, enable);

    addAndMakeVisible(gain);
    gainAttachment = std::make_unique<SliderAttachment>(state.apvts(), gainId, gain.getSlider());

    addAndMakeVisible(pan);
    panAttachment = std::make_unique<SliderAttachment>(state.apvts(), panId, pan.getSlider());

    addAndMakeVisible(waveFrame);
    waveFrameAttachment = std::make_unique<SliderAttachment>(state.apvts(), waveFrameId, waveFrame.getSlider());

    addAndMakeVisible(wavetables);
    wavetables.onClick = [this] () { createWavetableMenu(); };

    addAndMakeVisible(wV);

    addAndMakeVisible(transpose);
    transposeAttachment = std::make_unique<SliderAttachment>(state.apvts(), transposeId, transpose.getSlider());

    addAndMakeVisible(tune);
    tuneAttachment = std::make_unique<SliderAttachment>(state.apvts(), tuneId, tune.getSlider());

    addAndMakeVisible(phaseMult);
    phaseMultAttachment = std::make_unique<SliderAttachment>(state.apvts(), phaseMultId, phaseMult.getSlider());

    // Effects
    addAndMakeVisible(effects);
    effects.onClick = [this] () { createEffectMenu(); };

    addAndMakeVisible(effectPor);
}

OscillatorUI::~OscillatorUI()
{
    enable.setLookAndFeel(nullptr);
    gain.setLookAndFeel(nullptr);
    pan.setLookAndFeel(nullptr);
    wavetables.setLookAndFeel(nullptr);
    transpose.setLookAndFeel(nullptr);
    tune.setLookAndFeel(nullptr);
    phaseMult.setLookAndFeel(nullptr);
    effects.setLookAndFeel(nullptr);
    effectPor.setLookAndFeel(nullptr);
    effectMenu.setLookAndFeel(nullptr);

    // Menus
    menu.setLookAndFeel(nullptr);
    basicW.setLookAndFeel(nullptr);
    digitalW.setLookAndFeel(nullptr);
    complexW.setLookAndFeel(nullptr);
}

void OscillatorUI::paint(juce::Graphics& g)
{
    const float w = getWidth();
    const float h = getHeight();
    g.fillAll(Palette::DARK_1);

    gain.setValue(juce::String(std::trunc(gain.getSlider().getValue() * 10.0f) / 10.0f) + "Db");
    pan.setValue(juce::String(std::trunc(pan.getSlider().getValue() * 10.0f) / 10.0f));
    waveFrame.setValue(juce::String(static_cast<int>(waveFrame.getSlider().getValue() * 100)) + "%");
    transpose.setValue(juce::String(transpose.getSlider().getValue()));
    tune.setValue(juce::String(tune.getSlider().getValue()));
    phaseMult.setValue(juce::String(static_cast<int>(phaseMult.getSlider().getValue() * 360)) + "°");

    g.setColour(Palette::DARK_2);
    g.fillRoundedRectangle(10.0f, 10.0f, w - 20.0f, h - 20.0f, 10);

    const auto gainBounds = gain.getBounds().toFloat();
    const auto panBounds = pan.getBounds().toFloat();

    g.setColour(Palette::LIGHT_2);
    g.fillRoundedRectangle(gainBounds.getUnion(panBounds), 10);

    const auto effectBounds = effectPor.getBounds().toFloat();
    const auto labelBounds = label.getBounds().toFloat();

    g.fillRoundedRectangle(effectBounds.getUnion(labelBounds), 10);
}

void OscillatorUI::paintOverChildren(juce::Graphics& g)
{
    if (enable.getToggleState())
        return;

    const auto bounds = getLocalBounds().toFloat();
    const float w = bounds.getWidth();
    const float h = bounds.getHeight();

    constexpr float btnSize = 30.0f;
    constexpr float pad = 10.0f;

    auto hole = enable.getBounds().toFloat().expanded(5);

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

    grid.setGap(juce::Grid::Px(5));

    grid.items = {
        juce::GridItem(enable).withArea(4, 4, 6, 4),
        juce::GridItem(gain).withArea(4, 1, 6, 1),
        juce::GridItem(pan).withArea(4, 2, 6, 2),
        juce::GridItem(waveFrame).withArea(4, 3, 6, 3),
        juce::GridItem(wavetables).withArea(1, 4, 1, 7),
        juce::GridItem(wV).withArea(1, 1, 4, 4),
        juce::GridItem(transpose).withArea(2, 4, 4, 4),
        juce::GridItem(tune).withArea(2, 5, 4, 5),
        juce::GridItem(phaseMult).withArea(2, 6, 4, 6),
        juce::GridItem(label).withArea(4, 6),
        juce::GridItem(effects).withArea(5, 6),
        juce::GridItem(effectPor).withArea(4, 5, 6, 5),
    };

    grid.performLayout(getLocalBounds().reduced(10));
}

void OscillatorUI::setTitle(const juce::String val) {
    label.setText(val, juce::dontSendNotification);
}

void OscillatorUI::preOptions()
{
    label.setColour(juce::Label::textColourId, Palette::DARK_3);
    label.setFont(Font::get(24.0f));
    label.setJustificationType(juce::Justification::centred);

    enable.setLookAndFeel(&s1);

    gain.setTitle("Gain");
    gain.setInverseColor(true);
    gain.setLookAndFeel(&iS1);

    pan.setTitle("Pan");
    pan.setInverseColor(true);
    pan.setLookAndFeel(&iS2);

    waveFrame.setTitle("Wavetable");
    waveFrame.setLookAndFeel(&s1);

    transpose.setTitle("Transpose");
    transpose.setLookAndFeel(&s2);

    tune.setTitle("Tune");
    tune.setLookAndFeel(&s2);

    wavetables.setLookAndFeel(&s1);

    phaseMult.setTitle("Phase");
    phaseMult.setInverseColor(true);
    phaseMult.setLookAndFeel(&iS1);

    menu.setLookAndFeel(&s1);
    basicW.setLookAndFeel(&s1);
    digitalW.setLookAndFeel(&s1);
    complexW.setLookAndFeel(&s1);
    effectMenu.setLookAndFeel(&s1);

    effects.setLookAndFeel(&iS1);

    effectPor.setTitle("Effect");
    effectPor.setInverseColor(true);
    effectPor.setLookAndFeel(&iS1);
}

void OscillatorUI::createWavetableMenu() {
    menu.clear();
    basicW.clear();
    digitalW.clear();
    complexW.clear();

    for (int i = 0; i < WavetableLibrary::allCount; i++)
    {
        const auto* wt = &WavetableLibrary::all[i];
        const int menuId = wt->id + 1;

        if (wt->category == Category::Basic)
            basicW.addItem(menuId, wt->name);

        if (wt->category == Category::Digital)
            digitalW.addItem(menuId, wt->name);

        if (wt->category == Category::Complex)
            complexW.addItem(menuId, wt->name);
    }

    menu.addSubMenu("Basic", basicW);
    menu.addSubMenu("Digital", digitalW);
    menu.addSubMenu("Complex", complexW);

    menu.showMenuAsync(
        juce::PopupMenu::Options().withTargetComponent(&wavetables),
        [this](const int result)
        {
            if (result <= 0)
                return;

            const int selectedId = result - 1;
            const auto* wt = WavetableLibrary::find(selectedId);

            if (wt == nullptr)
                return;

            wavetables.setButtonText(wt->name);

            if (auto* param = dynamic_cast<juce::AudioParameterChoice*>(
                state.apvts().getParameter(waveSelectorId)))
            {
                const float normalized = param->convertTo0to1(
                    static_cast<float>(selectedId)
                );

                param->setValueNotifyingHost(normalized);
            }
        }
    );
}

void OscillatorUI::createEffectMenu()
{
    effectMenu.clear();
    effectMenu.addItem(1, "FM from");
    effectMenu.addItem(2, "AM from");

    effectMenu.showMenuAsync(
        juce::PopupMenu::Options().withTargetComponent(&effects),
        [this] (const int result) {

        }
    );
}


void OscillatorUI::setWaveform(const Wavetable::Frame& frame)
{
    wV.setFrame(frame);
    repaint();
}
