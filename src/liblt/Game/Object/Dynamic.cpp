#include "../Objects.h"

#include "Component/BoundingBox.h"
#include "Component/Drawable.h"
#include "Component/Motion.h"
#include "Component/Orientation.h"

#include "LTE/Pool.h"

typedef ObjectWrapper
  < Component_BoundingBox
  < Component_Drawable
  < Component_Motion
  < Component_Orientation
  < ObjectWrapperTail<ObjectType_Dynamic>
  > > > > >
  DynamicBaseT;

AutoClassDerivedEmpty(Dynamic, DynamicBaseT)
  DERIVED_TYPE_EX(Dynamic)
  POOLED_TYPE
};

DERIVED_IMPLEMENT(Dynamic)

Object Object_Dynamic(Generic<Renderable> const& renderable) {
  Reference<Dynamic> self = new Dynamic;
  if (renderable)
    self->Drawable.renderable = renderable;
  return self;
}
