#ifndef LTE_Time_h__
#define LTE_Time_h__

#include "AutoClass.h"
#include "Common.h"
#include "DeclareFunction.h"

AutoClass(Time,
  uint, second,
  uint, minute,
  uint, hour,
  uint, day,
  uint, month,
  uint, year)
  Time() {}
};

DeclareFunctionNoParams(Time_Current, Time)

#endif
