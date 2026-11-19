#pragma once
#include <JuceHeader.h>
#include "../Palette.h"
#include "../../Resources/Font.h"
#include "../../Resources/Constants.h"

class Style_1 : public juce::LookAndFeel_V4
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
        g.setColour(Palette::DARK_3);
        g.drawEllipse(rx, ry, rw, rw, 4.0f);

        // Outer arc
        juce::Path outerPath;
        g.setColour(Palette::LIGHT_1);
        outerPath.addArc(rx, ry, rw, rw, rotaryStartAngle, angle, true);
        g.strokePath(outerPath, juce::PathStrokeType(4.0f));

        // Inner arc
        auto innerRadius = radius * 0.75f;
        auto innerX = centreX - innerRadius;
        auto innerY = centreY - innerRadius;
        auto innerSize = innerRadius * 2.0f;

        juce::Path innerPath;
        g.setColour(Palette::LIGHT_3);
        innerPath.addArc(innerX, innerY, innerSize, innerSize, rotaryStartAngle, angle, true);
        g.strokePath(innerPath, juce::PathStrokeType(2.0f));

        // Thumb
        auto thumbInnerRadius = radius * 0.75f - 1.0f;
        auto thumbOuterRadius = radius + 5.0f;

        auto thumbStartX = centreX + std::sin(angle) * thumbInnerRadius;
        auto thumbStartY = centreY - std::cos(angle) * thumbInnerRadius;

        auto thumbEndX = centreX + std::sin(angle) * thumbOuterRadius;
        auto thumbEndY = centreY - std::cos(angle) * thumbOuterRadius;

        g.setColour(Palette::LIGHT_1);
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
        g.setColour(Palette::DARK_2);
        g.drawLine(startX, centreY, endX, centreY, 4.0f);

        g.setColour(Palette::DARK_3);
        g.drawLine(startX, centreY - space, endX, centreY - space, 2.0f);
        g.drawLine(startX, centreY + space, endX, centreY + space, 2.0f);

        // Progress track
        g.setColour(Palette::LIGHT_2);
        g.drawLine(startX, centreY, sliderPos, centreY, 4.0f);

        g.setColour(Palette::LIGHT_3);
        g.drawLine(startX, centreY - space, sliderPos, centreY - space, 2.0f);
        g.drawLine(startX, centreY + space, sliderPos, centreY + space, 2.0f);

        // Thumb
        float thumbRadius = 3.0f;
        g.setColour(Palette::LIGHT_1);
        g.fillRect(sliderPos - thumbRadius, centreY - thumbRadius * 3.0f, thumbRadius * 2.0f, thumbRadius * 6.0f);
    }

    void drawButtonBackground(
        juce::Graphics& g, juce::Button& btn,
        const juce::Colour &backgroundColour,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override
    {
        const auto bounds = btn.getLocalBounds().toFloat();
        auto color = Palette::LIGHT_2;

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
        g.setColour(Palette::DARK_3);
        g.setFont(Font::get(18.0f));
        g.drawFittedText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, 1);
    }

    void drawToggleButton(
        juce::Graphics& g, juce::ToggleButton& btn,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override
    {
        const auto bounds = btn.getLocalBounds().toFloat();
        bool toggled = btn.getToggleState();
        juce::Colour backgroundColor;
        juce::Colour iconColor;

        if (!toggled)
        {
            backgroundColor = Palette::DARK_3;
            iconColor = Palette::DARK_2;
        }
        else
        {
            backgroundColor = Palette::LIGHT_3;
            iconColor = Palette::DARK_3;
        }

        g.setColour(backgroundColor);
        g.fillRoundedRectangle(bounds, 5);

        const float size = juce::jmin(bounds.getWidth(), bounds.getHeight());
        const float rad = size * 0.5f;
        const float cx = bounds.getCentreX() - (rad * 0.5f);
        const float cy = bounds.getCentreY() - (rad * 0.5f);

        g.setColour(iconColor);
        juce::Path p;
        p.addArc(cx, cy, rad, rad, Constants::TAU * 0.125f, Constants::TAU * 0.875f, true);
        g.strokePath(p, juce::PathStrokeType(3.0f));

        g.drawLine(size * 0.5f, 5.0f, size * 0.5f, size * 0.5f, 3.0f);

    }

    // PopupMenus
    void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override
    {
        g.fillAll(Palette::LIGHT_3);
    }

    void drawPopupMenuItem(
    juce::Graphics& g,
    const juce::Rectangle<int>& area,
    bool isSeparator,
    bool isActive,
    bool isHighlighted,
    bool isTicked,
    bool hasSubMenu,
    const juce::String& text,
    const juce::String& shortcutKeyText,
    const juce::Drawable* icon,
    const juce::Colour* textColour) override
    {
        auto bounds = area.toFloat().reduced(3.0f, 1.0f);

        if (isSeparator)
        {
            g.setColour(Palette::DARK_2);
            g.fillRect(bounds.withHeight(1.0f).withY(bounds.getCentreY()));
            return;
        }

        // Highlight
        if (isHighlighted && isActive)
        {
            g.setColour(Palette::LIGHT_2);
            g.fillRoundedRectangle(bounds, 4.0f);
        }

        // Text
        g.setColour(isActive ? Palette::DARK_3 : Palette::DARK_2.withAlpha(0.4f));
        g.setFont(Font::get(18.0f));

        auto textArea = area.reduced(12, 0);

        if (isTicked)
        {
            g.drawText("✓", textArea.removeFromLeft(20), juce::Justification::centredLeft);
        }
        else
        {
            textArea.removeFromLeft(20);
        }

        if (hasSubMenu)
        {
            auto arrowArea = textArea.removeFromRight(20);
            juce::Path arrow;
            arrow.addTriangle(
                arrowArea.getX() + 5.0f, arrowArea.getCentreY() - 4.0f,
                arrowArea.getX() + 5.0f, arrowArea.getCentreY() + 4.0f,
                arrowArea.getX() + 11.0f, arrowArea.getCentreY()
            );
            g.fillPath(arrow);
        }

        g.drawFittedText(text, textArea, juce::Justification::centredLeft, 1);

        // Shortcut
        if (shortcutKeyText.isNotEmpty())
        {
            g.setColour(Palette::DARK_2.withAlpha(0.6f));
            g.drawText(shortcutKeyText, area.reduced(12, 0), juce::Justification::centredRight);
        }
    }

    int getPopupMenuBorderSize() override
    {
        return 2;
    }

    juce::Font getPopupMenuFont() override
    {
        return Font::get(18.0f);
    }
};

















