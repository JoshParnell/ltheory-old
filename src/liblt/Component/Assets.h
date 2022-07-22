#ifndef Component_Assets_h__
#define Component_Assets_h__

#include "Common.h"
#include "Game/Capability.h"
#include "Game/Object.h"
#include "LTE/AutoClass.h"
#include "LTE/Vector.h"

AutoClass(ComponentAssets,
  Vector<Object>, elements)

  ComponentAssets() {}
  
  LT_API ~ComponentAssets();

  LT_API void Add(ObjectT* self, Object const& asset);
  LT_API void Remove(ObjectT* self, Object const& asset);
};

AutoComponent(Assets)
  void AddAsset(Object const& asset) {
    Assets.Add(this, asset);
  }
  
  void RemoveAsset(Object const& asset) {
    Assets.Remove(this, asset);
  }

  Capability GetCapability() const {
    Capability total = BaseT::GetCapability();
    for (size_t i = 0; i < Assets.elements.size(); ++i)
      total += Assets.elements[i]->GetCapability();
    return total;
  }
};

#endif
