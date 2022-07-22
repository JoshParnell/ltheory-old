#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFRepeat, SDFT,
    SDF, source,
    SDF, box,
    V3, frequency,
    V3, phase)
    DERIVED_TYPE_EX(SDFRepeat)

    SDFRepeat() {}

    SDFRepeat(
        SDF const& source,
        V3 const& times,
        V3 const& spacing) :
      source(source)
    {
      Bound3 bound = source->GetBound();
      frequency = bound.GetSideLengths() + spacing + V3(1);
      phase = bound.lower;
      Bound3 boxBound(phase + V3(.01f), phase + frequency * times - V3(.02f));
      box = SDF_Box(boxBound.GetCenter(), boxBound.GetSideLengths() / 2.f);
    }

    float Evaluate(V3 const& p) const {
      V3 cp = Mod(p - phase, frequency) + phase;
      return Max(source->Evaluate(cp), box->Evaluate(p));
    }

    Bound3 GetBound() const {
      return box->GetBound();
    }

    String GetCode(String const& p) const {
      return Stringize()
        | "max(" | source->GetCode(Stringize()
        | "(mod(" | p | " - " | phase | ", " | frequency | ") + "
        | phase | ")")
        | "," | box->GetCode(p) | ")";
    }
  };

  DERIVED_IMPLEMENT(SDFRepeat)
}

DefineFunction(SDF_Repeat) {
  return new SDFRepeat(args.source, args.frequency, args.spacing);
}
