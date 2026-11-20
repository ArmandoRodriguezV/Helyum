#include "Filter.h"

namespace Filter
{
    const FilterInterface all[] = {
        {0, "LowPass 6" , FilterCategory::LowPass, 1},
        {1, "LowPass 12", FilterCategory::LowPass, 2},
        {2, "LowPass 24", FilterCategory::LowPass, 4},
        {3, "HighPass 6" , FilterCategory::HighPass, 1},
        {4, "HighPass 12", FilterCategory::HighPass, 2},
        {5, "HighPass 24", FilterCategory::HighPass, 4},
        {6, "Notch" , FilterCategory::Notch, 1},
    };

    const int allCount = std::size(all);

    const FilterInterface* getFilterById(const int id)
    {
        for (int i = 0; i < allCount; i++)
        {
            if (all[i].id == id)
                return &all[i];
        }

        return nullptr;
    }
}
