#ifndef Component_Orders_h__
#define Component_Orders_h__

#include "Common.h"
#include "Game/Item.h"
#include "Game/Order.h"
#include "LTE/AutoClass.h"

AutoClass(ComponentOrders,
  Vector<Order>, elements)

  ComponentOrders() {}

  void Run(ObjectT* self, UpdateState& state) {
    for (int i = 0; i < (int)elements.size(); ++i) {
      if (elements[i]->volume <= 0) {
        elements.removeIndex(i);
        i--;
      }
    }
  }
};

AutoComponent(Orders)
  void OnUpdate(UpdateState& s) {
    Orders.Run(this, s);
    BaseT::OnUpdate(s);
  }
};

#endif
