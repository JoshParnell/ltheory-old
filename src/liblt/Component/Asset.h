#ifndef Component_Asset_h__
#define Component_Asset_h__

#include "Common.h"
#include "Game/Player.h"
#include "LTE/AutoClass.h"
#include "LTE/Pointer.h"

AutoClass(ComponentAsset,
  Player, owner)

  ComponentAsset() {}

  LT_API void Run(ObjectT* self, UpdateState& state);
};

AutoComponent(Asset)
  void OnUpdate(UpdateState& s) {
    Asset.Run(this, s);
    BaseT::OnUpdate(s);
  }

  Player const& GetOwner() const {
    return Asset.owner;
  }

  void SetOwner(Player const& owner) {
    if (Asset.owner)
      Asset.owner->RemoveAsset(this);
    owner->AddAsset(this);
  }
};

#endif
