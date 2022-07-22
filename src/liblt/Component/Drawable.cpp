#include "Drawable.h"
#include "Cullable.h"

#include "Game/Object.h"

#include "LTE/DrawState.h"
#include "LTE/Function.h"
#include "LTE/RenderStyle.h"

void ComponentDrawable::Draw(ObjectT* self, DrawState* state) {
  if (!renderable)
    return;

  RenderStyle_Get()->SetTransform(self->GetTransform());
  // DrawState_Push("objectRadius", self->GetRadius());
  renderable()->Render(state);
  // DrawState_Pop("objectRadius");
}

VoidFreeFunction(Object_SetRenderable,
  "Set 'object's renderable to 'renderable'",
  Object, object,
  Renderable, renderable)
{
  object->GetDrawable()->renderable = renderable;
} FunctionAlias(Object_SetRenderable, SetRenderable);
