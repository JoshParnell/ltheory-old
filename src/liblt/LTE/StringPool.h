#ifndef LTE_StringPool_h__
#define LTE_StringPool_h__

#include "Map.h"
#include "String.h"

struct StringPool {
  Map<String, char const*> strMap;

  char const* Get(String const& str) {
    char const*& ptr = strMap[str];
    if (!ptr)
      ptr = strdup(str.c_str());
    return ptr;
  }
};

#endif
