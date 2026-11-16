#pragma once
#include <JuceHeader.h>
#include "AudioLoader.h"

class Wavetable
{
public:
    static constexpr int FRAME_SIZE = 2048;
    using Frame = std::vector<float>;
    using Table = std::vector<Frame>;

    static Table loadWavetable(const void* data, size_t dataSize)
    {
        auto audio = AudioLoader::loadWav(data, dataSize);
        Table chunks;

        const int totalFrames = static_cast<int>(audio.size()) / FRAME_SIZE;
        chunks.reserve(totalFrames);

        for (int i = 0; i < totalFrames; ++i)
        {
            const int start = i * FRAME_SIZE;
            const int end = start + FRAME_SIZE;

            chunks.emplace_back(audio.begin() + start, audio.begin() + end);
        }

        return chunks;
    }
};