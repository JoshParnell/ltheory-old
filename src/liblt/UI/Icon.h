#ifndef UI_Icon_h__
#define UI_Icon_h__

#include "Common.h"
#include "Glyph.h"
#include "LTE/AutoClass.h"
#include "LTE/DeclareFunction.h"
#include "LTE/Vector.h"

AutoClassDerived(IconT, RefCounted,
  Vector<Glyph>, glyphs)
  
  IconT() {}

  Icon Add(Glyph const& glyph) {
    glyphs.push(glyph);
    return this;
  }

  Icon Add(Icon const& icon, V2 const& offset, V2 const& scale) {
    for (size_t i = 0; i < icon->glyphs.size(); ++i)
      glyphs.push(icon->glyphs[i]->Clone()->Transform(offset, scale));
    return this;
  }

  void Draw(GlyphState const& state) const {
    for (size_t i = 0; i < glyphs.size(); ++i)
      glyphs[i]->Draw(state);
  }

  void Transform(V2 const& offset, V2 const& scale) {
    for (size_t i = 0; i < glyphs.size(); ++i)
      glyphs[i]->Transform(offset, scale);
  }
};

DeclareFunctionNoParams(Icon_Create, Icon)

#endif
