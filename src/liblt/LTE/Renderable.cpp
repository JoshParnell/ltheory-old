#include "Renderable.h"

#include "AutoClass.h"
#include "Math.h"
#include "Mesh.h"

TypeAlias(Reference<RenderableT>, Renderable);

namespace {
  AutoClassDerived(RenderableUnion, RenderableT,
    Renderable, one,
    Renderable, two)
    DERIVED_TYPE_EX(RenderableUnion)

    RenderableUnion() {}

    Bound3 GetBound() const {
      return one->GetBound().GetUnion(two->GetBound());
    }

    Mesh GetCollisionMesh() const {
      Mesh m1 = one->GetCollisionMesh();
      Mesh m2 = two->GetCollisionMesh();
      if (m1 && m2)
        return m1->AddMesh(m2);
      else if (m1)
        return m1;
      else
        return m2;
    }

    size_t GetHash() const {
      size_t h1 = one->GetHash();
      size_t h2 = two->GetHash();
      if (h1 && h2)
        return h1 ^ h2;
      else if (h1)
        return h1;
      else
        return h2;
    }

    short GetVersion() const {
      return one->GetVersion() + two->GetVersion();
    }

    bool Intersects(
      Ray const& r,
      float* tOut = nullptr,
      V3* normalOut = nullptr) const
    {
      float t1 = FLT_MAX;
      V3 n1;
      one->Intersects(r, &t1, &n1);

      float t2 = FLT_MAX;
      V3 n2;
      two->Intersects(r, &t2, &n2);

      if (t1 < t2) {
        if (tOut)
          *tOut = t1;
        if (normalOut)
          *normalOut = n1;
        return t1 < FLT_MAX;
      }

      else {
        if (tOut)
          *tOut = t2;
        if (normalOut)
          *normalOut = n2;
        return t2 < FLT_MAX;
      }
    }

    void Render(DrawState* state) const {
      one->Render(state);
      two->Render(state);
    }

    V3 Sample() const {
      return Rand() < 0.5f ? one->Sample() : two->Sample();
    }
  };
}

Mesh RenderableT::GetCollisionMesh() const {
  return nullptr;
}

LT_API Renderable Renderable_Union(
  Renderable const& a,
  Renderable const& b)
{
  return new RenderableUnion(a, b);
}
