#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFSphere, SDFT,
    V3, center,
    float, radius)
    DERIVED_TYPE_EX(SDFSphere)

    SDFSphere() {}

    float Evaluate(V3 const& p) const {
      return Length(p - center) - radius;
    }

    Bound3 GetBound() const {
      return Bound3(center - V3(radius), center + V3(radius));
    }

    String GetCode(String const& p) const {
      return Stringize()
        | "(length(" | p | " - " | center | ") - " | radius | ")";
    }
  };

  DERIVED_IMPLEMENT(SDFSphere)
}

DefineFunction(SDF_Sphere) {
  return new SDFSphere(args.center, args.radius);
}
