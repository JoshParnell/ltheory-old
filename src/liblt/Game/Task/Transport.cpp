#include "../Tasks.h"

#include "Game/Events.h"
#include "Game/Object.h"
#include "LTE/Pool.h"
#include "LTE/StackFrame.h"

namespace {
  AutoClassDerived(TaskTransport, TaskT, Task_Transport_Args, args)
    DERIVED_TYPE_EX(TaskTransport)
    POOLED_TYPE

    TaskTransport() {}

    /* TODO : Estimate of travel time. */
    float GetDuration() const {
      return Length(args.source->GetPos() - args.dest->GetPos());
    }

    String GetName() const {
      return "Transport";
    }

    String GetNoun() const {
      return "Transport";
    }

    void GetInputs(Vector<ItemDelta>& inputs) const {
      inputs.push(ItemDelta(args.item, args.source, 1));
    }

    void GetOutputs(Vector<ItemDelta>& outputs) const {
      outputs.push(ItemDelta(args.item, args.dest, 1));
    }

    Capability GetRateFactor() const {
      return Capability_Motion(1);
    }

    Capability GetScaleFactor() const {
      return Capability_Storage(1);
    }

    void OnUpdate(Object const& self, float dt, Data& data) { AUTO_FRAME;
      if (self->GetContainer() == args.source) {
        // Quantity quantity =
        //  (Quantity)Floor(self->GetFreeCapacity() / args.item->GetMass());
        // Event_Withdraw(self, args.source, args.item, quantity);
        self->PushTask(Task_Dock(args.dest));
      }

      else if (self->GetContainer() == args.dest) {
        // Event_Deposit(self, args.dest, args.item, self->GetItemCount(args.item));
        self->PushTask(Task_Dock(args.source));
      }

      else
        self->PushTask(Task_Dock(args.source));
    }
  };
}

DefineFunction(Task_Transport) {
  return new TaskTransport(args);
}
