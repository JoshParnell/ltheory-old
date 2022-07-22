#ifndef LTE_SpatialSignature_h__
#define LTE_SpatialSignature_h__

#include "Common.h"

namespace LTE {
  struct SpatialSignature {
    typedef bool (*AddFn)(Bound3 const& box, void* aux);

    virtual ~SpatialSignature() {}

    virtual void Add(void* object, Bound3 const& box, AddFn callback, void* aux) = 0;
    virtual void Add(void* object, Bound3 const& box) = 0;
    virtual void Clear() = 0;

    virtual size_t GetMemoryUsage() const = 0;
    virtual float GetOccupancy() const = 0;

    virtual bool Query(Bound3 const& box) const = 0;
  };

  LT_API SpatialSignature* SpatialSignature_Grid(
    Bound3 const& box,
    size_t cellsX,
    size_t cellsY,
    size_t cellsZ);

  LT_API SpatialSignature* SpatialSignature_Grid(Bound3 const& box, size_t cells);
}

#endif
