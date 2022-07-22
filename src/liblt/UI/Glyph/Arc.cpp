#include "../Glyphs.h"

#include "LTE/Pool.h"
#include "LTE/Shader.h"
#include "LTE/V4.h"

const float kExpansion = 16;

namespace {
  AutoClass(ArcVertex,
    V3, p,
    V4, color,
    V4, uv_r_a,
    V2, rs_as)
    ArcVertex() {}
  };

  AutoClassDerived(Arc, GlyphT, Glyph_Arc_Args, args)
    DERIVED_TYPE_EX(Arc)
    POOLED_TYPE

    Arc() {}

    Glyph Clone() const {
      return Glyph_Arc(args);
    }

    Shader GetShader() const {
      return Shader_Create("widget.jsl", "ui/arc.jsl");
    }

    Type GetVertexFormat() const {
      return Type_Get<ArcVertex>();
    }

    void Submit(void* vvertices, GlyphState const& s) const {
      ArcVertex* vertices = (ArcVertex*)vvertices;

      V2 ss = args.position * s.scale + s.center;
      float r = 4.0f * args.radius * s.scale.GetMin() + kExpansion;
      float a = args.angle;
      float alpha = args.alpha * s.alpha;
      V4 color = V4(alpha * args.color * s.color, alpha);
      V2 rs_as = V2(s.scale.GetMin() * args.radiusS, args.angleS);

      vertices[0] = ArcVertex(V3(ss + V2(-r, -r), 0), color, V4(0, 0, r, a), rs_as);
      vertices[1] = ArcVertex(V3(ss + V2( r, -r), 0), color, V4(1, 0, r, a), rs_as);
      vertices[2] = ArcVertex(V3(ss + V2( r,  r), 0), color, V4(1, 1, r, a), rs_as);
      vertices[3] = ArcVertex(V3(ss + V2(-r,  r), 0), color, V4(0, 1, r, a), rs_as);
    }

    Glyph Transform(V2 const& offset, V2 const& scale) {
      args.position = scale * args.position + offset;
      args.radius *= scale.GetMin();
      args.radiusS *= scale.GetMin();
      return this;
    }
  };
}

DefineFunction(Glyph_Arc) {
  return new Arc(args);
}
