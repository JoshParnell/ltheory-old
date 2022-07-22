#ifndef UI_Glyphs_h__
#define UI_Glyphs_h__

#include "Glyph.h"

#include "LTE/DeclareFunction.h"
#include "LTE/V3.h"

DeclareFunctionArgBind(Glyph_Arc, Glyph,
  V2, position,
  float, radius,
  float, radiusS,
  Color, color,
  float, alpha,
  float, angle,
  float, angleS)

DeclareFunctionArgBind(Glyph_Box, Glyph,
  V2, position,
  V2, size,
  Color, color,
  float, alpha)

DeclareFunctionArgBind(Glyph_Circle, Glyph,
  V2, position,
  float, radius,
  Color, color,
  float, alpha)

DeclareFunctionArgBind(Glyph_Line, Glyph,
  V2, p1,
  V2, p2,
  Color, color,
  float, alpha)

DeclareFunctionArgBind(Glyph_LineFade, Glyph,
  V2, p1,
  V2, p2,
  Color, color,
  float, alpha)

DeclareFunctionArgBind(Glyph_Gradient, Glyph,
  V2, position,
  V2, size,
  Color, color1,
  float, alpha1,
  Color, color2,
  float, alpha2)

DeclareFunctionArgBind(Glyph_Grid, Glyph,
  V2, p1,
  V2, p2,
  Color, color,
  float, alpha,
  V2, offset,
  V2, scale)

DeclareFunctionArgBind(Glyph_Rect, Glyph,
  V2, position,
  V2, size,
  Color, color,
  float, alpha,
  float, bevel,
  float, variance)

DeclareFunctionArgBind(Glyph_Ring, Glyph,
  V2, position,
  float, radius,
  Color, color,
  float, alpha,
  float, angle)

DeclareFunctionArgBind(Glyph_Triangle, Glyph,
  V2, p1,
  V2, p2,
  V2, p3,
  Color, color,
  float, alpha)

#endif
