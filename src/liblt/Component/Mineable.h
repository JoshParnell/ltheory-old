#ifndef Component_Mineable_h__
#define Component_Mineable_h__

#include "Common.h"
#include "Game/Item.h"
#include "LTE/AutoClass.h"
#include "LTE/Vector.h"

AutoClass(ComponentMineable,
  Item, item,
  Quantity, quantity,
  V3, phase)

  ComponentMineable() :
    quantity(0),
    phase(0)
    {}
};

AutoComponent(Mineable)
};

#endif
