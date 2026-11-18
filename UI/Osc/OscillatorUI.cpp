#include "OscillatorUI.h"
#include "../../Resources/ParameterIDs.h"
#include "../../Resources/Wavetable.h"

OscillatorUI::OscillatorUI(
    ParameterContext& state,
    juce::String gainId, juce::String panId,
    juce::String waveFrameId, juce::String waveSelectorId,
    juce::String tuneId
    )
    : state(state), waveSelectorId(waveSelectorId)
{
    addAndMakeVisible(gain);
    gainAttachment = std::make_unique<SliderAttachment>(state.apvts(), gainId, gain);

    addAndMakeVisible(pan);
    panAttachment = std::make_unique<SliderAttachment>(state.apvts(), panId, pan);

    waveFrameAttachment = std::make_unique<SliderAttachment>(state.apvts(), waveFrameId, waveFrame);
    addAndMakeVisible(waveFrame);

    addAndMakeVisible(wavetables);
    wavetables.onClick = [this] ()
    {
        createWavetableMenu();
    };

    addAndMakeVisible(wV);

    addAndMakeVisible(tune);
    tuneAttachment = std::make_unique<SliderAttachment>(state.apvts(), tuneId, tune);
}

OscillatorUI::~OscillatorUI()
{

}

void OscillatorUI::paint(juce::Graphics& g)
{
    g.fillAll(Palette::DARK_1);
    wV.setFrame(Wavetable::wavetableOsc1);
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

    grid.setGap(juce::Grid::Px(10));

    grid.items = {
        juce::GridItem(gain).withArea(4, 1, 6, 1),
        juce::GridItem(pan).withArea(4, 2, 6, 2),
        juce::GridItem(waveFrame).withArea(4, 3, 6, 3),
        juce::GridItem(wavetables).withArea(1, 4, 1, 7),
        juce::GridItem(wV).withArea(1, 1, 4, 4),
        juce::GridItem(tune).withArea(2, 4, 2, 7)
    };

    grid.performLayout(getLocalBounds().reduced(10));
}

void OscillatorUI::preOptions()
{
    gain.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gain.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, false);

    pan.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    pan.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, false);

    waveFrame.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    waveFrame.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, false);

    tune.setSliderStyle(juce::Slider::LinearHorizontal);
    tune.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, false);
}

void OscillatorUI::createWavetableMenu()
{
    juce::PopupMenu menu;
    juce::PopupMenu basic;
    juce::PopupMenu digital;
    juce::PopupMenu complex;

    for (int i = 0; i < WavetableLibrary::allCount; i++)
    {
        const auto* wt = &WavetableLibrary::all[i];
        const int menuId = wt->id + 1;

        if (wt->category == Category::Basic)
            basic.addItem(menuId, wt->name);

        if (wt->category == Category::Digital)
            digital.addItem(menuId, wt->name);

        if (wt->category == Category::Complex)
            complex.addItem(menuId, wt->name);
    }

    menu.addSubMenu("Basic", basic);
    menu.addSubMenu("Digital", digital);
    menu.addSubMenu("Complex", complex);

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