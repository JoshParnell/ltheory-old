#include "../SDFs.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"
#include "LTE/Vector.h"

const float kMaxSDFT = 1e35f;

namespace {
  AutoClassDerived(SDFPolyhedron, SDFT,
    Vector<Plane>, planes,
    float, shape,
    float, rcpShape)
    DERIVED_TYPE_EX(SDFPolyhedron)

    SDFPolyhedron() {}

    SDFPolyhedron(Vector<Plane> const& planes, int shape) :
      planes(planes),
      shape(2 * (float)shape)
    {
      rcpShape = 1 / this->shape;
    }

    float Evaluate(V3 const& p) const {
      float d = 0;
      for (size_t i = 0; i < planes.size(); ++i) {
        float dot = Dot(p, planes[i].normal) / planes[i].w;
        d += Min(Pow(dot, shape), kMaxSDFT);
      }
      return Pow(d, rcpShape) - 1;
    }

    Bound3 GetBound() const {
      return Bound3(-FLT_MAX, FLT_MAX);
    }

    String GetCode(String const& p) const {
      Stringize str;
      str | "(pow(";
      for (size_t i = 0; i < planes.size(); ++i) {
        if (i) str | " + ";
        str | "min(pow(abs(dot(" | p | ", " | planes[i].normal | ") / " 
            | planes[i].w | "), " | shape | "), 1e30)";
      }
      return str | ", " | rcpShape | ") - 1.)";
    }
  };

  DERIVED_IMPLEMENT(SDFPolyhedron)

  AutoClassDerived(SDFPolyhedron0, SDFT,
    Vector<Plane>, planes)
    DERIVED_TYPE_EX(SDFPolyhedron0)

    SDFPolyhedron0() {}

    float Evaluate(V3 const& p) const {
      float d = planes[0].PointTest(p);
      for (size_t i = 1; i < planes.size(); ++i)
        d = Max(d, planes[i].PointTest(p));
      return d;
    }

    Bound3 GetBound() const {
      return Bound3(-FLT_MAX, FLT_MAX);
    }

    String GetCode(String const& p) const {
      Stringize str;
      for (size_t i = 0; i < planes.size(); ++i) {
        if (i + 1 < planes.size())
          str | "max(";
        str | "dot(" | p | ", " | planes[i].normal | ") - " | planes[i].w;
        if (i + 1 < planes.size())
          str | ", ";
      }

      for (size_t i = 0; i < planes.size() - 1; ++i)
        str | ')';
      return str;
    }
  };

  DERIVED_IMPLEMENT(SDFPolyhedron0)
}

DefineFunction(SDF_Polyhedron) {
  if (args.shape == 0)
    return new SDFPolyhedron0(args.planes);
  else
    return new SDFPolyhedron(args.planes, args.shape);
}
