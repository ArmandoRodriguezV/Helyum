
#pragma once
#include <JuceHeader.h>
#include "../Palette.h"

class IStyle_2 : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(
        juce::Graphics& g,
        int x, int y,
        int width, int height,
        float sliderPos,
        const float rotaryStartAngle,
        const float rotaryEndAngle,
        juce::Slider&) override
    {
        auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;

        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;

        // Center position
        auto centrePos = 0.5f;

        // Calculate angle
        auto centreAngle = rotaryStartAngle + centrePos * (rotaryEndAngle - rotaryStartAngle);
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // Outer circle
        g.setColour(Palette::LIGHT_3);
        g.drawEllipse(rx, ry, rw, rw, 4.0f);

        // Outer arc
        juce::Path outerPath;
        g.setColour(Palette::DARK_1);

        if (sliderPos >= centrePos)
            outerPath.addArc(rx, ry, rw, rw, centreAngle, angle, true);
        else
            outerPath.addArc(rx, ry, rw, rw, angle, centreAngle, true);

        g.strokePath(outerPath, juce::PathStrokeType(4.0f));

        // Inner arc
        auto innerRadius = radius * 0.75f;
        auto innerX = centreX - innerRadius;
        auto innerY = centreY - innerRadius;
        auto innerSize = innerRadius * 2.0f;

        juce::Path innerPath;
        g.setColour(Palette::DARK_3);

        if (sliderPos >= centrePos)
            innerPath.addArc(innerX, innerY, innerSize, innerSize, centreAngle, angle, true);
        else
            innerPath.addArc(innerX, innerY, innerSize, innerSize, angle, centreAngle, true);

        g.strokePath(innerPath, juce::PathStrokeType(2.0f));

        // Thumb
        auto thumbInnerRadius = radius * 0.75f - 1.0f;
        auto thumbOuterRadius = radius + 5.0f;

        auto thumbStartX = centreX + std::sin(angle) * thumbInnerRadius;
        auto thumbStartY = centreY - std::cos(angle) * thumbInnerRadius;

        auto thumbEndX = centreX + std::sin(angle) * thumbOuterRadius;
        auto thumbEndY = centreY - std::cos(angle) * thumbOuterRadius;

        g.setColour(Palette::DARK_1);
        g.drawLine(thumbStartX, thumbStartY, thumbEndX, thumbEndY, 3.0f);
    }
};
