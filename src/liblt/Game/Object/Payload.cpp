#include "../Objects.h"

#include "Component/Drawable.h"
#include "Component/Interior.h"
#include "Component/Motion.h"
#include "Component/Orientation.h"
#include "Component/Queryable.h"

#include "Game/Light.h"
#include "Game/Renderables.h"

#include "LTE/Math.h"
#include "LTE/Pool.h"

#include "Module/SoundEngine.h"

typedef ObjectWrapper
  < Component_Drawable
  < Component_Motion
  < Component_Orientation
  < ObjectWrapperTail<ObjectType_Payload>
  > > > >
  PayloadBaseT;

AutoClassDerived(Payload, PayloadBaseT,
  LightRef, light,
  Item, payload,
  Object, source,
  V3, thrust)

  DERIVED_TYPE_EX(Payload)
  POOLED_TYPE
  
  Payload() {}

  Payload(
      Item const& payload,
      Object const& source,
      V3 const& thrust) :
    payload(payload),
    source(source),
    thrust(thrust)
    {}

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

    Motion.force += thrust;

    if (!light)  {
      light = Light_Create(this);
      light->radius = 10.0f;
      light->color = Color(2.8f, 1.3f, 1.0f);
    }

    /* CRITICAL. */
    WorldRay r = WorldRay::FromPoints(GetPos(), GetPos());
    float t;
    V3 normal;
    ObjectT* collider = GetContainer()
      ->QueryInterior(r, t, 1, &normal, true, RaycastCanCollideBidirectional, this);

    if (collider) {
      Sound_Play3D("impact/metal/1.wav", nullptr, GetPos());
      Object p = payload->Instantiate(collider);
      p->SetPos(r(t));
      p->SetLook(normal);
      GetContainer()->AddInterior(p);
      Delete();
    }
  }

  bool CanCollide(ObjectT const* o) const {
    return o->GetRoot() != source;
  }
};

DERIVED_IMPLEMENT(Payload)

DefineFunction(Object_Payload) {
  Reference<Payload> self = new Payload(args.payload, args.source, args.thrust);
  self->Orientation.GetTransformW().pos = args.position;
  self->Orientation.GetTransformW().look = Normalize(args.thrust);
  self->Motion.velocity = args.velocity;
  return self;
}
