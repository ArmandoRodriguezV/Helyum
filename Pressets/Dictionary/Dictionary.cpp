#include "Dictionary.h"

namespace WavetableLibrary
{
    const WaveTable all[] = {
        {0, Category::Basic, BinaryData::basic_shapes_wav, BinaryData::basic_shapes_wavSize, "Basic shapes"},
        {1, Category::Basic, BinaryData::basic_morph_wav, BinaryData::basic_morph_wavSize, "Basic morph"},
        {2, Category::Complex, BinaryData::sine_morph_wav, BinaryData::sine_morph_wavSize, "Sine morph"},
    };

    const int allCount = std::size(all);
    const WaveTable* find(const int id)
    {
        for (int i = 0; i < allCount; ++i)
        {
            if (all[i].id == id)
                return &all[i];
        }

        return nullptr;
    }
}