#include "Game/Task.h"
#include "LTE/Function.h"

TypeAlias(Reference<TaskT>, Task);

FreeFunction(String, Task_GetName,
  "Return the name of 'task'",
  Task, task)
{
  return task->GetName();
} FunctionAlias(Task_GetName, GetName);
