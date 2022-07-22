#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFSubtract, SDFT,
    SDF, a,
    SDF, b,
    float, sharpness)
    DERIVED_TYPE_EX(SDFSubtract)

    SDFSubtract() {}

    float Evaluate(V3 const& p) const {
      float at = a->Evaluate(p);
      float bt = -b->Evaluate(p);
      float alpha = Sigmoid((bt - at) * sharpness);
      return Mix(at, bt, alpha);
    }

    Bound3 GetBound() const {
      return a->GetBound();
    }

    String GetCode(const String& p) const {
      return Stringize()
        | "intersect(" | a->GetCode(p) | ", -" | b->GetCode(p) | ", "
        | sharpness | ")";
    }
  };

  DERIVED_IMPLEMENT(SDFSubtract)

  AutoClassDerived(SDFSubtract0, SDFT,
    SDF, a,
    SDF, b)
    DERIVED_TYPE_EX(SDFSubtract0)

    SDFSubtract0() {}

    float Evaluate(V3 const& p) const {
      return Max(a->Evaluate(p), -b->Evaluate(p));
    }

    Bound3 GetBound() const {
      return a->GetBound();
    }

    String GetCode(const String& p) const {
      return Stringize()
        | "max(" | a->GetCode(p) | ", -" | b->GetCode(p) | ")";
    }
  };

  DERIVED_IMPLEMENT(SDFSubtract0)
}

DefineFunction(SDF_Subtract) {
  if (args.sharpness <= 0)
    return new SDFSubtract0(args.a, args.b);
  else
    return new SDFSubtract(args.a, args.b, args.sharpness);
} FunctionAlias(SDF_Subtract, -);

SDF SDFT::Subtract(SDF const& other, float sharpness) {
  return SDF_Subtract(this, other, sharpness);
}
