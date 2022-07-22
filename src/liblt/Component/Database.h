#ifndef Component_Database_h__
#define Component_Database_h__

#include "Common.h"
#include "Game/Item.h"
#include "LTE/AutoClass.h"
#include "LTE/Map.h"

typedef Map<Item, Quantity> DatabaseMapT;
typedef DatabaseMapT::iterator DatabaseIter;
typedef DatabaseMapT::const_iterator DatabaseIterC;

AutoClass(ComponentDatabase,
  DatabaseMapT, elements)

  ComponentDatabase() {}

  ~ComponentDatabase() {
    // CRITICAL
    // for (DatabaseIter it = elements.begin(); it != elements.end(); ++it)
    //  it->first->copies -= it->second;
  }

  void Mod(Item const& data, Quantity quantity) {
    // data->copies += quantity;
    Quantity& q = elements[data];
    q += quantity;
    if (q == 0)
      elements.erase(data);
  }
  
  Quantity GetCount(Item const& item) const {
    Quantity const* count = elements.get(item);
    return count ? *count : 0;
  }
};

AutoComponent(Database)
  bool AddItem(Item const& item, Quantity count, bool force = false) {
    if (item->GetStorageType() == StorageType_Database) {
      Database.Mod(item, count);
      return true;
    } else
      return BaseT::AddItem(item, count, force);
  }

  Quantity GetItemCount(Item const& item) const {
    return item->GetStorageType() == StorageType_Database
      ? Database.GetCount(item)
      : BaseT::GetItemCount(item);
  }
};

#endif
