#ifndef UI_WidgetRenderer_h__
#define UI_WidgetRenderer_h__

#include "Common.h"
#include "LTE/DeclareFunction.h"
#include "LTE/V2.h"

DeclareFunction(WidgetRenderer_DrawGlyph, void,
  Glyph, glyph,
  GlyphState, state)

DeclareFunction(WidgetRenderer_DrawPanel, void,
  V2, pos,
  V2, size,
  Color, color,
  float, innerAlpha,
  float, alpha,
  float, bevel)

DeclareFunction(WidgetRenderer_DrawPanelRadial, void,
  V2, pos,
  float, r1,
  float, r2,
  float, phase,
  float, angle,
  Color, color,
  float, innerAlpha,
  float, alpha,
  float, bevel)

DeclareFunction(WidgetRenderer_DrawRenderable, void,
  Renderable, renderable,
  Transform, transform,
  V3, camPos,
  V3, camLook,
  V3, camUp,
  float, camFov,
  V2, pos,
  V2, size,
  Color, color,
  float, alpha,
  float, time)

DeclareFunction(WidgetRenderer_DrawTexture, void,
  Texture2D, texture,
  V2, pos,
  V2, size,
  float, alpha)

DeclareFunction(WidgetRenderer_DrawTextureAdditive, void,
  Texture2D, texture,
  V2, pos,
  V2, size,
  float, alpha)

DeclareFunction(WidgetRenderer_DrawText, void,
  Font, font,
  String, text,
  V2, pos,
  float, size,
  Color, color,
  float, alpha,
  bool, centered)

DeclareFunction(WidgetRenderer_DrawTextGlow, void,
  Font, font,
  String, text,
  V2, pos,
  float, size,
  Color, color,
  float, alpha,
  bool, centered)

DeclareFunctionNoParams(WidgetRenderer_Flush, void)

DeclareFunction(WidgetRenderer_GetTextSize, V2,
  Font, font,
  String, text,
  float, size)

#endif
