#include "Module.h"
#include "StackFrame.h"
#include "Vector.h"

namespace {
  Vector<Module>& GetModules() {
    static Vector<Module> gModules;
    return gModules;
  }
}

void Module_RegisterGlobal(Module const& module) {
  LTE_ASSERT(!GetModules().contains(module));
  GetModules().push(module);
}

void Module_UpdateGlobal() {
  for (size_t i = 0; i < GetModules().size(); ++i)
    FRAME(GetModules()[i]->GetName())
      GetModules()[i]->Update();
}
