#include "Attachable.h"
#include "Orientation.h"

#include "Game/Object.h"

#include "LTE/Matrix.h"

void ComponentAttachable::UpdateTransform(ObjectT* self) {
  if (self->parent) {
    Pointer<ComponentOrientation> myOrientation = self->GetOrientation();
    myOrientation->transform = self->parent->GetTransform() * transform;
    // myOrientation->transform.scale = transform.scale;
    myOrientation->version++;

    if (moved) {
      moved = false;
      transform.Orthogonalize();
    }
  }
}
