#include "../Glyphs.h"

#include "LTE/Pool.h"
#include "LTE/Shader.h"
#include "LTE/V4.h"

namespace {
  AutoClass(GridVertex,
    V3, p,
    V4, color,
    V4, uvSize,
    V4, offsetScale)
    GridVertex() {}
  };

  AutoClassDerived(Grid, GlyphT, Glyph_Grid_Args, args)
    DERIVED_TYPE_EX(Grid)
    POOLED_TYPE

    Grid() {}

    Glyph Clone() const {
      return Glyph_Grid(args);
    }

    Shader GetShader() const {
      return Shader_Create("widget.jsl", "ui/grid.jsl");
    }

    Type GetVertexFormat() const {
      return Type_Get<GridVertex>();
    }

    void Submit(void* vvertices, GlyphState const& s) const {
      GridVertex* vertices = (GridVertex*)vvertices;

      V2 ss1 = args.p1 * s.scale + s.center;
      V2 ss2 = args.p2 * s.scale + s.center;
      V2 size = (ss2 - ss1);
      float alpha = args.alpha * s.alpha;
      V4 color = V4(alpha * args.color * s.color, alpha);
      V4 os = V4(args.offset.x, args.offset.y, args.scale.x, args.scale.y);

      vertices[0] = GridVertex(V3(ss1 + size * V2(0, 0), 0), color, V4(0, 0, size.x, size.y), os);
      vertices[1] = GridVertex(V3(ss1 + size * V2(1, 0), 0), color, V4(1, 0, size.x, size.y), os);
      vertices[2] = GridVertex(V3(ss1 + size * V2(1, 1), 0), color, V4(1, 1, size.x, size.y), os);
      vertices[3] = GridVertex(V3(ss1 + size * V2(0, 1), 0), color, V4(0, 1, size.x, size.y), os);
    }

    Glyph Transform(V2 const& offset, V2 const& scale) {
      args.p1 = scale * args.p1 + offset;
      args.p2 = scale * args.p2 + offset;
      return this;
    }
  };
}

DefineFunction(Glyph_Grid) {
  return new Grid(args);
}
