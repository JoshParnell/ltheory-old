#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFMultiply, SDFT,
    SDF, source,
    float, value)
    DERIVED_TYPE_EX(SDFMultiply)

    SDFMultiply() {}

    float Evaluate(V3 const& p) const {
      return source->Evaluate(p) * value;
    }

    Bound3 GetBound() const {
      return source->GetBound();
    }

    String GetCode(String const& p) const {
      return Stringize()
        | "(" | value | " * max(" | source->GetCode(p) | ", 0.))";
    }
  };

  DERIVED_IMPLEMENT(SDFMultiply)
}

DefineFunction(SDF_Multiply) {
  return new SDFMultiply(args.source, args.value);
}
