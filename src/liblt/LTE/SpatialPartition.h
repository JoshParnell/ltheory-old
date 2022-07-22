#ifndef LTE_SpatialPartition_h__
#define LTE_SpatialPartition_h__

#include "Common.h"
#include "Vector.h"

struct SpatialPartition {
  typedef bool (*AddFn)(Bound3 const& box, void* aux);
  typedef bool (*QueryFn)(void* object, void* aux);

  virtual ~SpatialPartition() {}

  virtual void Add(
    void* object,
    Bound3 const& box,
    AddFn callback,
    void* aux) = 0;

  virtual void Add(void* object, Bound3 const& box) = 0;

  virtual void Clear() = 0;

  virtual void Remove(void* object, Bound3 const& box) = 0;

  virtual size_t GetMemoryUsage() const = 0;

  virtual void Query(V3 const& point, QueryFn callback, void* aux) const = 0;

  virtual void Query(Bound3 const& box, QueryFn callback, void* aux) const = 0;

  virtual void Query(
    Ray const& ray,
    float tMax,
    QueryFn callback,
    void* aux) const = 0;

  LT_API virtual void Query(V3 const& point, Vector<void*>& result) const;

  LT_API virtual void Query(Bound3 const& box, Vector<void*>& result) const;
};

LT_API SpatialPartition* SpatialPartition_Hash(V3 const& cellSize, size_t cells);

LT_API SpatialPartition* SpatialPartition_Uniform(
  Bound3 const& box,
  size_t cellsX,
  size_t cellsY,
  size_t cellsZ);

#endif
