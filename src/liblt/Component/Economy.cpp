#include "Economy.h"
#include "Cargo.h"
#include "Interior.h"
#include "Market.h"
#include "Mineable.h"
#include "Resources.h"

#include "Game/Tasks.h"

#include "LTE/HashSet.h"
#include "LTE/Map.h"
#include "LTE/StackFrame.h"

namespace {
  void CacheItem(
    ComponentEconomy* economy,
    Item const& item,
    HashSet<HashT>& itemSet)
  {
    HashT hash = item->GetHash();
    if (!itemSet.contains(hash)) {
      itemSet.insert(hash);
      economy->items.push(item);

      if (item->GetType() == ItemType_AssemblyChip)
        economy->tasks.push(Task_Produce(item));
      if (item->GetType() == ItemType_Blueprint) {
        economy->tasks.push(Task_Mint(item));
        economy->tasks.push(Task_Research(item));
      }
    }
  }

  void CacheTask(ComponentEconomy* economy, Task const& t) {
    economy->tasks.push(t);
  }

  void CacheCargo(
    ComponentEconomy* economy,
    Object const& o,
    ComponentCargo* cargo,
    HashSet<HashT>& itemSet)
  {
    /* TODO : Looting. */
    for (CargoIter it = cargo->elements.begin();
         it != cargo->elements.end(); ++it)
      CacheItem(economy, it->first, itemSet);
  }

  void CacheMarket(
    ComponentEconomy* economy,
    Object const& o,
    ComponentMarket* market,
    HashSet<HashT>& itemSet)
  {
    economy->nodes.push(o);
    for (MarketDataMap::iterator it = market->elements.begin();
         it != market->elements.end(); ++it)
      CacheItem(economy, it->second.item, itemSet);
  }

  void CacheZone(
    ComponentEconomy* economy,
    Object const& zone,
    HashSet<HashT>& itemSet)
  {
    ComponentResources* res = zone->GetResources();
    if (res) {
      for (size_t i = 0; i < res->elements.size(); ++i)
        CacheItem(economy, res->elements.entries[i].key, itemSet);

      CacheTask(economy, Task_Mine(zone));
      CacheTask(economy, Task_Pirate(zone));
    }
  }

  void CacheObject(
    ComponentEconomy* economy,
    Object const& object,
    HashSet<HashT>& itemSet)
  {
    Pointer<ComponentCargo> cargo = object->GetCargo();
    if (cargo)
      CacheCargo(economy, object, cargo, itemSet);

    Pointer<ComponentMarket> market = object->GetMarket();
    if (market)
      CacheMarket(economy, object, market, itemSet);

    Pointer<ComponentZoned> zoned = object->GetZoned();
    if (zoned)
      CacheZone(economy, object, itemSet);

    for (Object child = object->children; child; child = child->nextSibling)
      CacheObject(economy, child, itemSet);
  }
}

void ComponentEconomy::Run(ObjectT* self, UpdateState&) {
  SFRAME("Economy Caching");
  static HashSet<HashT> itemSet;
  itemSet.clear();

  nodes.clear();
  items.clear();
  itemSet.clear();
  tasks.clear();

  for (InteriorIterator it = Object_GetInteriorObjects(self);
       it.HasMore(); it.Advance())
    CacheObject(this, it.Get(), itemSet);

  /* Generate trade routes. */
  for (size_t i = 0; i < nodes.size(); ++i) {
    Object const& a = nodes[i];
    ComponentMarket* marketA = a->GetMarket();

    for (size_t j = i + 1; j < nodes.size(); ++j) {
      Object const& b = nodes[j];
      ComponentMarket* marketB = b->GetMarket();

      /* A -> B. */
      for (MarketDataMap::iterator it = marketA->elements.begin();
           it != marketA->elements.end(); ++it)
      {
        CacheTask(this, Task_Transport(a, b, it->second.item));
        CacheTask(this, Task_Transport(b, a, it->second.item));
      }

      /* B -> A. */
      for (MarketDataMap::iterator it = marketB->elements.begin();
           it != marketB->elements.end(); ++it)
      {
        CacheTask(this, Task_Transport(a, b, it->second.item));
        CacheTask(this, Task_Transport(b, a, it->second.item));
      }
    }
  }
}
