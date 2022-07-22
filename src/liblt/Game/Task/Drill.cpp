#include "../Tasks.h"

#include "Component/Resources.h"

#include "Game/Object.h"

#include "LTE/Math.h"
#include "LTE/Pool.h"
#include "LTE/StackFrame.h"
#include "LTE/Smooth.h"

namespace {
  AutoClassDerived(TaskDrill, TaskT,
    Task_Drill_Args, args)
    DERIVED_TYPE_EX(TaskDrill)
    POOLED_TYPE

    TaskDrill() {}

    float GetDuration() const {
      return 1.0f;
    }

    String GetName() const {
      return "Drill";
    }

    String GetNoun() const {
      return "Drilling Rig";
    }

    void GetOutputs(Vector<ItemDelta>& outputs) const {
      Pointer<ComponentResources> resources = args.target->GetResources();
      for (size_t i = 0; i < resources->elements.size(); ++i)
        outputs.push(ItemDelta(resources->elements.entries[i].key, nullptr, 1));
    }

    Capability GetRateFactor() const {
      return Capability_Transference(1);
    }

    void OnUpdate(Object const& self, float dt, Data& data) { AUTO_FRAME;
      if (RandExp() < dt)
        self->AddItem(args.target->GetResources()->elements.sample(Rand()), 1);
    }
  };
}

DefineFunction(Task_Drill) {
  return new TaskDrill(args);
}
