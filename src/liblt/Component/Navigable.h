#ifndef Component_Navigable_h__
#define Component_Navigable_h__

#include "Common.h"
#include "Game/Object.h"
#include "LTE/AutoClass.h"
#include "LTE/Vector.h"

AutoClass(NavigableNode,
  Object, dest,
  float, cost)
  NavigableNode() {}
};

AutoClass(ComponentNavigable,
  Vector<NavigableNode>, nodes)

  ComponentNavigable() {}

  bool ConnectsTo(Object const& target) const {
    for (size_t i = 0; i < nodes.size(); ++i)
      if (nodes[i].dest == target)
        return true;
    return false;
  }
};

AutoComponent(Navigable)
};

#endif
