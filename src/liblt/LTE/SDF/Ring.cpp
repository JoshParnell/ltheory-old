#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFRing, SDFT,
    V3, center,
    float, radius,
    float, thickness)
    DERIVED_TYPE_EX(SDFRing)

    SDFRing() {}

    float Evaluate(V3 const& p) const {
      return Abs(Length(p.GetXZ() - center.GetXZ()) - radius) - thickness;
    }

    Bound3 GetBound() const {
      return Bound3(center - V3(radius + thickness),
                  center + V3(radius + thickness));
    }

    String GetCode(String const& p) const {
      return Stringize()
        | "(abs(length(" | p | ".xz - " | center | ".xz) - " | radius | ") - "
        | thickness | ")";
    }
  };

  DERIVED_IMPLEMENT(SDFRing)
}

DefineFunction(SDF_Ring) {
  return new SDFRing(args.center, args.radius, args.thickness);
}
