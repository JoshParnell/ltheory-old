#ifndef Game_Tasks_h__
#define Game_Tasks_h__

#include "Task.h"
#include "Object.h"
#include "Player.h"

#include "LTE/DeclareFunction.h"

DeclareFunctionArgBind(Task_Buy, Task,
  Object, target,
  Item, item,
  Quantity, quantity,
  Task, task)

DeclareFunction(Task_Custom, Task,
  Data, data)

DeclareFunctionArgBind(Task_Destroy, Task,
  Object, target)

DeclareFunctionArgBind(Task_Dock, Task,
  Object, target)

DeclareFunctionArgBind(Task_Drill, Task,
  Object, target)

DeclareFunctionArgBind(Task_Goto, Task,
  Object, target,
  Distance, distance)

DeclareFunctionArgBind(Task_LOD, Task,
  Object, owner,
  Task, task)

DeclareFunctionArgBind(Task_Mine, Task,
  Object, zone)

DeclareFunctionArgBind(Task_Mint, Task,
  Item, blueprint)

DeclareFunctionArgBind(Task_Patrol, Task,
  Object, zone)

DeclareFunctionArgBind(Task_Pirate, Task,
  Object, zone)

DeclareFunctionArgBind(Task_Play, Task,
  Player, player)

DeclareFunctionArgBind(Task_Produce, Task,
  Item, chip)

DeclareFunctionArgBind(Task_Research, Task,
  Item, blueprint)

DeclareFunctionArgBind(Task_Sell, Task,
  Object, target,
  Item, item,
  Quantity, quantity,
  Task, task)

DeclareFunctionArgBind(Task_Spawn, Task,
  Item, item,
  float, rate)

DeclareFunctionArgBind(Task_Transport, Task,
  Object, source,
  Object, dest,
  Item, item)

DeclareFunctionArgBind(Task_Wait, Task,
  float, duration)

DeclareFunction(Task_Manage, Task,
  Project, project)

#endif
