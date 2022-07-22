#ifndef Component_History_h__
#define Component_History_h__

#include "Common.h"

#include "Game/Event.h"

#include "LTE/AutoClass.h"
#include "LTE/DeclareFunction.h"
#include "LTE/Vector.h"

AutoClass(ComponentHistory,
  Vector<Event>, elements)

  ComponentHistory() {}

  LT_API void Run(ObjectT*, UpdateState&);
};

AutoComponent(History)
  void OnUpdate(UpdateState& s) {
    History.Run(this, s);
    BaseT::OnUpdate(s);
  }
};

DeclareFunction(Object_AddHistory, void,
  Object, object,
  Event, event)

#endif
