#include "../RenderPasses.h"

#include "Game/Camera.h"
#include "Game/Object.h"

#include "Component/BoundingBox.h"
#include "Component/Cullable.h"
#include "Component/Interior.h"

#include "LTE/DrawState.h"
#include "LTE/Iterator.h"
#include "LTE/View.h"

namespace {
  struct Visibility : public RenderPassT {
    DERIVED_TYPE_EX(Visibility)

    void CheckVisibility(ObjectT* self, DrawState* state) {
      if (IsVisible(self, state))
        state->visible.push((void*)self);
      if (self->GetType() == ObjectType_Light)
        state->lights.push((void*)self);

      LIST_ITERATE(Object, self->children, nextSibling) {
        CheckVisibility(*it, state);
      }
    }

    char const* GetName() const {
      return "Visibility Pass";
    }

    bool IsVisible(ObjectT* self, DrawState* state) {
      ComponentCullable* c = self->GetCullable();
      if (!c)
        return true;

      c->Recompute(self);

      /* First, perform distance culling. */
      Transform const& transform = self->GetTransform();
      Distance d2 = LengthSquared(transform.pos - state->view->transform.pos);
      if (d2 > c->cullDistanceSquared)
        return false;

      ComponentBoundingBox* bb = self->GetBoundingBox();
      if (!bb)
        return true;

      return state->view->CanSee(self->GetGlobalBound());
    }

    void OnRender(DrawState* state) {
      ObjectT* container = Camera_Get()->GetContainer();
      state->lights.clear();
      state->visible.clear();
      state->visible.push((void*)container);

      for (ObjectType type = 0; type < ObjectType_SIZE; ++type)
        for (InteriorTypeIterator it = Object_GetInteriorObjects(container, type);
             it.HasMore(); it.Advance())
          CheckVisibility(it.Get(), state);
    }
  };
}

DefineFunction(RenderPass_Visibility) {
  return new Visibility;
}
