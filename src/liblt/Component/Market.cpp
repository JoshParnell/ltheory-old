#include "Market.h"
#include "Orders.h"

#include "Game/Player.h"

#include <algorithm>

const Time kMarketIntervalDuration = 1000;

namespace {
  inline bool SortAsks(Order const& a, Order const& b) {
    return
       a->price < b->price ||
      (a->price == b->price && a->volume > b->volume);
  }

  inline bool SortBids(Order const& a, Order const& b) {
    return
       a->price > b->price ||
      (a->price == b->price && a->volume > b->volume);
  }
}

void ComponentMarket::AddAsk(ObjectT* self, Order const& order) {
  Object const& locker = self->GetStorageLocker(order->owner);
  if (locker->GetItemCount(order->item) >= order->volume) {
    order->node = self;
    locker->RemoveItem(order->item, order->volume);
    GetMarketData(order->item).asks.push(order);
    order->owner->GetOrders()->elements.push(order);
  }
}

void ComponentMarket::AddBid(ObjectT* self, Order const& order) {
  Quantity total = order->price * order->volume;
  if (order->owner->GetCredits() >= total) {
    order->node = self;
    order->owner->RemoveCredits(total);
    GetMarketData(order->item).bids.push(order);
    order->owner->GetOrders()->elements.push(order);
  }
}

void ComponentMarket::RemoveAsk(ObjectT* self, Order const& order) {
  if (order->node == self &&
      GetMarketData(order->item).asks.remove(order))
  {
    order->node = nullptr;
    Object const& locker = self->GetStorageLocker(order->owner);
    locker->AddItem(order->item, order->volume);
  }
}

void ComponentMarket::RemoveBid(ObjectT* self, Order const& order) {
  if (order->node == self &&
      GetMarketData(order->item).bids.remove(order))
  {
    order->node = nullptr;
    order->owner->AddCredits(order->price * order->volume);
  }
}

void ComponentMarket::Run(ObjectT* self, UpdateState& state) {
  timer += state.quanta;
  bool newInterval = timer >= kMarketIntervalDuration;

  for (MarketDataMap::iterator it = elements.begin();
       it != elements.end(); ++it)
  {
    MarketData& d = it->second;

    /* Accumulate supply & demand. */
    for (size_t i = 0; i < d.asks.size(); ++i)
      d.intervalSupply += state.quanta * d.asks[i]->volume;
    for (size_t i = 0; i < d.bids.size(); ++i)
      d.intervalDemand += state.quanta * d.bids[i]->volume;

    /* Fill orders. */ {
      std::sort(d.asks.v.begin(), d.asks.v.end(), SortAsks);
      std::sort(d.bids.v.begin(), d.bids.v.end(), SortBids);

      size_t askIndex = 0;
      size_t bidIndex = 0;

      while (askIndex < d.asks.size() && bidIndex < d.bids.size()) {
        Order& ask = d.asks[askIndex];
        Order& bid = d.bids[bidIndex];

        if (ask->volume == 0) {
          askIndex++;
          continue;
        }

        if (bid->volume == 0) {
          bidIndex++;
          continue;
        }

        if (ask->price > bid->price)
          break;

        Quantity tradePrice = (ask->price + bid->price) / 2;
        Quantity tradeVolume = Min(ask->volume, bid->volume);
        Quantity totalPrice = tradePrice * tradeVolume;

        ask->owner->AddCredits(totalPrice);
        Object const& locker = self->GetStorageLocker(bid->owner);
        locker->AddItem(d.item, tradeVolume);

        /* Make sure to refund the buyer if the transaction occurred at a
         * lower price (since their funds are already in escrow) */
        if (tradePrice < bid->price)
          bid->owner->AddCredits(bid->price * bid->volume - totalPrice);

        ask->volume -= tradeVolume;
        ask->filledVolume += tradeVolume;
        ask->filledTotal += totalPrice;
        bid->volume -= tradeVolume;
        bid->filledVolume += tradeVolume;
        bid->filledTotal += totalPrice;
        
        d.intervalPrice += totalPrice;
        d.intervalVolume += tradeVolume;

        d.price = tradePrice;
        d.trades.push(Trade(Universe_Age(), tradeVolume, tradePrice));
      }
    }

    /* Remove empty orders. */ {
      for (int i = 0; i < (int)d.asks.size(); ++i) {
        if (d.asks[i]->volume <= 0) {
          d.asks[i]->node = nullptr;
          d.asks.removeIndex(i);
          i--;
        }
      }

      for (size_t i = 0; i < d.bids.size(); ++i) {
        if (d.bids[i]->volume <= 0) {
          d.bids[i]->node = nullptr;
          d.bids.removeIndex(i);
          i--;
        }
      }
    }

    if (newInterval) {
      if (d.intervalVolume)
        d.intervalPrice /= d.intervalVolume;

      double thisPrice = (double)d.intervalPrice;
      double thisVolume = (double)d.intervalVolume;
      double thisDemand = (double)d.intervalDemand / (double)timer;
      double thisSupply = (double)d.intervalSupply / (double)timer;
      double emaPeriod = 1.0;

      for (size_t i = 0; i < kMarketHistoryDepth; ++i) {
        double k = 2.0 / (1.0 + emaPeriod);
        MarketEMA& ema = d.ema[i];
        ema.volume = Mix(ema.volume, thisVolume, k);
        ema.demand = Mix(ema.demand, thisDemand, k);
        ema.supply = Mix(ema.supply, thisSupply, k);
        ema.price = Mix(ema.price, thisPrice, k);
        emaPeriod *= 2.0;
      }

      d.intervalPrice = 0;
      d.intervalVolume = 0;
      d.intervalDemand = 0;
      d.intervalSupply = 0;
    }
  }

  if (newInterval)
    timer = 0;
}

