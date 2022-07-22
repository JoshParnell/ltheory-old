#ifndef Component_Tasks_h__
#define Component_Tasks_h__

#include "Common.h"
#include "Game/Object.h"
#include "Game/Project.h"
#include "Game/Task.h"
#include "LTE/AutoClass.h"
#include "LTE/Vector.h"

AutoClass(ComponentTasks,
  Vector<TaskInstance>, elements,
  Project, project)

  ComponentTasks() {}

  LT_API void Clear(ObjectT* self);
  LT_API void Run(ObjectT* self, UpdateState& state);
};

AutoComponent(Tasks)
  void OnUpdate(UpdateState& s) {
    Tasks.Run(this, s);
    BaseT::OnUpdate(s);
  }

  void ClearTasks() {
    Tasks.elements.clear();
  }

  TaskInstance const* GetCurrentTask() const {
    return Tasks.elements.size() ? &Tasks.elements.back() : nullptr;
  }

  void OnDeath() {
    Tasks.Clear(this);
    BaseT::OnDeath();
  }

  void OnMessage(Data& message) {
    if (Tasks.elements.size())
      Tasks.elements.back().OnMessage(this, message);
    BaseT::OnMessage(message);
  }

  void PushTask(Task const& t) {
    Tasks.elements.push(TaskInstance(t));
  }
};

#endif
