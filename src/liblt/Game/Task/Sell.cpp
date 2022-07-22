#include "../Tasks.h"

#include "Component/Market.h"

#include "Game/Conditions.h"
#include "Game/Items.h"
#include "Game/Messages.h"
#include "Game/Player.h"

#include "LTE/Math.h"
#include "LTE/Pool.h"
#include "LTE/SDFs.h"
#include "LTE/StackFrame.h"

namespace {
  Quantity GetSellPrice(Object const& node, Item const& item) {
    Pointer<ComponentMarket> market = node->GetMarket();
    MarketData const* d = market->GetMarketDataConst(item);
    if (!d)
      return item->GetValue();

    for (size_t i = 5; i < kMarketHistoryDepth; ++i) {
      if (d->ema[i].volume > 0) {
        double priceMean = d->ema[i].price;
        double priceVariance = 1.0 + 10.0 / (1.0 + d->ema[i].volume / 100.0);
        double t = RandExp();
        return (Quantity)Max(1.0, priceMean * (0.9 + t * priceVariance));
      }
    }

    return item->GetValue();
  }

  namespace TaskSellState {
    enum Enum {
      Acting,
      Selling
    };
  }

  AutoClass(TaskSellInstance,
    TaskSellState::Enum, state,
    TaskInstance, task,
    Order, order)

    TaskSellInstance() :
      state(TaskSellState::Acting)
      {}
  };

  AutoClassDerived(TaskSell, TaskT, Task_Sell_Args, args)
    DERIVED_TYPE_EX(TaskSell)
    POOLED_TYPE

    TaskSell() {}

    String GetName() const {
      return args.task->GetName() + " -> Sell " + args.item->GetName();
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
    }

    void GetOutputs(Vector<ItemDelta>& outputs) const {
      args.task->GetOutputs(outputs);
      /* TODO : Feels...weird. */
      for (int i = 0; i < (int)outputs.size(); ++i) {
        if (outputs[i].item == args.item) {
          outputs.removeIndex(i);
          i--;
        }
      }
    }

    Quantity GetValue() const {
      return args.task->GetValue() + args.quantity * GetSellPrice(args.target, args.item);
    }

    void OnBegin(Object const& self, Data& data) {
      data = TaskSellInstance();
      TaskSellInstance& it = data.Convert<TaskSellInstance>();
      it.task = TaskInstance(args.task);
    }

    void OnUpdate(Object const& self, float dt, Data& data) { AUTO_FRAME;
      TaskSellInstance& it = data.Convert<TaskSellInstance>();

      Player const& owner = self->GetOwner();

      /* TODO : This breaks metrics. */
      /* Manage the market order. */ {
        Object const& locker = args.target->GetStorageLocker(owner);
        Quantity quantity = locker->GetItemCount(args.item);

        if (quantity > 0) {
          if (it.order && it.order->node)
            it.order->node->RemoveMarketAsk(it.order);

          it.order = Order_Create(owner, args.item,
            locker->GetItemCount(args.item),
            GetSellPrice(args.target, args.item));

          args.target->AddMarketAsk(it.order);
        }
      }

      if (it.state == TaskSellState::Selling) {
        if (self->GetContainer() != args.target)
          self->PushTask(Task_Dock(args.target));
        else {
          Object const& locker = args.target->GetStorageLocker(owner);
          Quantity quantity = self->GetItemCount(args.item);
          locker->AddItem(args.item, quantity);
          self->RemoveItem(args.item, quantity);
          it.state = TaskSellState::Acting;
        }
      }

      if (it.state == TaskSellState::Acting) {
        if (self->GetItemCount(args.item) >= args.quantity)
          it.state = TaskSellState::Selling;
        else
          it.task.OnUpdate(self, dt);
      }
    }

    void OnEnd(Object const& self, Data& data) {
      TaskSellInstance& it = data.Convert<TaskSellInstance>();
      if (it.order && it.order->node)
        it.order->node->RemoveMarketAsk(it.order);
    }
  };
}

DefineFunction(Task_Sell) {
  return new TaskSell(args);
}
