#pragma once
#include <JuceHeader.h>
#include "../Palette.h"
#include "../../Resources/Font.h"

class IStyle_1 : public juce::LookAndFeel_V4
{
public:
    // Slider
    void drawRotarySlider(
    juce::Graphics& g,
    int x, int y,
    int width, int height,
    float sliderPos,
    const float rotaryStartAngle,
    const float rotaryEndAngle,
    juce::Slider&) override
    {
        auto radius = (float) juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float) x + (float) width * 0.5f;
        auto centreY = (float) y + (float) height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // Outer circle
        g.setColour(Palette::LIGHT_3);
        g.drawEllipse(rx, ry, rw, rw, 4.0f);

        // Outer arc
        juce::Path outerPath;
        g.setColour(Palette::DARK_1);
        outerPath.addArc(rx, ry, rw, rw, rotaryStartAngle, angle, true);
        g.strokePath(outerPath, juce::PathStrokeType(4.0f));

        // Inner arc
        auto innerRadius = radius * 0.75f;
        auto innerX = centreX - innerRadius;
        auto innerY = centreY - innerRadius;
        auto innerSize = innerRadius * 2.0f;

        juce::Path innerPath;
        g.setColour(Palette::DARK_3);
        innerPath.addArc(innerX, innerY, innerSize, innerSize, rotaryStartAngle, angle, true);
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

    void drawLinearSlider(
            juce::Graphics& g,
            int x, int y,
            int width, int height,
            float sliderPos,
            float minSliderPos,
            float maxSliderPos,
            const juce::Slider::SliderStyle style,
            juce::Slider& slider) override
    {
        float centreY = (float)y + (float)height * 0.5f;
        float startX = (float)x;
        float endX = (float)x + (float)width;
        float space = 6.0f;

        // Background track
        g.setColour(Palette::LIGHT_2);
        g.drawLine(startX, centreY, endX, centreY, 4.0f);

        g.setColour(Palette::LIGHT_3);
        g.drawLine(startX, centreY - space, endX, centreY - space, 2.0f);
        g.drawLine(startX, centreY + space, endX, centreY + space, 2.0f);

        // Progress track
        g.setColour(Palette::DARK_2);
        g.drawLine(startX, centreY, sliderPos, centreY, 4.0f);

        g.setColour(Palette::DARK_3);
        g.drawLine(startX, centreY - space, sliderPos, centreY - space, 2.0f);
        g.drawLine(startX, centreY + space, sliderPos, centreY + space, 2.0f);

        // Thumb
        float thumbRadius = 3.0f;
        g.setColour(Palette::DARK_1);
        g.fillRect(sliderPos - thumbRadius, centreY - thumbRadius * 3.0f, thumbRadius * 2.0f, thumbRadius * 6.0f);
    }

    void drawButtonBackground(
        juce::Graphics& g, juce::Button& btn,
        const juce::Colour &backgroundColour,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override
    {
        const auto bounds = btn.getLocalBounds().toFloat();
        auto color = Palette::DARK_2;

        if (!btn.isEnabled())
            color = color.withAlpha(0.25f);
        else if (btn.isMouseButtonDown())
            color = color.withAlpha(0.75f);
        else if (btn.isMouseOver())
        {
            color = color.withAlpha(0.5f);
            btn.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        }

        g.setColour(color);
        g.fillRoundedRectangle(bounds, 5);
    }

    void drawButtonText(
        juce::Graphics& g, juce::TextButton& button,
        bool isMouseOverButton,
        bool isButtonDown) override
    {
        g.setColour(Palette::LIGHT_3);
        g.setFont(Font::get(18.0f));
        g.drawFittedText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, 1);
    }
};
