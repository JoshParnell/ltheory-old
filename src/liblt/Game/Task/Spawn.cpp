#include "../Tasks.h"

#include "Game/Object.h"

#include "LTE/Math.h"
#include "LTE/Pool.h"
#include "LTE/StackFrame.h"

namespace {
  AutoClassDerived(TaskSpawn, TaskT, Task_Spawn_Args, args)
    DERIVED_TYPE_EX(TaskSpawn)
    POOLED_TYPE

    TaskSpawn() {}

    float GetDuration() const {
      return args.rate;
    }

    String GetName() const {
      return "Spawn";
    }

    String GetNoun() const {
      return "Spawner";
    }

    void GetOutput(Vector<ItemDelta>& outputs) const {
      outputs.push(ItemDelta(args.item, nullptr, 1));
    }

    void OnUpdate(Object const& self, float dt, Data& data) { AUTO_FRAME;
      /* TODO */
#if 0
      while (args.rate * RandExp() < dt)
        self->GetRoot()->AddItem(args.item, 1);
#endif
    }
  };
}

DefineFunction(Task_Spawn) {
  return new TaskSpawn(args);
}
