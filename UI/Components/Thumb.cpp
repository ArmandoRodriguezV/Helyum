#include "Thumb.h"

Thumb::Thumb()
{
}

Thumb::~Thumb()
{
}

void Thumb::paint(juce::Graphics &g)
{
    const auto w = getWidth();
    const auto h = getHeight();
    const auto fr = juce::jmin(w, h) * 0.25f;
    const auto bounds = getLocalBounds().toFloat();

    g.setColour(inverse ? Palette::DARK_1 : Palette::LIGHT_1);

    if (type == MAIN)
    {
        g.fillEllipse(bounds.reduced(fr));
        g.drawEllipse(bounds.reduced(2), 4.0f);
    }

    if (type == SECOND)
        g.fillEllipse(bounds);

    if (isMouseOver())
        setMouseCursor(juce::MouseCursor::PointingHandCursor);

}

void Thumb::mouseDown(const juce::MouseEvent &event)
{
    dragPosition = getPosition();
}

void Thumb::mouseDrag(const juce::MouseEvent& event)
{
    const auto delta = event.getOffsetFromDragStart();
    const auto* parent = getParentComponent();

    if (parent == nullptr)
        return;

    const auto bounds = parent->getLocalBounds();

    const int halfW = getWidth() / 2;
    const int halfH = getHeight() / 2;

    const int x = juce::jlimit(0, bounds.getWidth(), dragPosition.x + delta.x + halfW);
    const int y = juce::jlimit(0, bounds.getHeight(), dragPosition.y + delta.y + halfH);

    setCentrePosition(x, y);

    if (onPositionChanged)
        onPositionChanged(getNormalizedPosition());
}

void Thumb::setInverse()
{
    inverse = true;
}

float Thumb::getCentreX() const
{
    return getX() + (getWidth() * 0.5f);
}

float Thumb::getCentreY() const
{
    return getY() + (getHeight() * 0.5f);
}

juce::Point<float> Thumb::getNormalizedPosition() const
{
    const auto* parent = getParentComponent();

    if (parent == nullptr)
        return { 0.0f, 0.0f };

    const auto bounds = parent->getLocalBounds();
    const auto position = getBounds().getCentre();

    const float x = static_cast<float>(position.x) / bounds.getWidth();
    const float y = static_cast<float>(position.y) / bounds.getHeight();

    return {
        juce::jlimit(0.0f, 1.0f, x),
        juce::jlimit(0.0f, 1.0f, y)
    };
}