#ifndef LTE_Renderable_h__
#define LTE_Renderable_h__

#include "BaseType.h"
#include "Bound.h"
#include "Reference.h"
#include "V3.h"

struct RenderableT : public RefCounted {
  BASE_TYPE(RenderableT)

  virtual Bound3 GetBound() const {
    return Bound3(0);
  }

  LT_API virtual Mesh GetCollisionMesh() const;

  virtual size_t GetHash() const {
    return 0;
  }

  virtual short GetVersion() const {
    return 0;
  }

  virtual bool Intersects(
    Ray const& r,
    float* tOut = nullptr,
    V3* normalOut = nullptr) const
  {
    NOT_IMPLEMENTED
    return false;
  }

  virtual void Render(DrawState* state) const = 0;

  virtual V3 Sample() const {
    return 0;
  }

  FIELDS {}
};

LT_API Renderable Renderable_Union(
  Renderable const& a,
  Renderable const& b);

#endif
