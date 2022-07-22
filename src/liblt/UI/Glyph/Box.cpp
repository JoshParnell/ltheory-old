#include "../Glyphs.h"

#include "LTE/Pool.h"
#include "LTE/Shader.h"
#include "LTE/V4.h"

namespace {
  AutoClass(BoxVertex,
    V3, p,
    V4, color,
    V4, uvsize)
    BoxVertex() {}
  };

  AutoClassDerived(Box, GlyphT, Glyph_Box_Args, args)
    DERIVED_TYPE_EX(Box)
    POOLED_TYPE

    Box() {}

    Glyph Clone() const {
      return Glyph_Box(args);
    }

    Shader GetShader() const {
      return Shader_Create("widget.jsl", "ui/box.jsl");
    }

    Type GetVertexFormat() const {
      return Type_Get<BoxVertex>();
    }

    void Submit(void* vvertices, GlyphState const& s) const {
      BoxVertex* vertices = (BoxVertex*)vvertices;

      V2 ss = args.position * s.scale + s.center;
      V2 size = args.size * s.scale + V2(48);
      float alpha = args.alpha * s.alpha;
      V4 color = V4(alpha * args.color * s.color, alpha);

      vertices[0] = BoxVertex(
        V3(ss + V2(-size.x, -size.y), 0), color, V4(0, 0, size.x, size.y));
      vertices[1] = BoxVertex(
        V3(ss + V2( size.x, -size.y), 0), color, V4(1, 0, size.x, size.y));
      vertices[2] = BoxVertex(
        V3(ss + V2( size.x,  size.y), 0), color, V4(1, 1, size.x, size.y));
      vertices[3] = BoxVertex(
        V3(ss + V2(-size.x,  size.y), 0), color, V4(0, 1, size.x, size.y));
    }

    Glyph Transform(V2 const& offset, V2 const& scale) {
      args.position = scale * args.position + offset;
      args.size *= scale;
      return this;
    }
  };
}

DefineFunction(Glyph_Box) {
  return new Box(args);
}
