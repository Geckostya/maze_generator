#pragma once
#include <algorithm>

template<typename T>
T clamp(const T value, const T lo, const T hi) {
  return std::min(std::max(value, lo), hi);
}
