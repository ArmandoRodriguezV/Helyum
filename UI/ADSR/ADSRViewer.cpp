#include "ADSRViewer.h"

ADSRViewer::ADSRViewer(ParameterContext& state)
    : state(state)
{
    // ---- Attack ----
    addAndMakeVisible(attackThumb);
    attackThumb.onPositionChanged = [this](juce::Point<float> pos)
    {
        if (updatingFromParam)
            return;

        auto* param = this->state.apvts().getParameter(ParameterIDs::ADSR_A_CURVE);
        if (param == nullptr)
            return;

        param->setValueNotifyingHost(juce::jlimit(0.0f, 1.0f, pos.getY()));
        repaint();
    };

    // ---- Decay ----
    addAndMakeVisible(decayThumb);
    decayThumb.onPositionChanged = [this](juce::Point<float> pos)
    {
        if (updatingFromParam)
            return;

        auto* param = this->state.apvts().getParameter(ParameterIDs::ADSR_D_CURVE);
        if (param == nullptr)
            return;

        const float sustainY = 1.0f - sustain;
        const float limitedY = juce::jlimit(0.0f, sustainY, pos.getY());
        const float curveValue = 1.0f - (limitedY / juce::jmax(sustainY, 0.0001f));

        param->setValueNotifyingHost(juce::jlimit(0.0f, 1.0f, curveValue));
        repaint();
    };

    // ---- Release ----
    addAndMakeVisible(releaseThumb);
    releaseThumb.onPositionChanged = [this](juce::Point<float> pos)
    {
        if (updatingFromParam)
            return;

        auto* param = this->state.apvts().getParameter(ParameterIDs::ADSR_R_CURVE);
        if (param == nullptr)
            return;

        const float sustainY = 1.0f - sustain;
        const float range = 1.0f - sustainY;
        const float limitedY = juce::jlimit(sustainY, 1.0f, pos.getY());
        const float curveValue = 1.0f - (limitedY - sustainY) / juce::jmax(range, 0.0001f);

        param->setValueNotifyingHost(juce::jlimit(0.0f, 1.0f, curveValue));
        repaint();
    };
}

ADSRViewer::~ADSRViewer()
{
}

void ADSRViewer::paint(juce::Graphics& g)
{
    updateLayoutValues();

    const auto bounds = getLocalBounds().toFloat();
    const float h = bounds.getHeight();

    g.setColour(Palette::LIGHT_2);
    g.fillRoundedRectangle(bounds, 10);
    constexpr int steps = 32;

    const float attackCurve = std::pow(2.0f, state.get(ParameterIDs::ADSR_A_CURVE));
    const float decayCurve = std::pow(2.0f, state.get(ParameterIDs::ADSR_D_CURVE));
    const float releaseCurve = std::pow(2.0f, state.get(ParameterIDs::ADSR_R_CURVE));

    juce::Path p;
    p.startNewSubPath(0.0f, h);

    // Attack
    for (int i = 1; i <= steps; ++i)
    {
        const float t = static_cast<float>(i) / static_cast<float>(steps);
        const float x = t * attackX;
        const float y = h - std::pow(t, attackCurve) * h;

        p.lineTo(x, y);
    }

    // Decay
    for (int i = 1; i <= steps; ++i)
    {
        const float t = static_cast<float>(i) / static_cast<float>(steps);
        const float x = attackX + t * (decayX - attackX);
        const float y = h - (sustain + (1.0f - sustain) * (1.0f - std::pow(t, decayCurve))) * h;

        p.lineTo(x, y);
    }

    // Release
    for (int i = 1; i <= steps; ++i)
    {
        const float t = static_cast<float>(i) / static_cast<float>(steps);
        const float x = decayX + t * (releaseX - decayX);
        const float y = h - sustain * (1.0f - std::pow(t, releaseCurve)) * h;

        p.lineTo(x, y);
    }

    g.setColour(Palette::DARK_1);
    g.strokePath(p, juce::PathStrokeType(3.0f));

    g.setColour(Palette::DARK_3.withAlpha(0.35f));
    g.fillPath(p);

    updateAttackCurveLimit();
    updateDecayCurveLimit();
    updateReleaseCurveLimit();

    drawTimeRuler(g, bounds);
}


