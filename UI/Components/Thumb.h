#pragma once
#include <JuceHeader.h>
#include "../Palette.h"

enum ThumbType {
    MAIN,
    SECOND,
};

class Thumb: public juce::Component
{
public:
    Thumb();
    ~Thumb();

    void paint(juce::Graphics &g) override;

    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void setInverse();

    float getCentreX() const;
    float getCentreY() const;

    std::function<void(juce::Point<float>)> onPositionChanged;

    juce::Point<float> getNormalizedPosition() const;

private:
    bool inverse = false;
    juce::Point<int> dragPosition;
    ThumbType type = MAIN;
};