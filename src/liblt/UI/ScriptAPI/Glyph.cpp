#include "UI/Glyph.h"

#include "LTE/Function.h"

VoidFreeFunction(Glyph_Draw,
  "Draw 'glyph' at 'center' with 'scale,' 'color,' and 'alpha'",
  Glyph, glyph,
  V2, center,
  V2, scale,
  Color, color,
  float, alpha)
{
  glyph->Draw(GlyphState(center, scale, color, alpha));
} FunctionAlias(Glyph_Draw, Draw);
