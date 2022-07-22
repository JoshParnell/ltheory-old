#ifndef Planet_h__
#define Planet_h__

#include "../ObjectWrapper.h"

#include "Component/BoundingBox.h"
#include "Component/Collidable.h"
#include "Component/Cullable.h"
#include "Component/Detectable.h"
#include "Component/Drawable.h"
#include "Component/Nameable.h"
#include "Component/Orientation.h"
#include "Component/Resources.h"
#include "Component/Seeded.h"
#include "Component/Supertyped.h"

#include "Game/Order.h"
#include "Game/Player.h"

typedef ObjectWrapper
  < Component_BoundingBox
  < Component_Collidable
  < Component_Cullable
  < Component_Detectable
  < Component_Drawable
  < Component_Nameable
  < Component_Orientation
  < Component_Resources
  < Component_Seeded
  < Component_Supertyped
  < ObjectWrapperTail<ObjectType_Planet>
  > > > > > > > > > > >
  PlanetBaseT;

struct Planet : public PlanetBaseT {
  Object orbitalStation;
  Player manager;

  Color surfaceColor1;
  Color surfaceColor2;
  Color atmoTint;
  float cloudLevel;
  float atmoDensity;
  float heightMult;
  float oceanLevel;
};

#endif
