#include "Glyph.h"
#include "WidgetRenderer.h"

void GlyphT::Draw(GlyphState const& state) const {
  WidgetRenderer_DrawGlyph((GlyphT*)this, state);
}
