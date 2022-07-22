#include "../Compositors.h"

#include "LTE/Math.h"
#include "LTE/Mesh.h"
#include "LTE/Renderer.h"
#include "LTE/Shader.h"
#include "LTE/Texture2D.h"

#include "Module/FrameTimer.h"

namespace {
  AutoClassDerived(CompositorBasic, CompositorT,
    float, lines,
    float, noise,
    V3, gradeBlue,
    float, age)
    Shader shader;

    CompositorBasic() {}

    DefineInitializer {
      shader = Shader_Create("identity.jsl", "ui/basic.jsl");
    }

    void Composite(Texture2D const& layer, Mesh const& surface) {
      RendererState s(BlendMode::Alpha, CullMode::Backface, false, false);
      (*shader)
        ("age", age)
        ("layer", layer)
        ("linesMag", lines)
        ("gradeBlue", gradeBlue)
        ("noiseMag", noise)
        ("seed", Rand())
        ("size", V2(layer->GetWidth(), layer->GetHeight()));
      Renderer_SetShader(*shader);
      surface->Draw();
    }

    void Update() {
      age += FrameTimer_Get();
    }
  };
}

DefineFunction(Compositor_Basic) {
  return new CompositorBasic(args.lines, args.noise, args.gradeBlue, 0);
}
