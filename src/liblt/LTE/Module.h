#ifndef LTE_Module_h__
#define LTE_Module_h__

#include "Reference.h"

struct ModuleT : public RefCounted {
  virtual ~ModuleT() {}

  virtual char const* GetName() const = 0;
  virtual void Update() = 0;
};

LT_API void Module_RegisterGlobal(Module const& module);
LT_API void Module_UpdateGlobal();

#endif
