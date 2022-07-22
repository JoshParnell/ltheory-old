#include "Tasks.h"
#include "Game/Tasks.h"
#include "LTE/Function.h"

void ComponentTasks::Clear(ObjectT* self) {
  while (elements.size()) {
    TaskInstance& current = elements.back();
    current.task->OnEnd(self, current.data);
    elements.pop();
  }
}

void ComponentTasks::Run(ObjectT* self, UpdateState& state) {
  if (elements.size() && !elements.back().IsFinished(self))
    elements.back().OnUpdate(self, state.dt);

  while (elements.size() && elements.back().IsFinished(self)) {
    TaskInstance& current = elements.back();
    current.task->OnEnd(self, current.data);
    elements.pop();
  }
}

VoidFreeFunction(Object_ClearTasks,
  "Clear all tasks from 'object's task stack",
  Object, object)
{
  object->ClearTasks();
} FunctionAlias(Object_ClearTasks, ClearTasks);

FreeFunction(TaskInstance*, Object_GetCurrentTask,
  "Return 'object's active task",
  Object, object)
{
  return (TaskInstance*)object->GetCurrentTask();
} FunctionAlias(Object_GetCurrentTask, GetCurrentTask);

VoidFreeFunction(Object_PushTask,
  "Push 'task' to 'object's task stack",
  Object, object,
  Task, task)
{
  object->PushTask(task);
} FunctionAlias(Object_PushTask, PushTask);
