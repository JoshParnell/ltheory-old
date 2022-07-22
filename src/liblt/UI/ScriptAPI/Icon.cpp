#include "LTE/Function.h"

#include "UI/Icon.h"

TypeAlias(Reference<GlyphT>, Glyph);
TypeAlias(Reference<IconT>, Icon);

VoidFreeFunction(Icon_AddGlyph,
  "Add 'glyph' to 'icon'",
  Icon, icon,
  Glyph, glyph)
{
  icon->Add(glyph);
} FunctionAlias(Icon_AddGlyph, +=);

VoidFreeFunction(Icon_AddIcon,
  "Add a copy of 'source' to 'icon' with translation 'offset' and scale 'scale'",
  Icon, icon,
  Icon, source,
  V2, offset,
  V2, scale)
{
  icon->Add(source, offset, scale);
} FunctionAlias(Icon_AddIcon, +=);

VoidFreeFunction(Icon_Draw,
  "Draw 'icon' to the screen at 'center' with 'scale,' 'color,' and 'alpha'",
  Icon, icon,
  V2, center,
  V2, scale,
  Color, color,
  float, alpha)
{
  icon->Draw(GlyphState(center, scale, color, alpha));
} FunctionAlias(Icon_Draw, Draw);

VoidFreeFunction(Icon_Transform,
  "Scale 'icon' by 'scale,' then move by 'offset'",
  Icon, icon,
  V2, offset,
  V2, scale)
{
  icon->Transform(offset, scale);
} FunctionAlias(Icon_Transform, Transform);
