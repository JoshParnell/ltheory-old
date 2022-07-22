#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFIntersection, SDFT,
    SDF, a,
    SDF, b,
    float, sharpness)
    DERIVED_TYPE_EX(SDFIntersection)

    SDFIntersection() {}

    float Evaluate(V3 const& p) const {
      return Max(a->Evaluate(p), b->Evaluate(p));
    }

    Bound3 GetBound() const {
      return a->GetBound().Intersect(b->GetBound());
    }

    String GetCode(String const& p) const {
      return "max(" + a->GetCode(p) + ", " + b->GetCode(p) + ")";
    }
  };

  DERIVED_IMPLEMENT(SDFIntersection)
}

SDF SDFT::Intersect(SDF const& other, float sharpness) {
  return new SDFIntersection(this, other, sharpness);
}
