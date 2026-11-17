#include <JuceHeader.h>

enum class Category
{
    Basic,
    Digital
};

struct WaveTable
{
    int id;
    Category cat;
    const void* data;
    size_t size;
    const char* name;
};

namespace WavetableLibrary
{
    inline const WaveTable all[] = {
        {0, Category::Basic, BinaryData::basic_shapes_wav, BinaryData::basic_shapes_wavSize},
        {1, Category::Basic, BinaryData::basic_morph_wav, BinaryData::basic_morph_wavSize},
        {2, Category::Basic, BinaryData::sine_morph_wav, BinaryData::sine_morph_wavSize},
    };

    inline constexpr int allCount = sizeof(all) / sizeof(WaveTable);
    inline const WaveTable* find(int id)
    {
        for (int i = 0; i < allCount; ++i)
        {
            if (all[i].id == id)
                return &all[i];
        }

        return nullptr;
    }
}