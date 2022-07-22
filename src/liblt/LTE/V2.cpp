#include "V2.h"
#include "Math.h"

V2 RandV2(float min, float max) {
  float x = Rand(min, max);
  float y = Rand(min, max);
  return V2(x, y);
}
