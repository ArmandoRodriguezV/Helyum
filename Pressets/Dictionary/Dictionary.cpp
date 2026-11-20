#include "Dictionary.h"

namespace WavetableLibrary
{
    const WaveTable all[] = {
        {0, Category::Basic, BinaryData::basic_shapes_wav, BinaryData::basic_shapes_wavSize, "Basic shapes"},
        {1, Category::Basic, BinaryData::basic_morph_wav, BinaryData::basic_morph_wavSize, "Basic morph"},
        {2, Category::Basic, BinaryData::sine_morph_wav, BinaryData::sine_morph_wavSize, "Sine morph"},
        {3, Category::Digital, BinaryData::duck_reflux_wav, BinaryData::duck_reflux_wavSize, "Duck reflux"},
        {4, Category::Complex, BinaryData::complex_1_wav, BinaryData::complex_1_wavSize, "Complex 1"},
        {5, Category::Complex, BinaryData::complex_2_wav, BinaryData::complex_2_wavSize, "Complex 2"},
        {6, Category::Digital, BinaryData::dark_ocean_wav, BinaryData::dark_ocean_wavSize, "Dark Ocean"},
        {7, Category::Basic, BinaryData::harmonic_ladder_wav, BinaryData::harmonic_ladder_wavSize, "Harmonic ladder"},
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