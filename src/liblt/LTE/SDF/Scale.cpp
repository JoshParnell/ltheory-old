#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFScale, SDFT,
    SDF, source,
    V3, scale)
    DERIVED_TYPE_EX(SDFScale)

    SDFScale() {}

    float Evaluate(V3 const& p) const {
      return source->Evaluate(p / scale);
    }

    Bound3 GetBound() const {
      Bound3 bound = source->GetBound();
      return Bound3(bound.lower * scale, bound.upper * scale);
    }

    String GetCode(String const& p) const {
      return source->GetCode(Stringize() | "(" | p | " / " | scale | ")");
    }
  };

  DERIVED_IMPLEMENT(SDFScale)
}

DefineFunction(SDF_Scale) {
  return new SDFScale(args.source, args.scale);
} FunctionAlias(SDF_Scale, *);
