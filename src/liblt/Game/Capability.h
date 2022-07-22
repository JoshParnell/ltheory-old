#ifndef Game_Capability_h__
#define Game_Capability_h__

#include "Common.h"

#define XLIST CAPABILITY_X
#define XNAME Capability
#define XSIZE CapabilityType_SIZE
#define XTYPE float
#include "LTE/XVector.h"
#undef XTYPE
#undef XSIZE
#undef XNAME
#undef XLIST

#define X(x) inline Capability Capability_##x(float amount) {                  \
  Capability c;                                                                \
  c.x = amount;                                                                \
  return c; }
CAPABILITY_X
#undef X

#endif
