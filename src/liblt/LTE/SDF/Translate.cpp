#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFTranslate, SDFT,
    SDF, source,
    V3, offset)
    DERIVED_TYPE_EX(SDFTranslate)

    SDFTranslate() {}

    float Evaluate(V3 const& p) const {
      return source->Evaluate(p - offset);
    }

    Bound3 GetBound() const {
      Bound3 bound = source->GetBound();
      return Bound3(bound.lower + offset, bound.upper + offset);
    }

    String GetCode(String const& p) const {
      return source->GetCode(Stringize() | "(" | p | " - " | offset | ")");
    }
  };

  DERIVED_IMPLEMENT(SDFTranslate)
}

DefineFunction(SDF_Translate) {
  return new SDFTranslate(args.source, args.offset);
} FunctionAlias(SDF_Translate, +);
