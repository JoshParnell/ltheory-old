#ifndef AI_Traits_h__
#define AI_Traits_h__

#define TRAITS_X                                                               \
  X(Aggressive)                                                                \
  X(Creative)                                                                  \
  X(Explorative)                                                               \
  X(Greedy)                                                                    \
  X(Intellectual)                                                              \
  X(Lawless)                                                                   \
  X(Sociable)                                                                  \

#define XLIST TRAITS_X
#define XNAME Traits
#define XSIZE 6
#define XTYPE float
#include "LTE/XVector.h"
#undef XTYPE
#undef XSIZE
#undef XNAME
#undef XLIST

#endif
