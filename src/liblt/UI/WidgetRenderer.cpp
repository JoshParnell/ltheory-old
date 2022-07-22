#include "WidgetRenderer.h"
#include "Glyph.h"

#include "LTE/AutoClass.h"
#include "LTE/DrawState.h"
#include "LTE/Font.h"
#include "LTE/Map.h"
#include "LTE/Math.h"
#include "LTE/Profiler.h"
#include "LTE/Renderer.h"
#include "LTE/Shader.h"
#include "LTE/ShaderInstance.h"
#include "LTE/StackFrame.h"
#include "LTE/Vector.h"
#include "LTE/Viewport.h"

#include "LTE/Debug.h"

const float kPanelShadowSize = 32;

namespace {
  AutoClass(GlyphInstance,
    Glyph, glyph,
    GlyphState, state)
    GlyphInstance() {}
  };

  AutoClass(PanelVertex,
    V3, p,
    V4, uvsize,
    V4, color,
    V2, innerAlphaBevel)
    PanelVertex() {}
  };

  AutoClass(RadialPanelVertex,
    V3, p,
    V4, uvr1r2,
    V4, color,
    V4, innerAlphaBevelPhaseAngle)
    RadialPanelVertex() {}
  };

  typedef Map<Type, Vector<GlyphInstance> > GlyphMapT;

  struct WidgetRenderer {
    GlyphMapT glyphs;

    Vector<PanelVertex> panelBuffer;
    Vector<RadialPanelVertex> radialPanelBuffer;
    Vector<uchar> vertexBuffer;
    Vector<uint> indexBuffer;

    Shader panelShader;
    Shader radialPanelShader;
    Shader textureShader;
    Shader textureShaderAdditive;

    Map<Type, Shader> shaderCache;
  } renderer;

  void BindShaderInputs(ShaderT* shader) {
    shader->BindInput(1, "vert_attrib1");
    shader->BindInput(2, "vert_attrib2");
    shader->BindInput(3, "vert_attrib3");
    shader->BindInput(4, "vert_attrib4");
    shader->Relink();
  }

  void WidgetRenderer_Initialize() {
    if (!renderer.panelShader) {
      renderer.panelShader = Shader_Create("widget.jsl", "ui/panel.jsl");
      renderer.radialPanelShader = Shader_Create("widget.jsl", "ui/radialpanel.jsl");
      renderer.textureShader = Shader_Create("widgetTexture.jsl", "ui/texture.jsl");
      renderer.textureShaderAdditive = Shader_Create("widgetTexture.jsl", "ui/textureadditive.jsl");
      BindShaderInputs(renderer.panelShader);
    }
  }

  void PopulateIndices(size_t count) {
    for (uint i = renderer.indexBuffer.size() / 6; i < count; ++i) {
      uint offset = i * 4;
      renderer.indexBuffer.push(offset + 0);
      renderer.indexBuffer.push(offset + 1);
      renderer.indexBuffer.push(offset + 2);
      renderer.indexBuffer.push(offset + 0);
      renderer.indexBuffer.push(offset + 2);
      renderer.indexBuffer.push(offset + 3);
    }
  }

  template <class T>
  void Render(Shader const& shader, Vector<T> const& vertexBuffer) {
    DrawState_Link(shader);
    Renderer_SetShader(*shader);
    Renderer_DrawVertices(
      vertexBuffer.data(),
      Type_Get<T>(),
      renderer.indexBuffer.data(),
      6 * (vertexBuffer.size() / 4),
      GL_IndexFormat::Int);
    Profiler_Flush();
  }

  void RenderGlyphs(
    Type const& glyphType,
    Vector<GlyphInstance> const& glyphs)
  {
    if (!glyphs.size())
      return;

    Shader& shader = renderer.shaderCache[glyphType];
    if (!shader) {
      shader = glyphs[0].glyph->GetShader();
      BindShaderInputs(shader);
    }

    PopulateIndices(glyphs.size());

    Type const& vertexFormat = glyphs[0].glyph->GetVertexFormat();
    renderer.vertexBuffer.clear();
    renderer.vertexBuffer.reserve(4 * glyphs.size() * vertexFormat->size);

    for (size_t i = 0; i < glyphs.size(); ++i) {
      uchar* pBuffer = renderer.vertexBuffer.data() + 4 * i * vertexFormat->size;
      glyphs[i].glyph->Submit((void*)pBuffer, glyphs[i].state);
    }

    DrawState_Link(shader);
    Renderer_SetShader(*shader);
    Renderer_DrawVertices(
      renderer.vertexBuffer.data(),
      vertexFormat,
      renderer.indexBuffer.data(),
      6 * glyphs.size(),
      GL_IndexFormat::Int);
    Profiler_Flush();
  }
}

