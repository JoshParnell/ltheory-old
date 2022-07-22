#ifndef Attribute_Docks_h__
#define Attribute_Docks_h__

#include "Common.h"
#include "LTE/Bound.h"
#include "LTE/Vector.h"

template <class T>
struct Attribute_Docks : public T {
  typedef Attribute_Docks SelfType;
  Vector<Bound3> docks;
  uint dockCapacity;

  Attribute_Docks() :
    dockCapacity(0)
    {}

  uint GetDockCapacity() const {
    return dockCapacity;
  }

  Vector<Bound3> const* GetDocks() const {
    return &docks;
  }

  bool HasDocks() const {
    return true;
  }
};

#endif
