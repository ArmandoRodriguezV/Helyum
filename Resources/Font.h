#pragma once
#include <JuceHeader.h>

struct Font {
    static juce::Font get(float size = 16.0f) {
        static auto typeface = juce::Typeface::createSystemTypefaceFor(
            BinaryData::JetBrainsMono_ttf,
            BinaryData::JetBrainsMono_ttfSize
        );
        return juce::Font(juce::FontOptions(typeface).withHeight(size));
    }
};