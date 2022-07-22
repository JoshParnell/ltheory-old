#include "../Warps.h"

#include "LTE/AutoClass.h"

namespace {
  AutoClassDerived(WarpAttractorPoint, WarpT,
    V3, center,
    float, strength)
    DERIVED_TYPE_EX(WarpAttractorPoint)

    WarpAttractorPoint() {}

    V3 GetDelta(V3 const& p) const {
      V3 toCenter = center - p;
      float dist = Length(toCenter);
      return Mix(p, center, Exp(-dist / strength)) - p;
    }
  };

  AutoClassDerived(WarpAttractorPlane, WarpT,
    V3, center,
    V3, normal,
    float, strength)
    DERIVED_TYPE_EX(WarpAttractorPlane)

    WarpAttractorPlane() {}

    V3 GetDelta(V3 const& p) const {
      V3 proj = p + normal * Dot(center - p, normal);
      V3 toCenter = proj - p;
      float dist = Length(toCenter);
      return Mix(p, proj, Exp(-dist / strength)) - p;
    }
  };
}

DefineFunction(Warp_AttractorPlane) {
  return new WarpAttractorPlane(args.center, args.normal, args.strength);
}

DefineFunction(Warp_AttractorPoint) {
  return new WarpAttractorPoint(args.center, args.strength);
}
