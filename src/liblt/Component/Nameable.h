#ifndef Component_Nameable_h__
#define Component_Nameable_h__

#include "Common.h"
#include "Game/Item.h"
#include "LTE/AutoClass.h"
#include "LTE/String.h"

AutoClass(ComponentNameable,
  String, name)

  ComponentNameable() :
    name("Unknown")
    {}
};

AutoComponent(Nameable)
  void SetSupertype(Item const& type) {
    if (type->GetName().size())
      Nameable.name = type->GetName();

    BaseT::SetSupertype(type);
  }

  String GetName() const {
    return Nameable.name;
  }

  void SetName(String const& name) {
    Nameable.name = name;
  }

  String ToString() const {
    return Nameable.name;
  }
};

#endif
