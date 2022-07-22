#include "../Warps.h"

#include "LTE/AutoClass.h"
#include "LTE/Script.h"

namespace {
  AutoClassDerived(WarpCustom, WarpT,
    Data, instance,
    ScriptFunction, getDelta)
    DERIVED_TYPE_EX(WarpCustom)

    WarpCustom() {}

    V3 GetDelta(V3 const& p) const {
      V3 result;
      getDelta->VoidCall(&result, instance, p);
      return result;
    }
  };
}

DefineFunction(Warp_Custom) {
  ScriptType type = args.data.type->GetAux().Convert<ScriptType>();
  Reference<WarpCustom> self = new WarpCustom;
  self->instance = args.data;
  self->getDelta = type->GetFunction("GetDelta");
  return self;
}
