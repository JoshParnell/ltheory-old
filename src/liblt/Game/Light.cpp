#include "Light.h"
#include "Objects.h"

#include "LTE/Pool.h"

AutoClassDerivedEmpty(LightImpl, Light)
  DERIVED_TYPE_EX(LightImpl)
  POOLED_TYPE
};

DERIVED_IMPLEMENT(LightImpl)

LightRef Light_Create(ObjectT* parent) {
  LightRef self = new LightImpl;
  parent->AddChild(self);
  return self;
}

DefineFunction(Object_Light) {
  LightRef self = new LightImpl;
  self->color = args.color;
  self->radius = args.radius;
  self->flare = args.lensFlare;
  return self;
}
