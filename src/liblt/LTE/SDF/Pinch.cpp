#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFPinchAxis, SDFT,
    SDF, source,
    V3, axis)
    DERIVED_TYPE_EX(SDFPinchAxis)

    SDFPinchAxis() {}

    float Evaluate(V3 const& p) const {
      NOT_IMPLEMENTED
      return source->Evaluate(p);
    }

    Bound3 GetBound() const {
      return source->GetBound();
    }

    String GetCode(String const& p) const {
      return source->GetCode(Stringize() | "pinchAxis(" | p | ", " | axis | ")");
    }
  };

  DERIVED_IMPLEMENT(SDFPinchAxis)

  AutoClassDerived(SDFPinchY, SDFT,
    SDF, source,
    V3, axis)
    DERIVED_TYPE_EX(SDFPinchY)

    SDFPinchY() {}

    float Evaluate(V3 const& p) const {
      V3 cp = p;
      cp.y *= 1.0f + Max(0.0f, Dot(p, axis) - 1.0f);
      return source->Evaluate(cp);
    }

    Bound3 GetBound() const {
      return source->GetBound();
    }

    String GetCode(String const& p) const {
      return source->GetCode(Stringize() | "pinchY(" | p | ", " | axis | ")");
    }
  };

  DERIVED_IMPLEMENT(SDFPinchY)
}

SDF SDFT::PinchAxis(V3 const& axis) {
  return new SDFPinchAxis(this, axis);
}

SDF SDFT::PinchY(V3 const& axis) {
  return new SDFPinchY(this, axis);
}