DefineFunction(WidgetRenderer_DrawGlyph) {
  renderer.glyphs[args.glyph->GetDerivedTypeInfo()]
    .push(GlyphInstance(args.glyph, args.state));
} FunctionAlias(WidgetRenderer_DrawGlyph, DrawGlyph);

DefineFunction(WidgetRenderer_DrawPanel) {
  WidgetRenderer_Flush();
  SFRAME("Panel");
  RendererState rs(BlendMode::Alpha, CullMode::Disabled, false, false);
  renderer.panelBuffer.clear();
  PopulateIndices(1);

  V2 ss1 = args.pos;
  V2 ss2 = args.pos + args.size;
  V2 size = ss2 - ss1 + 2.0f * V2(kPanelShadowSize);
  V4 color = V4(args.color, args.alpha);

  renderer.panelBuffer.push(PanelVertex(
    V3(V2(ss1.x - kPanelShadowSize, ss1.y - kPanelShadowSize), 0),
    V4(0, 0, size.x, size.y),
    color,
    V2(args.innerAlpha, args.bevel)));

  renderer.panelBuffer.push(PanelVertex(
    V3(V2(ss2.x + kPanelShadowSize, ss1.y - kPanelShadowSize), 0),
    V4(1, 0, size.x, size.y),
    color,
    V2(args.innerAlpha, args.bevel)));

  renderer.panelBuffer.push(PanelVertex(
    V3(V2(ss2.x + kPanelShadowSize, ss2.y + kPanelShadowSize), 0),
    V4(1, 1, size.x, size.y),
    color,
    V2(args.innerAlpha, args.bevel)));

  renderer.panelBuffer.push(PanelVertex(
    V3(V2(ss1.x - kPanelShadowSize, ss2.y + kPanelShadowSize), 0),
    V4(0, 1, size.x, size.y),
    color,
    V2(args.innerAlpha, args.bevel)));

  (*renderer.panelShader)("shadowSize", kPanelShadowSize);
  Render(renderer.panelShader, renderer.panelBuffer);
} FunctionAlias(WidgetRenderer_DrawPanel, DrawPanel);

