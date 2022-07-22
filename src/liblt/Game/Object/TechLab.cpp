#include "../Objects.h"

#include "Component/Orientation.h"
#include "Component/Pluggable.h"
#include "Component/Supertyped.h"
#include "Component/Tasks.h"

#include "Game/Player.h"
#include "Game/Item/AssemblyChip.h"

#include "LTE/Pool.h"

#include "Module/SoundEngine.h"

typedef ObjectWrapper
  < Component_Orientation
  < Component_Pluggable
  < Component_Supertyped
  < Component_Tasks
  < ObjectWrapperTail<ObjectType_TechLab>
  > > > > >
  TechLabBaseT;

AutoClassDerivedEmpty(TechLab, TechLabBaseT)
  Sound sound;

  DERIVED_TYPE_EX(TechLab)
  POOLED_TYPE
  
  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

    if (!sound)
      sound = Sound_Play3D("techlab/loop.ogg",
        GetRoot().t, 0, 0,
        0.1f * GetRoot()->GetScale().GetMax(), true);
    sound->SetVolume(GetCurrentTask() == nullptr ? 0.0f : 0.1f);
  }
};

DERIVED_IMPLEMENT(TechLab)

DefineFunction(Object_TechLab) {
  Reference<TechLab> self = new TechLab;
  self->SetSupertype(args.type);
  return self;
}
