#include "Project.h"

#include "Component/Market.h"

#include "LTE/Math.h"

const float kOrderUpdateFrequency = 1.0f;

namespace {
  Quantity GetItemValue(
    Object const& self,
    Object const& node,
    Item const& item)
  {
    Pointer<ComponentMarket> market = node->GetMarket();
    MarketData const* d = market->GetMarketDataConst(item);
    if (!d)
      return item->GetValue();

    for (size_t i = 1; i < kMarketHistoryDepth; ++i)
      if (d->ema[i].volume > 0)
        return (Quantity)d->ema[i].price;

    return item->GetValue();
  }

  Quantity GetSellPrice(
    Object const& self,
    Object const& node,
    Item const& item)
  {
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

  Quantity GetInputPrice(Object const& self, Task const& task) {
    Quantity price = 0;
    static Vector<ItemDelta> inputs;
    inputs.clear();
    task->GetInputs(inputs);

    for (size_t i = 0; i < inputs.size(); ++i) {
      ItemDelta const& input = inputs[i];
      price += input.delta * GetItemValue(self, input.node, input.item);
    }

    return price;
  }

  Quantity GetOutputPrice(Object const& self, Task const& task) {
    Quantity price = 0;
    static Vector<ItemDelta> outputs;
    outputs.clear();
    task->GetOutputs(outputs);

    for (size_t i = 0; i < outputs.size(); ++i) {
      ItemDelta const& output = outputs[i];
      price += output.delta * GetItemValue(self, output.node, output.item);
    }

    return price;
  }
}

void ProjectT::Stop() {
  inProgress = false;

  /* Clean up market orders. */
  for (size_t i = 0; i < buys.size(); ++i)
    buys[i]->node->RemoveMarketBid(buys[i]);
  for (size_t i = 0; i < sells.size(); ++i)
    sells[i]->node->RemoveMarketAsk(sells[i]);
}

void ProjectT::Update(UpdateState& state) {
  metrics.duration += state.quanta;

  if (kOrderUpdateFrequency * RandExp() < state.dt)
    UpdateMarketOrders();

  /* Check for cleared sales. */ {
    for (int i = 0; i < (int)sells.size(); ++i) {
      Order& order = sells[i];
      if (order->filledVolume > 0) {
        metrics.revenue += order->filledTotal;
        wallet.accumulator += (double)settings.feedback * (double)order->filledTotal;
        order->filledTotal = 0;
        order->filledVolume = 0;
      }

      if (!order->node) {
        sells.removeIndex(i);
        i--;
      }
    }
  }

  /* Check for cleared buys. */ {
    for (int i = 0; i < (int)buys.size(); ++i) {
      Order& order = buys[i];
      if (order->filledVolume > 0) {
        metrics.expense += order->filledTotal;
        order->filledTotal = 0;
        order->filledVolume = 0;
      }

      if (!order->node) {
        buys.removeIndex(i);
        i--;
      }
    }
  }

  /* Add feedbacked revenue to the project wallet. */ {
    if (wallet.accumulator >= 1.0) {
      Quantity amount = (Quantity)Floor(wallet.accumulator);
      if (owner->GetCredits() >= amount) {
        owner->RemoveCredits(amount);
        wallet.credits += amount;
      }

      wallet.accumulator -= (double)amount;
    }
  }

  /* Check for lost assets. Remove and update losses metric accordingly. */
  for (int i = 0; i < (int)allocated.size(); ++i) {
    ProjectAllocation& alloc = allocated[i];
    if (!alloc.object->IsAlive()) {
      metrics.losses += alloc.object->GetValue();
      inProgress -= alloc.inProgress;
      alloc.inProgress = 0;
      allocated.removeIndex(i);
      i--;
    }
  }

  /* Poll allocated assets to check for idleness; update allocation
   * records accordingly. */
  for (size_t i = 0; i < allocated.size() && inProgress > 0; ++i) {
    ProjectAllocation& a = allocated[i];
    if (a.inProgress > 0 && !a.object->GetCurrentTask()) {
      inProgress -= a.inProgress;
      a.inProgress = 0;
    }
  }

  /* Dispatch orders to idle assets. */
  if (active) {
    Quantity maxScale = task->CanPerform(owner);
    for (size_t i = 0; i < allocated.size() && maxScale > 0; ++i) {
      ProjectAllocation& a = allocated[i];
      if (a.inProgress == 0) {
        Quantity thisScale = 1;
        maxScale -= thisScale;
        inProgress += thisScale;
        a.inProgress = thisScale;
        a.object->ClearTasks();
        a.object->PushTask(task);
      }
    }
  }

  /* Update sub-projects. */ {
    for (size_t i = 0; i < children.size(); ++i)
      children[i]->Update(state);
  }
}

void ProjectT::UpdateGross() {
  Quantity input = GetInputPrice(owner, task);
  Quantity output = GetOutputPrice(owner, task);
  gross = output - input + task->GetValue();
  grossRate = (double)gross / task->GetDuration();
}

void ProjectT::UpdateMarketOrders() {
  UpdateGross();
  Quantity scale = 1;
  ItemDelta d;

  static Vector<ItemDelta> deltas;
  deltas.clear();

  /* Place buy orders for inputs. */ {
    task->GetInputs(deltas);
    for (size_t i = 0; i < deltas.size(); ++i) {
      ItemDelta const& input = deltas[i];
      Quantity volume = input.delta * scale;

      Quantity current = 0;
      for (size_t i = 0; i < buys.size(); ++i)
        if (buys[i]->item == input.item)
          current += buys[i]->volume;

      if (current < volume) {
        Quantity price = GetItemValue(owner, input.node, input.item);
        Order order = Order_Create(owner, input.item, volume, price);
        input.node->AddMarketBid(order);
        buys.push(order);
      }
    }
  }

  deltas.clear();

  /* Place sell orders for outputs. */ {
    task->GetOutputs(deltas);
    for (size_t i = 0; i < deltas.size(); ++i) {
      ItemDelta const& output = deltas[i];
      Quantity volume = output.node->GetStorageLocker(owner)->GetItemCount(output.item);
      if (volume) {
        Quantity price = GetSellPrice(owner, output.node, output.item);
        Order order = Order_Create(owner, output.item, volume, price);
        output.node->AddMarketAsk(order);
        sells.push(order);
      }
    }
  }
}

DefineFunction(Project_Allocate) {
  Project const& self = args.project;

  Pointer<ComponentTasks> tasks = args.asset->GetTasks();
  if (tasks->project)
    Project_Deallocate(tasks->project, args.asset);

  self->allocated.push(ProjectAllocation(args.asset, 0, 0));
  tasks->project = self;
} FunctionAlias(Project_Allocate, Allocate);

DefineFunction(Project_Deallocate) {
  Project const& self = args.project;

  Pointer<ComponentTasks> tasks = args.asset->GetTasks();
  LTE_ASSERT(tasks->project == self);

  for (size_t i = 0; i < self->allocated.size(); ++i) {
    if (self->allocated[i].object == args.asset) {
      self->inProgress -= self->allocated[i].inProgress;
      self->allocated.removeIndex(i);
      break;
    }
  }

  tasks->project = nullptr;
} FunctionAlias(Project_Deallocate, Deallocate);

DefineFunction(Project_Create) {
  Project self = new ProjectT;
  self->name = args.task->GetName();
  self->owner = args.owner;
  self->task = args.task;
  return self;
}
