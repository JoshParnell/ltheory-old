#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFCapsule, SDFT,
    V3, o,
    V3, n,
    V3, nNorm,
    float, length,
    float, radius)
    DERIVED_TYPE_EX(SDFCapsule)

    SDFCapsule() {}

    SDFCapsule(V3 const& p1, V3 const& p2, float radius) :
      o(p1),
      n(p2 - p1),
      radius(radius)
    {
      length = Length(n);
      nNorm = Normalize(n);
    }

    float Evaluate(V3 const& p) const {
      V3 v = p - o;
      float proj = Clamp(Dot(v, nNorm), 0.f, length);
      return Length(v - nNorm * proj) - radius;
    }

    Bound3 GetBound() const {
      Bound3 box = Bound3::FromPoints(o, o + n);
      return Bound3(box.lower - V3(radius), box.upper + V3(radius));
    }

    String GetCode(String const& p) const {
      return Stringize()
        | "capsule(" | p | ", " | o | ", " | nNorm | ", " | length
        | ", " | radius | ")";
    }
  };

  DERIVED_IMPLEMENT(SDFCapsule)
}

DefineFunction(SDF_Capsule) {
  return new SDFCapsule(args.p1, args.p2, args.radius);
}
