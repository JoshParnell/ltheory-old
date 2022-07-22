#include "BoundingBox.h"
#include "Orientation.h"
#include "Cullable.h"
#include "Drawable.h"

#include "Game/Object.h"

#include "LTE/Matrix.h"
#include "LTE/Model.h"

void ComponentBoundingBox::Recompute(ObjectT const* self) {
  ComponentOrientation const& orientation = *self->GetOrientation();
  ComponentDrawable const& drawable = *self->GetDrawable();

  if (drawable.renderable) {
    short newVersion = drawable.renderable()->GetVersion();
    if (modelVersion != newVersion) {
      modelVersion = newVersion;
      orientationVersion = -1;
    }
  }

  if (orientation.version != orientationVersion) {
    orientationVersion = orientation.version;

    /* TODO : Cleaner way than extracting matrix from frame? */
    worldBox = self->GetLocalBound();
    worldBox = worldBox.GetTransformed(orientation.transform.GetMatrix());
    radius = worldBox.GetRadius();

    /* Recompute the cull distance if necessary. */
    ComponentCullable const* cullable = self->GetCullable();
    if (cullable)
      cullable->Recompute(self);
  }
}

FreeFunction(Bound3D, Object_GetBound,
  "Return the world-space bounding box of 'object'",
  Object, object)
{
  return object->GetBoundingBox()->worldBox;
} FunctionAlias(Object_GetBound, GetBound);
