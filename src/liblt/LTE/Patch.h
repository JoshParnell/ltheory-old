#ifndef LTE_Patch_h__
#define LTE_Patch_h__

#include "Common.h"

namespace LTE {
  struct Patch {
    virtual ~Patch() {}

    virtual void Add(Location const& target, Location const& patchFile) = 0;
    virtual bool Apply() const = 0;
  };

  LT_API Patch* Patch_Create();
}

#endif
