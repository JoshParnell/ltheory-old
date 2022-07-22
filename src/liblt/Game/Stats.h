#ifndef Game_Stats_h__
#define Game_Stats_h__

#include "Common.h"

#define XLIST STATS_X
#define XNAME Stats
#define XSIZE StatType_SIZE
#define XTYPE double
#include "LTE/XVector.h"
#undef XTYPE
#undef XSIZE
#undef XNAME
#undef XLIST

#endif
