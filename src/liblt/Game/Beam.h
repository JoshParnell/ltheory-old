#ifndef Beam_h__
#define Beam_h__

#include "Object.h"
#include "ObjectWrapper.h"

#include "Component/Damager.h"
#include "Component/Drawable.h"
#include "Component/Orientation.h"
#include "LTE/Common.h"

typedef ObjectWrapper
  < Component_Damager
  < Component_Drawable
  < Component_Orientation
  < ObjectWrapperTail<ObjectType_Beam>
  > > > >
  BeamBaseT;

struct Beam : public BeamBaseT {
  V3 origin;
  V3 direction;
  float width;
};

LT_API Beam* Beam_Create();

#endif
