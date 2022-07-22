#ifndef Component_Dockable_h__
#define Component_Dockable_h__

#include "Common.h"
#include "Game/Item.h"
#include "Game/Object.h"
#include "LTE/AutoClass.h"
#include "LTE/Bound.h"
#include "LTE/Vector.h"

AutoClass(ComponentDockable,
  Vector<Bound3>, hangars,
  Vector<Bound3>, ports,
  int, docked,
  int, capacity)

  ComponentDockable() :
    docked(0),
    capacity(0)
    {}

  LT_API bool CanDock(ObjectT* self, Object const& docker);
  LT_API void Dock(ObjectT* self, Object const& docker);
  LT_API void Undock(ObjectT* self, Object const& docker);
};

AutoComponent(Dockable)
  void SetSupertype(Item const& type) {
    if (type->GetDocks()) {
      Dockable.ports = *type->GetDocks();
      Dockable.hangars = Dockable.ports; // TODO
    }
    Dockable.capacity = type->GetDockCapacity();

    BaseT::SetSupertype(type);
  }

  bool CanDock(Object const& docker) {
    return Dockable.CanDock(this, docker);
  }

  void Dock(Object const& docker) {
    Dockable.Dock(this, docker);
  }

  void Undock(Object const& docker) {
    Dockable.Undock(this, docker);
  }
};

#endif
