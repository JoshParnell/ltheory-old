#include "../Renderables.h"

#include "Game/Materials.h"

#include "LTE/Math.h"
#include "LTE/Model.h"
#include "LTE/SDFs.h"
#include "LTE/SDFMesh.h"

const size_t kUniqueModels = 3;
namespace {
  Renderable Generate(Renderable_Ice_Args const& args) {
    static Renderable models[kUniqueModels];
    if (!models[0]) {
      for (size_t i = 0; i < kUniqueModels; ++i) {
        SDF d = SDF_Radial(SDF_FractalWorley(Rand(1, 1000), 4, 2.0f), 0.5f, 1.5f);
        models[i] = (Renderable)Model_Create()
          ->Add(SDFMesh_Create(d), Material_Ice());
      }
    }
    return models[args.seed % kUniqueModels];
  }
}

DefineFunction(Renderable_Ice) {
  return Generate(args);
}
