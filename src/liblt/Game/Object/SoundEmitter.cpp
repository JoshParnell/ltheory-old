#include "../Objects.h"

#include "Component/Drawable.h"
#include "Component/Motion.h"
#include "Component/Orientation.h"

#include "LTE/Pool.h"

#include "Module/SoundEngine.h"

typedef ObjectWrapper
  < Component_Drawable
  < Component_Motion
  < Component_Orientation
  < ObjectWrapperTail<ObjectType_SoundEmitter>
  > > > >
  SoundEmitterBaseT;

AutoClassDerived(SoundEmitter, SoundEmitterBaseT,
  float, life)
  Sound sound;

  DERIVED_TYPE_EX(SoundEmitter)
  POOLED_TYPE

  SoundEmitter() :
    life(0)
    {}

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

    life -= state.dt;
    if (life <= 0) {
      Delete();
      return;
    }
  }
};

DERIVED_IMPLEMENT(SoundEmitter)

DefineFunction(Object_SoundEmitter) {
  Reference<SoundEmitter> self = new SoundEmitter;
  self->SetPos(args.position);
  self->sound = Sound_Play3D(args.filename, self, 0, args.volume, args.distanceDiv, false);
  self->life = self->sound->GetDuration() / 1000.0f;
  return self;
}
