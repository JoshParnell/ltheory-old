#include "../Glyphs.h"

#include "LTE/Pool.h"
#include "LTE/Shader.h"
#include "LTE/V4.h"

const float kExpansion = 32;

namespace {
  AutoClass(LineVertex,
    V3, p,
    V4, color,
    V2, uv,
    V4, p1dir,
    V4, originsize)
    LineVertex() {}
  };

  template <class T>
  void SubmitVertices(T const& args, void* vvertices, GlyphState const& s) {
    LineVertex* vertices = (LineVertex*)vvertices;

    V2 ss1 = args.p1 * s.scale + s.center;
    V2 ss2 = args.p2 * s.scale + s.center;
    V2 minP = Min(ss1, ss2) - V2(kExpansion);
    V2 maxP = Max(ss1, ss2) + V2(kExpansion);
    V2 size = maxP - minP;
    V2 dir = ss2 - ss1;
    V4 p1dir = V4(ss1.x, ss1.y, dir.x, dir.y);
    V4 originsize = V4(minP.x, minP.y, size.x, size.y);
    float alpha = args.alpha * s.alpha;
    V4 color = V4(alpha * args.color * s.color, alpha);

    vertices[0] = LineVertex(V3(V2(minP.x, minP.y), 0), color, V2(0, 0), p1dir, originsize);
    vertices[1] = LineVertex(V3(V2(maxP.x, minP.y), 0), color, V2(1, 0), p1dir, originsize);
    vertices[2] = LineVertex(V3(V2(maxP.x, maxP.y), 0), color, V2(1, 1), p1dir, originsize);
    vertices[3] = LineVertex(V3(V2(minP.x, maxP.y), 0), color, V2(0, 1), p1dir, originsize);
  }

  AutoClassDerived(Line, GlyphT, Glyph_Line_Args, args)
    DERIVED_TYPE_EX(Line)
    POOLED_TYPE

    Line() {}

    Glyph Clone() const {
      return Glyph_Line(args);
    }

    Shader GetShader() const {
      return Shader_Create("widget.jsl", "ui/line.jsl");
    }

    Type GetVertexFormat() const {
      return Type_Get<LineVertex>();
    }

    void Submit(void* vvertices, GlyphState const& state) const {
      SubmitVertices(args, vvertices, state);
    }

    Glyph Transform(V2 const& offset, V2 const& scale) {
      args.p1 = scale * args.p1 + offset;
      args.p2 = scale * args.p2 + offset;
      return this;
    }
  };

  AutoClassDerived(LineFade, GlyphT, Glyph_LineFade_Args, args)
    DERIVED_TYPE_EX(LineFade)
    POOLED_TYPE

    LineFade() {}

    Glyph Clone() const {
      return Glyph_LineFade(args);
    }

    Shader GetShader() const {
      return Shader_Create("widget.jsl", "ui/linefade.jsl");
    }

    Type GetVertexFormat() const {
      return Type_Get<LineVertex>();
    }

    void Submit(void* vvertices, GlyphState const& state) const {
      SubmitVertices(args, vvertices, state);
    }

    Glyph Transform(V2 const& offset, V2 const& scale) {
      args.p1 = scale * args.p1 + offset;
      args.p2 = scale * args.p2 + offset;
      return this;
    }
  };
}

DefineFunction(Glyph_Line) {
  return new Line(args);
}

DefineFunction(Glyph_LineFade) {
  return new LineFade(args);
}