DefineFunction(WidgetRenderer_DrawPanelRadial) {
  WidgetRenderer_Flush();
  SFRAME("PanelRadial");
  RendererState rs(BlendMode::Alpha, CullMode::Disabled, false, false);
  renderer.radialPanelBuffer.clear();
  PopulateIndices(1);

  V2 ss1 = args.pos - V2(args.r2 + kPanelShadowSize);
  V2 ss2 = args.pos + V2(args.r2 + kPanelShadowSize);
  V4 color = V4(args.color, args.alpha);

  renderer.radialPanelBuffer.push(RadialPanelVertex(
    V3(V2(ss1.x - kPanelShadowSize, ss1.y - kPanelShadowSize), 0),
    V4(0, 0, args.r1, args.r2),
    color,
    V4(args.innerAlpha, args.bevel, args.phase, args.angle)));

  renderer.radialPanelBuffer.push(RadialPanelVertex(
    V3(V2(ss2.x + kPanelShadowSize, ss1.y - kPanelShadowSize), 0),
    V4(1, 0, args.r1, args.r2),
    color,
    V4(args.innerAlpha, args.bevel, args.phase, args.angle)));

  renderer.radialPanelBuffer.push(RadialPanelVertex(
    V3(V2(ss2.x + kPanelShadowSize, ss2.y + kPanelShadowSize), 0),
    V4(1, 1, args.r1, args.r2),
    color,
    V4(args.innerAlpha, args.bevel, args.phase, args.angle)));

  renderer.radialPanelBuffer.push(RadialPanelVertex(
    V3(V2(ss1.x - kPanelShadowSize, ss2.y + kPanelShadowSize), 0),
    V4(0, 1, args.r1, args.r2),
    color,
    V4(args.innerAlpha, args.bevel, args.phase, args.angle)));

  (*renderer.radialPanelShader)("shadowSize", kPanelShadowSize);
  Render(renderer.radialPanelShader, renderer.radialPanelBuffer);
} FunctionAlias(WidgetRenderer_DrawPanelRadial, DrawPanelRadial);

template <class T>
void WidgetRenderer_DrawTextGeneric(T const& args, bool additive) {
  V2 textSize = args.font->GetTextSize(args.text, args.size);
  V2 offset = V2(0, 0.25f * textSize.y);
  if (args.centered)
    offset.x = -0.5f * textSize.x;

  args.font->Draw(
    args.text,
    args.pos + offset,
    args.size,
    args.color,
    args.alpha,
    additive);
}

DefineFunction(WidgetRenderer_DrawText) {
  WidgetRenderer_Flush();
  RendererState rs(BlendMode::Alpha, CullMode::Disabled, false, false);
  WidgetRenderer_DrawTextGeneric(args, false);
  WidgetRenderer_Flush();
} FunctionAlias(WidgetRenderer_DrawText, DrawText);

DefineFunction(WidgetRenderer_DrawTextGlow) {
  WidgetRenderer_Flush();
  RendererState rs(BlendMode::Additive, CullMode::Disabled, false, false);
  WidgetRenderer_DrawTextGeneric(args, true);
  WidgetRenderer_Flush();
} FunctionAlias(WidgetRenderer_DrawTextGlow, DrawTextGlow);

DefineFunction(WidgetRenderer_DrawTexture) {
  WidgetRenderer_Flush();
  RendererState rs(BlendMode::Alpha, CullMode::Disabled, false, false);
  (*renderer.textureShader)
    ("texture", args.texture)
    ("alpha", args.alpha);
  Renderer_SetShader(*renderer.textureShader);
  Renderer_DrawQuad(args.pos, args.pos + args.size);
} FunctionAlias(WidgetRenderer_DrawTexture, Draw);

DefineFunction(WidgetRenderer_DrawTextureAdditive) {
  WidgetRenderer_Flush();
  RendererState rs(BlendMode::Additive, CullMode::Disabled, false, false);
  (*renderer.textureShaderAdditive)
    ("texture", args.texture)
    ("alpha", args.alpha);
  Renderer_SetShader(*renderer.textureShaderAdditive);
  Renderer_DrawQuad(args.pos, args.pos + args.size);
} FunctionAlias(WidgetRenderer_DrawTextureAdditive, DrawAdditive);

DefineFunction(WidgetRenderer_Flush) {
  WidgetRenderer_Initialize();
  RendererState rs(
    BlendMode::Additive,
    CullMode::Disabled,
    false,
    false);

  for (GlyphMapT::iterator it = renderer.glyphs.begin();
       it != renderer.glyphs.end(); ++it)
  {
    FRAME(it->first->name) {
      RenderGlyphs(it->first, it->second);
      it->second.clear();
    }
  }
} FunctionAlias(WidgetRenderer_Flush, Flush);

DefineFunction(WidgetRenderer_GetTextSize) {
  return args.font->GetTextSize(args.text, args.size);
} FunctionAlias(WidgetRenderer_GetTextSize, GetTextSize);
