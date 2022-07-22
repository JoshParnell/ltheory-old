#include "../Glyphs.h"

#include "LTE/Pool.h"
#include "LTE/Shader.h"
#include "LTE/V4.h"

namespace {
  AutoClass(GradientVertex,
    V3, p,
    V4, color1,
    V4, color2,
    V4, uvsize)
    GradientVertex() {}
  };

  AutoClassDerived(Gradient, GlyphT, Glyph_Gradient_Args, args)
    DERIVED_TYPE_EX(Gradient)
    POOLED_TYPE

    Gradient() {}

    Glyph Clone() const {
      return Glyph_Gradient(args);
    }

    Shader GetShader() const {
      return Shader_Create("widget.jsl", "ui/gradient.jsl");
    }

    Type GetVertexFormat() const {
      return Type_Get<GradientVertex>();
    }

    void Submit(void* vvertices, GlyphState const& s) const {
      GradientVertex* vertices = (GradientVertex*)vvertices;

      V2 ss = args.position * s.scale + s.center;
      V2 size = args.size * s.scale;
      float alpha1 = args.alpha1 * s.alpha;
      float alpha2 = args.alpha2 * s.alpha;
      V4 color1 = V4(alpha1 * args.color1 * s.color, alpha1);
      V4 color2 = V4(alpha2 * args.color2 * s.color, alpha2);

      vertices[0] = GradientVertex(
        V3(ss + V2(0, 0), 0), color1, color2, V4(0, 0, size.x, size.y));
      vertices[1] = GradientVertex(
        V3(ss + V2(size.x, 0), 0), color1, color2, V4(1, 0, size.x, size.y));
      vertices[2] = GradientVertex(
        V3(ss + V2(size.x, size.y), 0), color1, color2, V4(1, 1, size.x, size.y));
      vertices[3] = GradientVertex(
        V3(ss + V2(0, size.y), 0), color1, color2, V4(0, 1, size.x, size.y));
    }

    Glyph Transform(V2 const& offset, V2 const& scale) {
      args.position = scale * args.position + offset;
      args.size *= scale;
      return this;
    }
  };
}

DefineFunction(Glyph_Gradient) {
  return new Gradient(args);
}

