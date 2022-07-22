#include "../RenderPasses.h"

#include "LTE/Vector.h"

namespace {
  struct Composite : public RenderPassT {
    Vector<RenderPass> passes;
    DERIVED_TYPE_EX(Composite)
  
    Composite() {}

    Composite(Vector<RenderPass> const& passes) : passes(passes) {}

    char const* GetName() const {
      return "Composite";
    }

    void OnRender(DrawState* state) {
      for (size_t i = 0; i < passes.size(); ++i)
        passes[i]->Render(state);
    }
  };
}

DefineFunction(RenderPass_Composite) {
  return new Composite(args.passes);
}
