#ifndef LTE_Timer_h__
#define LTE_Timer_h__

#include "Common.h"

struct Timer {
  struct TimerData* d;

  LT_API Timer();
  LT_API ~Timer();

  float Lap() {
    float elapsed = GetElapsed();
    Reset();
    return elapsed;
  }

  /* NOTE : Measured in seconds. */
  LT_API float GetElapsed() const;

  LT_API void Reset();
};

#endif
