#pragma once
#include <JuceHeader.h>
#include "../Palette.h"
#include "../../Resources/ParameterContext.h"
#include "../../Resources/ParameterIDs.h"
#include "../../Resources/Font.h"
#include "../Components/Thumb.h"

class ADSRViewer: public juce::Component
{
public:
    ADSRViewer(ParameterContext& state);
    ~ADSRViewer();

    void paint(juce::Graphics &g) override;
    void resized() override;

    void setAttackValue(const float val);
    void setDecayValue(const float val);
    void setSustainValue(const float val);
    void setReleaseValue(const float val);

    void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;
private:
    ParameterContext& state;
    float zoom = 3.0f;

    // Setters
    float attack = 0.0f;
    float decay = 0.0f;
    float sustain = 0.0f;
    float release = 0.0f;

    // Coords
    float attackX = 0.0f;
    float decayX = 0.0f;
    float releaseX = 0.0f;

    Thumb attackThumb;
    Thumb decayThumb;
    Thumb releaseThumb;

    bool updatingFromParam = false;

    void updateLayoutValues();
    void updateAttackCurveLimit();
    void updateDecayCurveLimit();
    void updateReleaseCurveLimit();

    void drawTimeRuler(juce::Graphics& g, const juce::Rectangle<float>& bounds) const;
};