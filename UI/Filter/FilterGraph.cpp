
#include "FilterGraph.h"

FilterGraph::FilterGraph()
{
}

FilterGraph::~FilterGraph()
{
}

void FilterGraph::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    g.setColour(Palette::DARK_3);
    g.fillRoundedRectangle(bounds, 10);

    drawResponse(g, bounds);
}

void FilterGraph::drawResponse(juce::Graphics& g, const juce::Rectangle<float>& graph)
{
    if (FilterCoefficients::coefficients == nullptr)
        return;

    constexpr int points = 512;
    juce::Path response;
    juce::Path fill;

    for (int i = 0; i < points; ++i)
    {
        float normalized = (float)i / (points - 1);

        // Frecuencia logarítmica: 20 Hz - 20 kHz
        float frequency = 20.0f * std::pow(1000.0f, normalized);

        float magnitude = FilterCoefficients::getMagnitude(frequency);
        magnitude = juce::jlimit(-56.0f, 44.0f, magnitude);

        float x = graph.getX() + normalized * graph.getWidth();
        float y = mapDecibels(magnitude, graph);

        if (i == 0)
        {
            response.startNewSubPath(x, y);
            fill.startNewSubPath(x, graph.getCentreY());
            fill.lineTo(x, y);
        }
        else
        {
            response.lineTo(x, y);
            fill.lineTo(x, y);
        }
    }

    fill.lineTo(graph.getRight(), graph.getCentreY());
    fill.closeSubPath();

    // Fill
    g.setColour(Palette::LIGHT_3.withAlpha(0.12f));
    g.fillPath(fill);

    // Response curve
    g.setColour(Palette::LIGHT_1);
    g.strokePath(response, juce::PathStrokeType(2.0f));
}

float FilterGraph::mapDecibels(float db, const juce::Rectangle<float>& graph)
{
    return juce::jmap(db, -60.0f, 48.0f, graph.getBottom(), graph.getY());
}
