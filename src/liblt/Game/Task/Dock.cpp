#include "../Tasks.h"

#include "Game/Conditions.h"
#include "Game/Items.h"
#include "Game/Player.h"

#include "LTE/Pool.h"
#include "LTE/SDF.h"
#include "LTE/StackFrame.h"

const Distance kDockDistance = 100.0;

namespace {
  AutoClassDerived(TaskDock, TaskT, Task_Dock_Args, args)
    DERIVED_TYPE_EX(TaskDock)
    POOLED_TYPE

    TaskDock() {}

    String GetName() const {
      return "Dock";
    }

    Capability GetRateFactor() const {
      return Capability_Motion(1);
    }

    Object GetTarget() const {
      return args.target;
    }

    bool IsFinished(Object const& self, Data const&) const {
      return self->GetContainer() == args.target;
    }

    void OnUpdate(Object const& self, float dt, Data&) { AUTO_FRAME;
      if (Condition_Nearby(self, args.target, kDockDistance))
        args.target->Dock(self);
      else
        self->PushTask(Task_Goto(args.target, 0.9 * kDockDistance));
    }
  };
}

DefineFunction(Task_Dock) {
  return new TaskDock(args);
}
