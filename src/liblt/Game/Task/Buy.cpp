#include "../Tasks.h"

#include "Component/Market.h"

#include "Game/Conditions.h"
#include "Game/Items.h"
#include "Game/Messages.h"
#include "Game/Player.h"

#include "LTE/Pool.h"
#include "LTE/SDFs.h"
#include "LTE/StackFrame.h"

namespace {
  Quantity GetBuyPrice(Object const& node, Item const& item) {
    Pointer<ComponentMarket> market = node->GetMarket();
    MarketData const* d = market->GetMarketDataConst(item);
    if (!d)
      return item->GetValue();

    for (size_t i = 1; i < kMarketHistoryDepth; ++i)
      if (d->ema[i].volume > 0)
        return (Quantity)d->ema[i].price;

    return item->GetValue();
  }

  namespace TaskBuyState {
    enum Enum {
      Acting,
      Buying
    };
  }

  AutoClass(TaskBuyInstance,
    TaskBuyState::Enum, state,
    TaskInstance, task,
    Order, order)

    TaskBuyInstance() :
      state(TaskBuyState::Buying)
      {}
  };

  AutoClassDerived(TaskBuy, TaskT, Task_Buy_Args, args)
    DERIVED_TYPE_EX(TaskBuy)
    POOLED_TYPE

    TaskBuy() {}

    String GetName() const {
      return "Buy " + args.item->GetName() + " -> " + args.task->GetName();
    }

    String GetNoun() const {
      return args.task->GetNoun();
    }

    Capability GetRateFactor() const {
      return Capability_Motion(1);
    }

    Object GetTarget() const {
      return args.target;
    }

    void GetInputs(Vector<ItemDelta>& inputs) const {
      args.task->GetInputs(inputs);
      /* TODO : Feels...weird. */
      for (int i = 0; i < (int)inputs.size(); ++i) {
        if (inputs[i].item == args.item) {
          inputs.removeIndex(i);
          i--;
        }
      }
    }

    void GetOutputs(Vector<ItemDelta>& outputs) const {
      args.task->GetOutputs(outputs);
    }

    Quantity GetValue() const {
      return args.task->GetValue() - args.quantity * GetBuyPrice(args.target, args.item);
    }

    void OnBegin(Object const& self, Data& data) {
      data = TaskBuyInstance();
      TaskBuyInstance& it = data.Convert<TaskBuyInstance>();
      it.task = TaskInstance(args.task);
    }

    void OnUpdate(Object const& self, float dt, Data& data) { AUTO_FRAME;
      TaskBuyInstance& it = data.Convert<TaskBuyInstance>();

      if (it.state == TaskBuyState::Buying) {
        if (self->GetContainer() != args.target)
          self->PushTask(Task_Dock(args.target));
        else {
          Object const& locker = args.target->GetStorageLocker(self->GetOwner());
          Quantity desired = args.quantity - self->GetItemCount(args.item);
          if (desired > 0) {
            Quantity transfer = Min(desired, locker->GetItemCount(args.item));
            locker->RemoveItem(args.item, transfer);
            self->AddItem(args.item, transfer);
            desired -= transfer;
          }

          if (desired <= 0)
            it.state = TaskBuyState::Acting;
        }
      }

      if (it.state == TaskBuyState::Acting) {
        if (self->GetItemCount(args.item) == 0)
          it.state = TaskBuyState::Buying;
        else
          it.task.OnUpdate(self, dt);
      }
    }
  };
}

DefineFunction(Task_Buy) {
  return new TaskBuy(args);
}
