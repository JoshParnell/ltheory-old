#include "Debug.h"

namespace {
  int gLevel = 0;
}

namespace LTE {
  int Debug_GetLevel() {
    return gLevel;
  }

  void Debug_SetLevel(int level) {
    gLevel = level;
  }
}