void ADSRViewer::drawTimeRuler(juce::Graphics& g, const juce::Rectangle<float>& bounds) const
{
    constexpr float totalSeconds = 15.0f;
    constexpr float majorTickHeight = 10.0f;
    constexpr float minorTickHeight = 5.0f;
    constexpr float rulerPadding = 5.0f;
    const float visibleSeconds = totalSeconds * zoom;
    const float pixelsPerSecond = bounds.getWidth() / visibleSeconds;
    const float top = bounds.getY() + rulerPadding;
    const float startX = bounds.getX();
    const float endX = bounds.getRight();
    const float targetSpacing = 55.0f;
    const float rawInterval = targetSpacing / pixelsPerSecond;
    const float magnitude = std::pow(10.0f, std::floor(std::log10(rawInterval)));
    const float normalized = rawInterval / magnitude;
    const float tickInterval = (normalized <= 1.0f ? 1.0f : normalized <= 2.0f ? 2.0f : normalized <= 5.0f ? 5.0f : 10.0f) * magnitude;
    const float minorInterval = tickInterval * 0.5f;
    g.saveState();
    g.reduceClipRegion(bounds.toNearestInt());
    g.setFont(Font::get(10.0f));
    for (float second = 0.0f; second <= visibleSeconds; second += tickInterval)
    {
        const float x = startX + second * pixelsPerSecond;
        if (x > endX)
            break;
        g.setColour(Palette::DARK_3.withAlpha(0.45f));
        g.drawVerticalLine(juce::roundToInt(x), top, top + majorTickHeight);
        g.drawText(juce::String(second / 3.0f, 2).trimCharactersAtEnd("0").trimCharactersAtEnd(".") + "s",
                   juce::Rectangle<float>(x - 20.0f, top + majorTickHeight, 40.0f, 12.0f),
                   juce::Justification::centred);
        const float minorX = x + minorInterval * pixelsPerSecond;
        if (minorX < endX)
        {
            g.setColour(Palette::DARK_3.withAlpha(0.20f));
            g.drawVerticalLine(juce::roundToInt(minorX), top, top + minorTickHeight);
        }
    }
    g.restoreState();
}

void ADSRViewer::resized()
{
    attackThumb.setInverse();
    decayThumb.setInverse();
    releaseThumb.setInverse();

    attackThumb.setSize(20, 20);
    decayThumb.setSize(20, 20);
    releaseThumb.setSize(20, 20);

    updateLayoutValues();

    updateAttackCurveLimit();
    updateDecayCurveLimit();
    updateReleaseCurveLimit();
}

void ADSRViewer::updateLayoutValues()
{
    const float tW = static_cast<float>(getWidth()) / zoom;

    attackX = attack * tW;
    decayX = attackX + decay * tW;
    releaseX = decayX + release * tW;
}

void ADSRViewer::updateAttackCurveLimit()
{
    const auto* param = state.apvts().getParameter(ParameterIDs::ADSR_A_CURVE);
    if (param == nullptr)
        return;

    // Evita que setCentrePosition dispare onPositionChanged y reescriba el parametro
    const juce::ScopedValueSetter<bool> guard(updatingFromParam, true);

    const float curveValue = param->getValue();

    attackThumb.setCentrePosition(juce::roundToInt(attackX * 0.5f),
                                  juce::roundToInt(curveValue * static_cast<float>(getHeight())));
}

void ADSRViewer::updateDecayCurveLimit()
{
    const auto* param = state.apvts().getParameter(ParameterIDs::ADSR_D_CURVE);
    if (param == nullptr)
        return;

    const juce::ScopedValueSetter<bool> guard(updatingFromParam, true);

    const float middleX = (attackX + decayX) * 0.5f;
    const float sustainY = 1.0f - sustain;
    const float curveValue = param->getValue();

    const float thumbY = (1.0f - curveValue) * sustainY;

    decayThumb.setCentrePosition(juce::roundToInt(middleX),
                                 juce::roundToInt(thumbY * static_cast<float>(getHeight())));
}

void ADSRViewer::updateReleaseCurveLimit()
{
    const auto* param = state.apvts().getParameter(ParameterIDs::ADSR_R_CURVE);
    if (param == nullptr)
        return;

    const juce::ScopedValueSetter<bool> guard(updatingFromParam, true);

    const float middleX = (decayX + releaseX) * 0.5f;
    const float sustainY = 1.0f - sustain;
    const float range = 1.0f - sustainY; // = sustain, mismo rango que en onPositionChanged
    const float curveValue = 1.0f - param->getValue();

    const float thumbY = sustainY + curveValue * range;

    releaseThumb.setCentrePosition(juce::roundToInt(middleX),
                                   juce::roundToInt(thumbY * static_cast<float>(getHeight())));
}

// Setters
void ADSRViewer::setAttackValue(const float val)
{
    attack = juce::jmap(val, 0.0f, 5.0f, 0.0f, 1.0f);
    repaint();
}

void ADSRViewer::setDecayValue(const float val)
{
    decay = juce::jmap(val, 0.0f, 5.0f, 0.0f, 1.0f);
    repaint();
}

void ADSRViewer::setSustainValue(const float val)
{
    sustain = val;
    repaint();
}

void ADSRViewer::setReleaseValue(const float val)
{
    release = juce::jmap(val, 0.0f, 5.0f, 0.0f, 1.0f);
    repaint();
}

// Mouse
void ADSRViewer::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    juce::ignoreUnused(event);

    if (wheel.deltaY > 0.0f)
        zoom -= 0.05f * zoom;
    if (wheel.deltaY < 0.0f)
        zoom += 0.05f * zoom;

    zoom = juce::jlimit(0.25f, 4.0f, zoom);
    repaint();
}