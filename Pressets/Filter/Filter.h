#pragma once
#include <JuceHeader.h>

enum class FilterCategory
{
    LowPass,
    HighPass,
    Notch
};

struct FilterInterface
{
    int id;
    const char* name;
    FilterCategory category;
};

namespace Filter
{
    extern const FilterInterface all[];
    extern const int allCount;
    const FilterInterface* getFilterById(const int id);
};