#include "../Objects.h"

#include "Component/BoundingBox.h"
#include "Component/Collidable.h"
#include "Component/Cullable.h"
#include "Component/Detectable.h"
#include "Component/Drawable.h"
#include "Component/Mineable.h"
#include "Component/Orientation.h"
#include "Component/Seeded.h"

#include "Game/Renderables.h"

#include "LTE/Math.h"
#include "LTE/Pool.h"
#include "LTE/RNG.h"
#include "LTE/ShaderInstance.h"

typedef ObjectWrapper
  < Component_BoundingBox
  < Component_Collidable
  < Component_Cullable
  < Component_Drawable
  < Component_Orientation
  < Component_Seeded
  < ObjectWrapperTail<ObjectType_Asteroid>
  > > > > > > >
  AsteroidBaseT;

AutoClassDerivedEmpty(Asteroid, AsteroidBaseT)
  DERIVED_TYPE_EX(Asteroid)
  POOLED_TYPE

  void Initialize() {
    Drawable.renderable =
      // Renderable_Ice(Seeded.seed);
      Renderable_Asteroid(Seeded.seed);
  }
};

DERIVED_IMPLEMENT(Asteroid)

typedef ObjectWrapper
  < Component_BoundingBox
  < Component_Collidable
  < Component_Cullable
  < Component_Detectable
  < Component_Drawable
  < Component_Mineable
  < Component_Orientation
  < Component_Seeded
  < ObjectWrapperTail<ObjectType_AsteroidRich>
  > > > > > > > > >
  AsteroidRichBaseT;

AutoClassDerivedEmpty(AsteroidRich, AsteroidRichBaseT)
  DERIVED_TYPE_EX(AsteroidRich)
  POOLED_TYPE

  void Initialize() {
    Drawable.renderable =
      //Renderable_Ice(Seeded.seed);
      Renderable_Asteroid(Seeded.seed);
  }

  Signature GetSignature() const {
    return Signature(1.0f, 6.0f, 0.5f, 2.0f);
  }
};

DERIVED_IMPLEMENT(AsteroidRich)

DefineFunction(Object_Asteroid) {
  Reference<Asteroid> self = new Asteroid;
  self->Seeded.seed = args.seed;
  self->Initialize();
  return self;
}

DefineFunction(Object_AsteroidRich) {
  Reference<AsteroidRich> self = new AsteroidRich;

  /* TODO : Factor generating algorithm. */
  RNG rg = RNG_MTG(args.seed);
  self->Mineable.item = args.resource;
  self->Mineable.quantity = args.quantity;
  self->Mineable.phase = rg->GetDirection();
  self->Seeded.seed = args.seed;
  self->Initialize();
  return self;
}
