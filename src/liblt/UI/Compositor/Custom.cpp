#include "../Compositors.h"

#include "LTE/Script.h"
#include "LTE/Texture2D.h"

namespace {
  AutoClassDerived(CompositorCustom, CompositorT,
    Data, instance,
    ScriptFunction, composite,
    Compositor, base)

    CompositorCustom() {}

    void Composite(Texture2D const& layer, Mesh const& surface) {
      Texture2D result;
      composite->VoidCall(&result, instance, layer);
      base->Composite(result, surface);
    }

    void Update() {
      base->Update();
    }
  };
}

DefineFunction(Compositor_Custom) {
  Reference<CompositorCustom> self = new CompositorCustom;
  ScriptType type = args.data.type->GetAux().Convert<ScriptType>();
  self->instance = args.data;
  self->composite = type->GetFunction("Composite");
  self->base = args.base;
  return self;
}
