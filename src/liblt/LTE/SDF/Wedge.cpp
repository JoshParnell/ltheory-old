#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFWedge, SDFT,
    V3, center,
    float, angle,
    float, angularExtent,
    float, radius,
    float, radialExtent,
    float, height)
    DERIVED_TYPE_EX(SDFWedge)

    SDFWedge() {}

    float Evaluate(V3 const& p) const {
      NOT_IMPLEMENTED
      return 0;
    }

    Bound3 GetBound() const {
      float r = radius + radialExtent;
      return Bound3(center - V3(r, height, r), center + V3(r, height, r));
    }

    String GetCode(const String& p) const {
      return Stringize()
        | "wedge(" | p | ", " | center | ", " | angle | ", "
        | angularExtent | ", " | radius | ", " | radialExtent | ", "
        | height | ")";
    }
  };

  DERIVED_IMPLEMENT(SDFWedge)
}

DefineFunction(SDF_Wedge) {
  return new SDFWedge(
    args.center,
    args.angle,
    args.angularExtent,
    args.radius,
    args.radialExtent,
    args.height);
}
