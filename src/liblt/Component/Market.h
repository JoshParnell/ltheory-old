#ifndef Component_Market_h__
#define Component_Market_h__

#include "Common.h"

#include "Game/Item.h"
#include "Game/Order.h"
#include "Game/Player.h"
#include "Game/Task.h"

#include "LTE/Array.h"
#include "LTE/AutoClass.h"
#include "LTE/Map.h"
#include "LTE/Vector.h"

const uint kMarketHistoryDepth = 27;

AutoClass(Trade,
  Time, time,
  Quantity, volume,
  Quantity, price)

  Trade() {}
};

AutoClass(MarketEMA,
  double, price,
  double, volume,
  double, demand,
  double, supply)

  MarketEMA() :
    price(0),
    volume(0),
    demand(0),
    supply(0)
    {}
};

AutoClass(MarketData,
  Item, item,
  Quantity, price,
  Vector<Order>, asks,
  Vector<Order>, bids,
  Vector<Trade>, trades,
  Quantity, intervalPrice,
  Quantity, intervalVolume,
  Quantity, intervalDemand,
  Quantity, intervalSupply,
  Array<MarketEMA>, ema)

  MarketData() :
    price(0),
    intervalPrice(0),
    intervalVolume(0),
    intervalDemand(0),
    intervalSupply(0)
  {
    ema.resize(kMarketHistoryDepth);
  }
};

typedef Map<HashT, MarketData> MarketDataMap;

AutoClass(ComponentMarket,
  MarketDataMap, elements,
  Time, timer)

  ComponentMarket() : timer(0) {}

  Quantity GetPrice(Item const& item) const {
    MarketData const* d = GetMarketDataConst(item);
    return d ? d->price : 0;
  }

  MarketData& GetMarketData(Item const& item) {
    MarketData& d = elements[item->GetHash()];
    if (!d.item)
      d.item = item;
    return d;
  }

  MarketData const* GetMarketDataConst(Item const& item) const {
    return elements.get(item->GetHash());
  }

  LT_API void AddAsk(ObjectT* self, Order const&);
  LT_API void AddBid(ObjectT* self, Order const&);
  LT_API void RemoveAsk(ObjectT* self, Order const&);
  LT_API void RemoveBid(ObjectT* self, Order const&);

  LT_API void Run(ObjectT* self, UpdateState& state);
};

AutoComponent(Market)
  void OnUpdate(UpdateState& s) {
    Market.Run(this, s);
    BaseT::OnUpdate(s);
  }

  void AddMarketAsk(Order const& order) {
    Market.AddAsk(this, order);
  }

  void AddMarketBid(Order const& order) {
    Market.AddBid(this, order);
  }

  void RemoveMarketAsk(Order const& order) {
    Market.RemoveAsk(this, order);
  }

  void RemoveMarketBid(Order const& order) {
    Market.RemoveBid(this, order);
  }
};

#endif
