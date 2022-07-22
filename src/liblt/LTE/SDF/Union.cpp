#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFUnion, SDFT,
    SDF, a,
    SDF, b,
    float, sharpness)
    DERIVED_TYPE_EX(SDFUnion)

    SDFUnion() {}

    float Evaluate(V3 const& p) const {
      float at = a->Evaluate(p);
      float bt = b->Evaluate(p);
      return Mix(at, bt, Sigmoid((at - bt) * sharpness));
    }

    Bound3 GetBound() const {
      return a->GetBound().Union(b->GetBound());
    }

    String GetCode(const String& p) const {
      return Stringize()
        | "smoothUnion(" | a->GetCode(p) | ", " | b->GetCode(p) | ", "
        | sharpness | ")";
    }
  };

  DERIVED_IMPLEMENT(SDFUnion)

  AutoClassDerived(SDFUnion0, SDFT,
    SDF, a,
    SDF, b)
    DERIVED_TYPE_EX(SDFUnion0)

    SDFUnion0() {}

    float Evaluate(V3 const& p) const {
      return Min(a->Evaluate(p), b->Evaluate(p));
    }

    Bound3 GetBound() const {
      return a->GetBound().Union(b->GetBound());
    }

    String GetCode(const String& p) const {
      return "min(" + a->GetCode(p) + ", " + b->GetCode(p) + ")";
    }
  };

  DERIVED_IMPLEMENT(SDFUnion0)
}

DefineFunction(SDF_Union) {
  if (args.sharpness <= 0)
    return new SDFUnion0(args.a, args.b);
  else
    return new SDFUnion(args.a, args.b, args.sharpness);
}

SDF SDFT::Union(SDF const& other, float sharpness) {
  return SDF_Union(this, other, sharpness);
}
