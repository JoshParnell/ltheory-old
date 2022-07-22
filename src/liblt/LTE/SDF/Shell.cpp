#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFShell, SDFT,
    V3, center,
    float, radius,
    float, thickness)
    DERIVED_TYPE_EX(SDFShell)

    SDFShell() {}

    float Evaluate(V3 const& p) const {
      return Abs(Length(p - center) - radius) - thickness;
    }

    Bound3 GetBound() const {
      return Bound3(center - V3(radius + thickness),
                  center + V3(radius + thickness));
    }

    String GetCode(String const& p) const {
      return Stringize()
        | "shell(" | p | ", " | center | ", " | radius | ", " | thickness | ")";
    }
  };

  DERIVED_IMPLEMENT(SDFShell)
}

DefineFunction(SDF_Shell) {
  return new SDFShell(args.center, args.radius, args.thickness);
}
