#include "../Objects.h"

#include "Component/BoundingBox.h"
#include "Component/Drawable.h"
#include "Component/Orientation.h"

#include "LTE/Pool.h"

typedef ObjectWrapper
  < Component_BoundingBox
  < Component_Drawable
  < Component_Orientation
  < ObjectWrapperTail<ObjectType_Static>
  > > > >
  StaticBaseT;

AutoClassDerivedEmpty(Static, StaticBaseT)
  DERIVED_TYPE_EX(Static)
  POOLED_TYPE
};

DERIVED_IMPLEMENT(Static)

Object Object_Static(Generic<Renderable> const& renderable) {
  Reference<Static> self = new Static;
  if (renderable)
    self->Drawable.renderable = renderable;
  return self;
}
