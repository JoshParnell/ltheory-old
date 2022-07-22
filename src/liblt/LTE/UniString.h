#ifndef LTE_UniString_h__
#define LTE_UniString_h__

#include "Common.h"

struct UniStringIterator {
  uint32 codepoint;
  char const* iter;
  char const* end;

  LT_API void Advance();

  bool HasMore() const {
    return iter != end;
  }

  uint32 Get() const {
    return codepoint;
  }
};

LT_API UniStringIterator UniString_Begin(String const& source);

#endif
