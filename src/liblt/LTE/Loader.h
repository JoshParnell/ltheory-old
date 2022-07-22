#ifndef LTE_Loader_h__
#define LTE_Loader_h__

#include "Common.h"

namespace LTE {
  LT_API bool RegisterLoader(void (*loader)());
  LT_API bool RegisterUnloader(void (*unloader)());
}

#endif
