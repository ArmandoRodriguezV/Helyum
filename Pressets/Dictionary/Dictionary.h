#pragma once
#include <JuceHeader.h>

enum class Category
{
  Basic,
  Complex
};

struct WaveTable
{
  int id;
  Category category;
  const void* data;
  size_t size;
  const char* name;
};

namespace WavetableLibrary
{
  extern const WaveTable all[];
  extern const int allCount;
  const WaveTable* find(int id);
}