
#pragma once
#include <JuceHeader.h>

class AudioLoader
{
public:
    static std::vector<float> loadWav(const void* data, size_t dataSize)
    {
        auto stream = std::make_unique<juce::MemoryInputStream>(data, dataSize, false);

        juce::AudioFormatManager formatManager;
        formatManager.registerBasicFormats();

        std::unique_ptr<juce::AudioFormatReader> reader(
            formatManager.createReaderFor(std::move(stream))
        );

        if (reader == nullptr)
            return {};

        int numSamples = static_cast<int>(reader->lengthInSamples);
        int numChannels = static_cast<int>(reader->numChannels);

        juce::AudioBuffer<float> buffer(numChannels, numSamples);

        if (!reader->read(&buffer, 0, numSamples, 0, true, true))
            return {};

        const float* channel = buffer.getReadPointer(0);

        return std::vector<float>(channel, channel + numSamples);
    }
};
