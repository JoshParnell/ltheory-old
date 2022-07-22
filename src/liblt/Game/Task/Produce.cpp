#include "../Tasks.h"

#include "Game/Events.h"
#include "Game/Icons.h"
#include "Game/Object.h"
#include "Game/Item/AssemblyChip.h"

#include "LTE/Pool.h"
#include "LTE/StackFrame.h"

namespace {
  /* TODO : Use Time, not float. */
  AutoClass(TaskProduceInstance,
    float, remaining,
    bool, inProgress)

    TaskProduceInstance() :
      remaining(-1),
      inProgress(false)
      {}
  };

  AutoClassDerived(TaskProduce, TaskT,
    Task_Produce_Args, args)
    DERIVED_TYPE_EX(TaskProduce)
    POOLED_TYPE

    TaskProduce() {}

    float GetDuration() const {
      return ((AssemblyChip*)args.chip.t)->duration;
    }

    Icon GetIcon() const {
      return Icon_Task_Produce();
    }

    void GetInputs(Vector<ItemDelta>& inputs) const {
      AssemblyChip* chip = (AssemblyChip*)args.chip.t;
      inputs.push(ItemDelta(chip, nullptr, 1));

      for (size_t i = 0; i < chip->requirements.size(); ++i)
        inputs.push(ItemDelta(
          chip->requirements[i].item, nullptr,
          chip->requirements[i].quantity));
    }

    String GetName() const {
      return "Produce";
    }

    String GetNoun() const {
      return "Production Lab";
    }

    void GetOutput(Vector<ItemDelta>& outputs) const {
      AssemblyChip* chip = (AssemblyChip*)args.chip.t;
      outputs.push(ItemDelta(chip->item, nullptr, 1));
    }

    Capability GetRateFactor() const {
      return Capability_Production(1);
    }

    void OnBegin(Object const& self, Data& data) {
      data = TaskProduceInstance();
    }

    void OnUpdate(Object const& self, float dt, Data& data) { AUTO_FRAME;
      TaskProduceInstance& it = data.Convert<TaskProduceInstance>();
      Pointer<ObjectT> root = self->GetRoot();
      AssemblyChip* chip = (AssemblyChip*)args.chip.t;

      if (!it.inProgress) {
        if (!root->GetItemCount(chip))
          return;

        for (size_t i = 0; i < chip->requirements.size(); ++i)
          if (root->GetItemCount(chip->requirements[i].item) <
              chip->requirements[i].quantity)
            return;

        root->RemoveItem(chip, 1);
        for (size_t i = 0; i < chip->requirements.size(); ++i)
          root->RemoveItem(chip->requirements[i].item, chip->requirements[i].quantity);

        it.inProgress = true;
        it.remaining = GetDuration();
      }

      if (it.inProgress) {
        it.remaining -= dt;

        if (it.remaining <= 0) {
          it.inProgress = false;
          root->AddItem(chip->item, 1, true);
        }
      }
    }
  };
}

DefineFunction(Task_Produce) {
  return new TaskProduce(args);
}
