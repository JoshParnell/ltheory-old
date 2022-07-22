#include "../Tasks.h"

#include "Component/MotionControl.h"

#include "Game/Object.h"

#include "LTE/Math.h"
#include "LTE/Pool.h"
#include "LTE/SDFs.h"
#include "LTE/StackFrame.h"

namespace {
  AutoClass(TaskPatrolInstance,
    Position, target)

    TaskPatrolInstance() {}
  };

  AutoClassDerived(TaskPatrol, TaskT,
    Task_Patrol_Args, args)
    DERIVED_TYPE_EX(TaskPatrol)
    POOLED_TYPE

    TaskPatrol() {}

    String GetName() const {
      return "Patrol";
    }

    String GetNoun() const {
      return "Patrol Unit";
    }

    Capability GetRateFactor() const {
      return Capability_Attack(1);
    }

    void OnBegin(Object const& self, Data& data) {
      TaskPatrolInstance it;
      it.target = self->GetPos();
      data = it;
    }

    void OnUpdate(Object const& self, float dt, Data& data) { AUTO_FRAME;
      TaskPatrolInstance& it = data.Convert<TaskPatrolInstance>();

      if (Length(self->GetPos() - it.target) < 512.0f)
        it.target = args.zone->GetPos() + SampleSphere() * args.zone->GetScale();

      // self->Broadcast(MessageBoost());
      self->GetMotionControl()->elements.push(SDF_Sphere(it.target, 1.0f));
    }
  };
}

DefineFunction(Task_Patrol) {
  return new TaskPatrol(args);
}
