#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFExpand, SDFT,
    SDF, source,
    V3, amount)
    DERIVED_TYPE_EX(SDFExpand)

    SDFExpand() {}

    float Evaluate(V3 const& p) const {
      return source->Evaluate(p - Clamp(p, -amount, amount));
    }

    Bound3 GetBound() const {
      return source->GetBound().GetExpanded(V3(1) + amount);
    }

    String GetCode(String const& p) const {
      return source->GetCode(Stringize() | "expand(" | p | ", " | amount | ")");
    }
  };

  DERIVED_IMPLEMENT(SDFExpand)

  AutoClassDerived(SDFExpandRadial, SDFT,
    SDF, source,
    V3, axis,
    float, amount)
    DERIVED_TYPE_EX(SDFExpandRadial)

    SDFExpandRadial() {}

    float Evaluate(V3 const& p) const {
      NOT_IMPLEMENTED
      return 0;
    }

    Bound3 GetBound() const {
      return source->GetBound().GetExpanded(1 + amount);
    }

    String GetCode(String const& p) const {
      return source->GetCode(Stringize()
        | "expandRadial(" | p | ", " | axis | ", " | amount | ")");
    }
  };

  DERIVED_IMPLEMENT(SDFExpandRadial)
}

SDF SDFT::Expand(V3 const& amount) {
  return new SDFExpand(this, amount);
}

SDF SDFT::ExpandRadial(V3 const& axis, float amount) {
  return new SDFExpandRadial(this, axis, amount);
}
