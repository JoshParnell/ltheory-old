#ifndef Game_Action_h__
#define Game_Action_h__

#include "Common.h"
#include "LTE/BaseType.h"
#include "LTE/Reference.h"

struct ActionT : public RefCounted {
  BASE_TYPE(ActionT)

  virtual void Execute(UpdateState&) const = 0;
  virtual String GetName() const = 0;
};

#endif
