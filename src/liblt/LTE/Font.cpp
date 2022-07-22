#include "Font.h"

#include "Array.h"
#include "Color.h"
#include "DrawState.h"
#include "HashMap.h"
#include "Map.h"
#include "ProgramLog.h"
#include "Renderer.h"
#include "Shader.h"
#include "StackFrame.h"
#include "Texture2D.h"
#include "UniString.h"
#include "Viewport.h"

#include "Debug.h"

#include "FreeType/ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H

const V2U kTextureSize = 1024;
const V2U kSDFSize = 1024;
const int kFontSize = 64;
const int kPadding = 8;

TypeAlias(Reference<FontT>, Font);

namespace {
  typedef Map<String, Font> FontCache;
  typedef HashMap<uint32, float> KerningMap;

  FontCache& GetFontCache() {
    static FontCache cache;
    return cache;
  }

  struct TextGlyph {
    uint32 codepoint;
    V2 uvMin;
    V2 uvMax;
    V2 origin;
    V2 size;
    float advance;
    bool exists;
    mutable KerningMap kerning;
  };

  AutoClass(TextVertex,
    V3, p,
    V4, color,
    V3, uvScale)
    TextVertex() {}
  };

  struct FontImpl : public FontT {
    mutable Map<uint32, TextGlyph> glyphs;
    mutable V2U cursor;
    Texture2D texture;
    Texture2D glyphBitmap;
    FT_Face face;
    FT_Library ft;

    FontImpl() :
      cursor(0) {}

    ~FontImpl() {
      /* Clean up FT. */ {
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
      }
    }

    void AddGlyph(uint32 codepoint) const {
      static Shader compute = Shader_Create("identity.jsl", "compute/sdffont.jsl");

      uint32 glyphIndex = FT_Get_Char_Index(face, codepoint);
      if (glyphIndex == 0) {
        TextGlyph& glyph = glyphs[codepoint];
        glyph.codepoint = codepoint;
        glyph.exists = false;
        return;
      }

      if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT) != 0)
        error("FreeType -- Failed to load glyph");

      if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))
        error("FreeType -- Failed to render glyph");

      /* Copy FT bitmap to our buffer. */
      FT_Bitmap const& bitmap = face->glyph->bitmap;
      uint sx = bitmap.width;
      uint sy = bitmap.rows;

      Array<float> buffer(sx * sy);
      uchar const* pixels = bitmap.buffer;
      for (uint y = 0; y < sy; ++y) {
        for (uint x = 0; x < sx; ++x) {
          // buffer[x + y * sx] = (pixels[x / 8] & (1 << (7 - x % 8))) ? 1 : 0;
          buffer[x + y * sx] = (float)pixels[x] / 255.0f;
        }
        pixels += bitmap.pitch;
      }

      uint px = cursor.x + kPadding + (kFontSize - sx) / 2;
      uint py = cursor.y + kPadding + (kFontSize - sy) / 2;

      if (cursor.x + (2 * kPadding + kFontSize) >= kTextureSize.x) {
        cursor.x = 0;
        cursor.y += 2 * kPadding + kFontSize;
        px = cursor.x + kPadding + (kFontSize - sx) / 2;
        py = cursor.y + kPadding + (kFontSize - sy) / 2;
      }

      /* Write glyph to bitmap. */
      glyphBitmap->SetData(
        px, py, sx, sy,
        GL_PixelFormat::Red,
        GL_DataFormat::Float,
        buffer.data());

      TextGlyph& glyph = glyphs[codepoint];
      glyph.codepoint = codepoint;
      glyph.uvMin = (V2)cursor / (V2)kTextureSize;
      glyph.uvMax = ((V2)cursor + V2(2 * kPadding + kFontSize)) / (V2)kTextureSize;
      glyph.origin = V2(px, py) - V2(cursor)
        + V2(-face->glyph->bitmap_left, face->glyph->bitmap_top);
      glyph.advance = (float)(face->glyph->advance.x >> 6);
      glyph.size = V2(2 * kPadding + kFontSize);
      glyph.exists = true;
      
      /* Compute the distance field. */ {
        // glyphBitmap->GenerateMipmap();
        Renderer_PushAllBuffers();
        texture->Bind(0);
        (*compute)
          ("bitmap", glyphBitmap)
          ("frame", (V2)kTextureSize)
          ("radius", (float)(kFontSize));
        Renderer_SetShader(*compute);
        RendererState rs(BlendMode::Disabled, CullMode::Backface, false, false);
        Renderer_PushScissorOn(cursor, glyph.size);
        Renderer_DrawQuad();
        Renderer_PopScissor();
        texture->Unbind();
        texture->GenerateMipmap();
        Renderer_PopAllBuffers();
      }

      /* Advance the cursor. */
      cursor.x += 2 * kPadding + kFontSize;

      // texture->SaveTo("font.bmp");
    }

    void Create(String const& path) {
      /* Create the FT library. */ {
        if (FT_Init_FreeType(&ft)) {
          Log_Error("FreeType -- Failed to initialize");
          return;
        }
      }

      /* Create the FT face. */ {
        String realPath = "../lt/resource/font/" + path;
        if (FT_New_Face(ft, realPath.c_str(), 0, &face)) {
          Log_Error("FreeType -- Failed to load font " + realPath);
          return;
        }
      }

      FT_Set_Pixel_Sizes(face, kFontSize, kFontSize);

      /* Create and clear the maps. */ {
        glyphBitmap = Texture_Create(
          kTextureSize.x,
          kTextureSize.y,
          GL_TextureFormat::R16F);

        glyphBitmap->Bind(0);
        Renderer_Clear(V4(0));
        glyphBitmap->Unbind();
        glyphBitmap->SetWrapMode(GL_TextureWrapMode::ClampToEdge);

        texture = Texture_Create(
          kSDFSize.x,
          kSDFSize.y,
          GL_TextureFormat::R32F);
      }

#if 0
      /* Render all chars to the bitmap. */ {
        uint32 glyphIndex;
        uint32 c = FT_Get_First_Char(face, &glyphIndex);

        while (glyphIndex != 0) {
          AddGlyph(c);
          c = FT_Get_Next_Char(face, c, &glyphIndex);
        }
      }
#endif

    }

    void Draw(
      String const& text,
      V2 const& position,
      float size,
      Color const& color,
      float alpha,
      bool additive) const
    {
      AUTO_FRAME;
      size /= kFontSize;

      static Vector<TextVertex> vertices;
      static Vector<uint> indices;
      vertices.clear();
      indices.clear();

      V2 p = position;
      V4 c = V4((V3)color, alpha);

      TextGlyph const* prev = nullptr;
      for (UniStringIterator it = UniString_Begin(text); it.HasMore(); it.Advance()) {
        uint32 codepoint = it.Get();
        TextGlyph const* glyph = glyphs.get(codepoint);
        if (!glyph) {
          AddGlyph(codepoint);
          glyph = glyphs.get(codepoint);
        }

        if (!glyph->exists) {
          prev = nullptr;
          continue;
        }

        /* Indices. */ {
          uint offset = vertices.size();
          indices.push(offset + 0);
          indices.push(offset + 1);
          indices.push(offset + 2);
          indices.push(offset + 0);
          indices.push(offset + 2);
          indices.push(offset + 3);
        }

        if (prev)
          p.x += size * GetKerning(prev, glyph);
        prev = glyph;

        /* Vertices. */ {
          V2 p0 = p  - size * glyph->origin;
          V2 p1 = p0 + size * V2(glyph->size.x, 0);
          V2 p2 = p0 + size * glyph->size;
          V2 p3 = p0 + size * V2(0, glyph->size.y);
          vertices.push(TextVertex(V3(p0, 0), c, V3(glyph->uvMin.x, glyph->uvMin.y, size)));
          vertices.push(TextVertex(V3(p1, 0), c, V3(glyph->uvMax.x, glyph->uvMin.y, size)));
          vertices.push(TextVertex(V3(p2, 0), c, V3(glyph->uvMax.x, glyph->uvMax.y, size)));
          vertices.push(TextVertex(V3(p3, 0), c, V3(glyph->uvMin.x, glyph->uvMax.y, size)));
        }

        p.x += size * glyph->advance;
      }

      /* Render. */ {
        static Shader shader = Shader_Create("widget.jsl", "ui/text.jsl");
        shader->BindInput(1, "vert_attrib1");
        shader->BindInput(2, "vert_attrib2");
        shader->BindInput(3, "vert_attrib3");
        shader->BindInput(4, "vert_attrib4");

        DrawState_Link(shader);
        (*shader)
          ("additive", additive ? 1 : 0)
          ("font", texture)
          ("frame", V2(kSDFSize));

        Renderer_SetShader(*shader);
        Renderer_DrawVertices(
          vertices.data(),
          Type_Get<TextVertex>(),
          indices.data(),
          indices.size(),
          GL_IndexFormat::Int);
      }
    }

    /* Kerning caching. */
    float GetKerning(TextGlyph const* prev, TextGlyph const* curr) const {
      float* k = prev->kerning.get(curr->codepoint);
      if (k)
        return *k;

      FT_Vector kern;
      FT_Get_Kerning(
        face,
        FT_Get_Char_Index(face, prev->codepoint),
        FT_Get_Char_Index(face, curr->codepoint),
        FT_KERNING_DEFAULT,
        &kern);

      float kerning = kern.x >> 6;
      prev->kerning[curr->codepoint] = kerning;
      return kerning;
    }

    V2 GetTextSize(String const& text, float size) const {
      float sx = 0;
      float sy = size;
      size /= kFontSize;

      TextGlyph const* prev = nullptr;
      for (UniStringIterator it = UniString_Begin(text); it.HasMore(); it.Advance()) {
        uint32 codepoint = it.Get();
        TextGlyph const* glyph = glyphs.get(codepoint);
        if (!glyph) {
          AddGlyph(codepoint);
          glyph = glyphs.get(codepoint);
        }

        if (!glyph->exists) {
          prev = nullptr;
          continue;
        }

        if (prev)
          sx += size * GetKerning(prev, glyph);
        prev = glyph;
        sx += size * glyph->advance;
      }

      return V2(sx, sy);
    }
  };
}

DefineFunction(Font_Get) {
  Font& font = GetFontCache()[args.path];
  if (!font) {
    Reference<FontImpl> self = new FontImpl;
    self->Create(args.path);
    font = self.t;
  }
  return font;
}
