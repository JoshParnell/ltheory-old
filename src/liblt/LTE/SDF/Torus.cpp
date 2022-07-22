#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFTorus, SDFT,
    V3, center,
    float, radius,
    float, thickness)
    DERIVED_TYPE_EX(SDFTorus)

    SDFTorus() {}

    float Evaluate(V3 const& p) const {
      V3 o = p - center;
      return Length(o - radius * Normalize(V3(o.x, 0.0f, o.z))) - thickness;
    }

    Bound3 GetBound() const {
      return Bound3(
        center - V3(radius + thickness),
        center + V3(radius + thickness));
    }

    String GetCode(String const& p) const {
      return Stringize()
        | "torus(" | p | ", " | center | ", " | radius | ", " | thickness | ")";
    }
  };

  DERIVED_IMPLEMENT(SDFTorus)
}

DefineFunction(SDF_Torus) {
  return new SDFTorus(args.center, args.radius, args.thickness);
}
