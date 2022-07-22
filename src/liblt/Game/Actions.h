#ifndef Game_Actions_h__
#define Game_Actions_h__

#include "Action.h"
#include "LTE/DeclareFunction.h"

DeclareFunction(Action_Mine, Action,
  Object, object,
  Object, target,
  Position, point)

#endif
