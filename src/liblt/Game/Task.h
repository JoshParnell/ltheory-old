#ifndef Game_Task_h__
#define Game_Task_h__

#include "Common.h"
#include "Capability.h"
#include "Item.h"
#include "Object.h"
#include "UI/Icon.h"

#include "LTE/Data.h"
#include "LTE/Reference.h"

AutoClass(ItemDelta,
  Item, item,
  Object, node,
  Quantity, delta)

  ItemDelta() :
    delta(0)
    {}
};

struct TaskT : public RefCounted {
  BASE_TYPE(TaskT)

  /* How many cycles of this task could be performed with the given
   * resources? */
  LT_API Quantity CanPerform(Object const& owner) const;

  /* Is this task an act of friendliness or hostility towards the target?
   *  1.0 = Maximally Friendly (e.g. Repairing)
   *  0.0 = Neutral (e.g. Following)
   * -1.0 = Maximally Hostile (e.g. Attacking) */
  virtual float GetAlignment() const {
    return 0;
  }

  virtual float GetDuration() const {
    return 1;
  }

  virtual Icon GetIcon() const {
    return nullptr;
  }

  virtual String GetName() const = 0;

  virtual String GetNoun() const {
    return GetName();
  }

  /* Inputs are consumed by the task. */
  virtual void GetInputs(Vector<ItemDelta>&) const {}

  /* Outputs are produced by the task. */
  virtual void GetOutputs(Vector<ItemDelta>&) const {}

  /* The rate of a task scales reciprocally with the sum of the dependent
   * rate capabilities of the agent implementing the task. */
  virtual Capability GetRateFactor() const {
    return Capability();
  }

  /* A task may require some 'activation' capability that is required for
   * an agent to perform it, but that doesn't influence the rate. */
  virtual Capability GetRequirements() const {
    return Capability();
  }

  /* The magnitude of a task scales linearly with the sum of the dependent
   * scaling capabilities of the agent implementing the task. */
  virtual Capability GetScaleFactor() const {
    return Capability();
  }

  /* What is the object receiving the action of this task? */
  LT_API virtual Object GetTarget() const;

  /* How many credits does the task produce? */
  virtual Quantity GetValue() const {
    return 0;
  }

  virtual bool IsFinished(Object const& self, Data const& data) const {
    return false;
  }

  virtual void OnBegin(Object const& self, Data& data) {}

  virtual void OnEnd(Object const& self, Data& data) {}

  virtual void OnMessage(Object const& self, Data& data, Data& message) {}

  virtual void OnUpdate(Object const& self, float dt, Data& data) = 0;

  FIELDS {}
};

AutoClass(TaskInstance,
  Task, task,
  Data, data,
  float, duration)

  TaskInstance() {}

  TaskInstance(Task const& task) :
    task(task),
    duration(0.0f)
    {}

  bool IsFinished(Object const& self) {
    return duration > 0.0f
      ? task->IsFinished(self, data)
      : false;
  }

  void OnMessage(Object const& self, Data& message) {
    if (duration > 0.0f)
      task->OnMessage(self, data, message);
  }

  void OnUpdate(Object const& self, float dt) {
    if (dt <= 0)
      return;
    if (!task)
      return;

    if (!duration)
      task->OnBegin(self, data);

    task->OnUpdate(self, dt, data);
    duration += dt;
  }
};

#endif
