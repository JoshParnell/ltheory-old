#include "SpatialPartition.h"

namespace  {
  bool VectorCallback(void* object, void* aux) {
    Vector<void*>& result = *(Vector<void*>*)aux;
    result.push(object);
    return true;
  }
}

void SpatialPartition::Query(V3 const& point, Vector<void*>& result) const {
  Query(point, VectorCallback, &result);
}

void SpatialPartition::Query(Bound3 const& box, Vector<void*>& result) const {
  Query(box, VectorCallback, &result);
}
