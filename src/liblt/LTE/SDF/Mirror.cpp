#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"

namespace {
  AutoClassDerived(SDFMirror, SDFT,
    SDF, source,
    V3, origin,
    V3, normal)
    DERIVED_TYPE_EX(SDFMirror)

    SDFMirror() {}

    float Evaluate(V3 const& p) const {
      return source->Evaluate(HalfMirrorPoint(p));
    }

    V3 HalfMirrorPoint(V3 const& p) const {
      return p + 2.0f * Max(Dot(origin - p, normal), 0.0f) * normal;
    }

    V3 MirrorPoint(V3 const& p) const {
      return p + 2.0f * Dot(origin - p, normal) * normal;
    }

    Bound3 GetBound() const {
      Bound3 sourceBound = source->GetBound();
      Bound3 newBound(sourceBound);
      newBound.AddPoint(MirrorPoint(sourceBound.lower));
      newBound.AddPoint(MirrorPoint(sourceBound.upper));
      return newBound;
    }

    String GetCode(String const& p) const {
      return source->GetCode(Stringize()
        | "(" | p | " + 2.0*max(dot(" | origin | " - " | p | ", "
        | normal | "), 0.)*" | normal | ")");
    }
  };

  DERIVED_IMPLEMENT(SDFMirror)

  AutoClassDerived(SDFMirrorRadial, SDFT,
    SDF, source,
    V3, origin,
    V3, e1,
    V3, e2,
    V3, e3,
    float, angleSize)
    DERIVED_TYPE_EX(SDFMirrorRadial)

    SDFMirrorRadial() {}

    SDFMirrorRadial(
        SDF const& source,
        V3 const& origin,
        V3 const& normal,
        float angleSize) :
      source(source),
      origin(origin),
      e3(normal),
      angleSize(angleSize)
    {
      OrthoBasis(normal, e1, e2);
    }

    float Evaluate(V3 const& p) const {
      V3 pos = p - origin;
      float x = Dot(pos, e1);
      float y = Dot(pos, e2);
      float z = Dot(pos, e3);
      float a = kTau * Mod(Atan(y, x) / kTau + 0.5f, angleSize) / angleSize;
      float r = Sqrt(x*x + y*y);
      return source->Evaluate((r * Cos(a)) * e1 +
                              (r * Sin(a)) * e2 +
                              z * e3 + origin);
    }

    Bound3 GetBound() const {
      Bound3 sourceBound = source->GetBound();

      float radius = 0;
      for (uint i = 0; i < 8; ++i) {
        V3 v = sourceBound[i] - origin;
        radius = Length(V2(Dot(v, e1), Dot(v, e2))); 
      }

      return Bound3(origin - V3(radius),
                  origin + V3(radius));
    }

    String GetCode(String const& p) const {
      return source->GetCode(Stringize()
        | "mirrorRadial(" | p | ", " | origin | ", "
        | e1 | ", " | e2 | ", " | e3 | ", " | angleSize | ")");
    }
  };

  DERIVED_IMPLEMENT(SDFMirrorRadial)
}

SDF SDFT::Mirror(V3 const& origin, V3 const& normal) {
  return new SDFMirror(this, origin, normal);
}

SDF SDFT::MirrorRadial(
  V3 const& origin,
  V3 const& normal,
  float angleSize)
{
  return new SDFMirrorRadial(this, origin, normal, angleSize);
}
