#include "../Tasks.h"

#include "Game/Items.h"
#include "LTE/Pool.h"
#include "LTE/StackFrame.h"

#include "LTE/Debug.h"

namespace {
  AutoClass(TaskLODInstance,
    Quantity, scale,
    float, timeToCompletion)

    TaskLODInstance() :
      scale(0),
      timeToCompletion(0)
      {}
  };

  AutoClassDerived(TaskLOD, TaskT, Task_LOD_Args, args)
    DERIVED_TYPE_EX(TaskLOD)
    POOLED_TYPE

    TaskLOD() {}

    float GetAlignment() const {
      return args.task->GetAlignment();
    }

    float GetDuration() const {
      return args.task->GetDuration();
    }

    String GetName() const {
      return "LOD Task";
    }

    void GetInputs(Vector<ItemDelta>& inputs) const {
      args.task->GetInputs(inputs);
    }

    void GetOutputs(Vector<ItemDelta>& outputs) const {
      args.task->GetOutputs(outputs);
    }

    Capability GetRateFactor() const {
      return args.task->GetRateFactor();
    }

    Capability GetRequirements() const {
      return args.task->GetRequirements();
    }

    Capability GetScaleFactor() const {
      return args.task->GetScaleFactor();
    }

    Object GetTarget() const {
      return args.task->GetTarget();
    }

    void OnBegin(Object const& self, Data& data) {
      data = TaskLODInstance();
    }

    void OnUpdate(Object const& self, float dt, Data& data) { AUTO_FRAME;
      TaskLODInstance& it = data.Convert<TaskLODInstance>();

      it.timeToCompletion -= dt;
      if (it.timeToCompletion <= 0) {
        ItemDelta d;

        /* Finish the old cycle. */
        static Vector<ItemDelta> deltas;
        deltas.clear();

        if (it.scale > 0) {
          args.task->GetOutputs(deltas);
          for (size_t i = 0; i < deltas.size(); ++i) {
            ItemDelta const& output = deltas[i];
            Quantity change = (Quantity)(it.scale * output.delta);
            output.node->GetStorageLocker(args.owner)->AddItem(output.item, change);
          }
        }

        /* Start a new cycle. */
        it.scale = args.task->CanPerform(args.owner);
        it.timeToCompletion = args.task->GetDuration();

        deltas.clear();
        if (it.scale > 0) {
          args.task->GetInputs(deltas);
          it.scale = 1; // CRITICAL :T
          for (size_t i = 0; i < deltas.size(); ++i) {
            ItemDelta const& input = deltas[i];
            Quantity change = it.scale * input.delta;
            input.node->GetStorageLocker(args.owner)->RemoveItem(input.item, change);
          }
        }
      }
    }
  };
}

DefineFunction(Task_LOD) {
  return new TaskLOD(args);
}
