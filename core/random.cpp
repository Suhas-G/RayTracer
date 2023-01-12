#include <core/macros.h>
#include <core/random.h>

#include <random>

#include "random/pcg_random.hpp"

namespace rt {
inline static float uint32ToFloat(uint32_t v) {
  union {
    uint32_t u;
    float f;
  } x;

  x.u = (v >> 9) | 0x3F800000U;
  return x.f - 1.0f;
}

float random() {
  static thread_local pcg32 randomGenerator;
  return uint32ToFloat(randomGenerator());
}

float random(float max) { return random() * max; }

float random(float min, float max) { return random() * (max - min) + min; }

} // namespace rt
