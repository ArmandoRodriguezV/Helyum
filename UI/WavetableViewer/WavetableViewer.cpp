#include "WavetableViewer.h"

WavetableViewer::WavetableViewer()
{

}

WavetableViewer::~WavetableViewer()
{
}

void WavetableViewer::paint(juce::Graphics& g)
{

    const float h = getHeight();
    const float w = getWidth();
    const float halfH = h * 0.5f;

    g.setColour(Palette::DARK_2);
    g.fillRoundedRectangle(0, 0, w, h, 10);

    if (frame.size() < 2)
        return;

    juce::Path path;

    for (int i = 0; i < frame.size(); ++i)
    {
        const float x = static_cast<float>(i) / (frame.size() - 1) * w;
        const float y = halfH + (frame[i] * halfH * 0.85f);

        if (i == 0)
        {
            path.startNewSubPath(x, halfH);
            path.lineTo(x, halfH);
        }
        else
        {
            path.lineTo(x, y);
        }
    }

    path.lineTo(w, halfH);
    path.closeSubPath();

    g.setColour(Palette::BLUE_1);
    g.strokePath(path, juce::PathStrokeType(1.5f));

    g.setColour(Palette::BLUE_2.withAlpha(0.35f));
    g.fillPath(path);
}

void WavetableViewer::resized()
{

}

void WavetableViewer::setFrame(const Wavetable::Frame& fm)
{
    frame = fm;
}
