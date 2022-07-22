#ifndef Game_Item_h__
#define Game_Item_h__

#include "Common.h"
#include "AI/Traits.h"
#include "UI/Common.h"

#include "LTE/AutoClass.h"
#include "LTE/BaseType.h"
#include "LTE/Reference.h"
#include "LTE/String.h"
#include "LTE/V3.h"

const double kInertiaFactor = 3.75 / 3.0;

struct ItemT : public RefCounted {
  BASE_TYPE(ItemT)

  ItemID id;

  LT_API ItemT();

#define X(type, name, default)                                                 \
  LT_API virtual type const& Get##name() const;                                \
  LT_API virtual bool Has##name() const { return false; }
  ITEMPROPERTY_X
#undef X

  virtual uint GetDockCapacity() const {
    return 0;
  }

  virtual Vector<Bound3> const* GetDocks() const {
    return 0;
  }

  ItemID GetID() const {
    return id;
  }

  virtual float GetLevel() const {
    return 0;
  }

  virtual Mass GetInertia() const {
    return (Mass)Pow((double)GetMass(), kInertiaFactor);
  }

  virtual V3 GetOffset() const {
    return 0;
  }

  virtual uint GetSkillEngineering() const {
    return 0;
  }

  virtual uint GetSkillMining() const {
    return 0;
  }

  virtual uint GetSkillPiloting() const {
    return 0;
  }

  virtual Array<Socket> const* GetSockets() const {
    return 0;
  }

  virtual SocketType GetSocketType() const {
    return SocketType_SIZE;
  }

  virtual String GetSound() const {
    return "";
  }

  virtual ItemT* GetSuperType() const {
    return nullptr;
  }

  virtual Quantity GetUses() const {
    return 0;
  }

  /* Is the item a descendent of the given ancestor item? */
  LT_API bool IsType(Item const& type) const;

  LT_API virtual Object Instantiate(ObjectT* parent = 0);

  FIELDS {
    MAPFIELD(id)
  }
};

AutoClass(ItemQuantity,
  Item, item,
  Quantity, quantity)
  ItemQuantity() {}
};

#endif
