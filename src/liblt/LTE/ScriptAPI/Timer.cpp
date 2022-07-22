#include "LTE/Timer.h"
#include "LTE/Function.h"

FreeFunctionNoParams(Timer*, Timer,
  "Create and reset a new timer")
{
  return new Timer;
}

FreeFunction(float, Timer_GetElapsed,
  "Return the number of seconds elapsed since 'timer' was reset",
  Timer*, timer)
{
  return timer->GetElapsed();
} FunctionAlias(Timer_GetElapsed, GetElapsed);

VoidFreeFunction(Timer_Reset,
  "Reset 'timer'",
  Timer*, timer)
{
  Mutable(*timer).Reset();
} FunctionAlias(Timer_Reset, Reset);