AutoClass(MarketIterator,
  MarketDataMap::iterator, iterator,
  Object, object)

  MarketIterator() {}
};

FreeFunction(MarketIterator, Object_GetMarketListings,
  "Return an iterator to the market listings 'object'",
  Object, object)
{
  return MarketIterator(object->GetMarket()->elements.begin(), object);
} FunctionAlias(Object_GetMarketListings, GetMarketListings);

FreeFunction(MarketData*, MarketIterator_Access,
  "Return the value of 'iterator'",
  MarketIterator, iterator)
{
  return &iterator.iterator->second;
} FunctionAlias(MarketIterator_Access, Get);

VoidFreeFunction(MarketIterator_Advance,
  "Advance 'iterator'",
  MarketIterator, iterator)
{
  ++((MarketIterator&)iterator).iterator;
} FunctionAlias(MarketIterator_Advance, Advance);

FreeFunction(bool, MarketIterator_HasMore,
  "Return whether 'iterator' has more elements",
  MarketIterator, iterator)
{
  return iterator.iterator != iterator.object->GetMarket()->elements.end();
} FunctionAlias(MarketIterator_HasMore, HasMore);

VoidFreeFunction(Object_AddMarketSellOrder,
  "Add a sell order to 'market' with 'owner', 'item', 'volume', and 'price'",
  Object, market,
  Object, owner,
  Item, item,
  Quantity, volume,
  Quantity, price)
{
  market->AddMarketAsk(Order_Create(owner, item, volume, price));
} FunctionAlias(Object_AddMarketSellOrder, AddMarketSellOrder);

VoidFreeFunction(Object_AddMarketBuyOrder,
  "Add a buy order to 'market' with 'owner', 'item', 'volume', and 'price'",
  Object, market,
  Object, owner,
  Item, item,
  Quantity, volume,
  Quantity, price)
{
  market->AddMarketBid(Order_Create(owner, item, volume, price));
} FunctionAlias(Object_AddMarketBuyOrder, AddMarketBuyOrder);

FreeFunction(MarketData*, Object_GetMarketData,
  "Return the market data for 'item' at 'market'",
  Object, market,
  Item, item)
{
  return market->GetMarket()
    ? Mutable(market->GetMarket()->GetMarketDataConst(item))
    : nullptr;
} FunctionAlias(Object_GetMarketData, GetMarketData);
