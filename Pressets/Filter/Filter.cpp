#include "Filter.h"

namespace Filter
{
    const FilterInterface all[] = {
        {0, "LowPass" , FilterCategory::LowPass},
        {1, "HighPass" , FilterCategory::HighPass},
        {2, "Notch" , FilterCategory::Notch},
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
