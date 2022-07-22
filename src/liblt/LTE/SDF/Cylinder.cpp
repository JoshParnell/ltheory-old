#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFCylinder, SDFT,
    V3, center,
    V3, axis,
    float, radius)
    DERIVED_TYPE_EX(SDFCylinder)

    SDFCylinder() {}

    float Evaluate(V3 const& p) const {
      NOT_IMPLEMENTED
      return 1;
    }

    Bound3 GetBound() const {
      return Bound3(-FLT_MAX, FLT_MAX);
    }

    String GetCode(String const& p) const {
      return Stringize()
        | "cylinder(" | p | ", " | center | ", " | axis
        | ", " | radius | ")";
    }
  };

  DERIVED_IMPLEMENT(SDFCylinder)
}

DefineFunction(SDF_Cylinder) {
  return new SDFCylinder(args.center, Normalize(args.axis), args.radius);
}
