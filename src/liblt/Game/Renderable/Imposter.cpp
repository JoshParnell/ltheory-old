#include "../Renderables.h"

#include "Game/Graphics/RenderStyles.h"

#include "LTE/DrawState.h"
#include "LTE/GL.h"
#include "LTE/Math.h"
#include "LTE/Meshes.h"
#include "LTE/Model.h"
#include "LTE/RNG.h"
#include "LTE/Renderer.h"
#include "LTE/RenderStyle.h"
#include "LTE/ShaderInstance.h"
#include "LTE/StackFrame.h"
#include "LTE/Texture2D.h"
#include "LTE/View.h"
#include "LTE/Viewport.h"

const float kLodFactor = 1;

namespace {
  AutoClassDerived(Imposter, RenderableT,
    Renderable, source,
    Renderable, imposter)
    DERIVED_TYPE_EX(Imposter)

    Imposter() {}

    Bound3 GetBound() const {
      return source->GetBound();
    }

    Mesh GetCollisionMesh() const {
      return source->GetCollisionMesh();
    }

    size_t GetHash() const {
      return source->GetHash();
    }

    short GetVersion() const {
      return source->GetVersion();
    }

    void Render(DrawState* state) const {
      Bound3 const& bound = GetBound();
      Matrix const& world = Renderer_GetWorldMatrix();
      Matrix const& view = Renderer_GetViewMatrix();
      Bound3 worldBound = bound.GetTransformed(world);
      Bound3 viewBound = worldBound.GetTransformed(view);
      V3 viewExtent = viewBound.GetSideLengths();
      float camDistance = Length(Max(viewBound.GetCenter() - viewExtent, V3(0)));
      float lod = kLodFactor * camDistance / Length(viewExtent);
      (lod > 1 ? imposter : source)->Render(state);
    }

    V3 Sample() const {
      return source->Sample();
    }
  };

  Renderable Generate(Renderable_Imposter_Args const& args) {
    SFRAME("Generate Imposter");
    /* TODO : Valid depth buffer. */
    const uint kResolution = 512;

    GL_Texture depthBuffer = GL_GenTexture();
    GL_BindTexture(GL_TextureTargetBindable::T2D, depthBuffer);
    GL_TexImage2D(
      GL_TextureTarget::T2D, 0,
      GL_TextureFormat::DepthComponent32F,
      kResolution, 6 * kResolution,
      GL_PixelFormat::DepthComponent,
      GL_DataFormat::Float, nullptr);

    Texture2D albedo =
      Texture_Create(kResolution, 6 * kResolution, GL_TextureFormat::RGBA16F);
    Texture2D normal =
      Texture_Create(kResolution, 6 * kResolution, GL_TextureFormat::RGBA16F);
    normal->SetMagFilter(GL_TextureFilter::Nearest);
    normal->SetMinFilter(GL_TextureFilterMip::Nearest);

    Texture2D depth = Texture_Create(1, 1, GL_TextureFormat::R32F);
    float d = 1e6;
    depth->SetData(0, 0, 1, 1, &d);

    Bound3 bound = args.source->GetBound();
    V3 center = bound.GetCenter();
    V3 sides = bound.GetSideLengths();

    DrawState state;
    RenderStyle_Push(RenderStyle_Default(false));
    View view(Transform_Identity(), kPi4, 1, 0.05, 1e6f);
    Viewport vp = Viewport_Create(0, V2(kResolution, 6 * kResolution), 1, false);
    Viewport_Push(vp);

    state.Push();
    state.view = &view;
    state.depth = depth;
    args.source->Render(&state);

    Renderer_PushAllBuffers();
    Renderer_PushCullMode(CullMode::Backface);
    Renderer_PushDepthBuffer(depthBuffer);
    Renderer_PushScissorOff();
    Renderer_PushBlendMode(BlendMode::Disabled);
    Renderer_PushZBuffer(true);

    albedo->Bind(0);
    normal->Bind(1);

    Renderer_Clear(V4(0, 0, 0, 0));
    Renderer_ClearDepth();

    for (uint i = 0; i < 6; ++i) {
      V3 look, up;
      switch (i) {
        case CubeFace::PositiveX:
          look = V3( 1, 0, 0); up = V3(0, 1, 0); break;
        case CubeFace::NegativeX:
          look = V3(-1, 0, 0); up = V3(0, 1, 0); break;
        case CubeFace::PositiveY:
          look = V3(0,  1, 0); up = V3(0, 0, -1); break;
        case CubeFace::NegativeY:
          look = V3(0, -1, 0); up = V3(0, 0,  1); break;
        case CubeFace::PositiveZ:
          look = V3(0, 0,  1); up = V3(0, 1, 0); break;
        case CubeFace::NegativeZ:
          look = V3(0, 0, -1); up = V3(0, 1, 0); break;
      }

      Renderer_SetViewTransform(Transform_LookUp(0, look, up));
      Renderer_SetProjMatrix(
        MatrixD::Translation(V3(0, 2.0f * (((float)i + 0.5f) / 6.0f) - 1.0f, 0)) *
        MatrixD::Scale(V3(1, 1.0f / 6.0f, 1.0f)) *
        MatrixD::Orthographic(1, 1, 0, 1) *
        MatrixD::Translation(V3(0, 0, 1)));
      Renderer_SetWorldTransform(
        Transform_Scale(V3(1) / sides) *
        Transform_Translation(-center));

      args.source->Render(&state);
    }

    normal->Unbind();
    albedo->Unbind();
    albedo->GenerateMipmap();
    normal->GenerateMipmap();

    Renderer_PopZBuffer();
    Renderer_PopBlendMode();
    Renderer_PopScissor();
    Renderer_PopDepthBuffer();
    Renderer_PopCullMode();
    Renderer_PopAllBuffers();

    state.Pop();
    Viewport_Pop();
    RenderStyle_Pop();

    GL_DeleteTexture(depthBuffer);

#if 1
    Mesh mesh = Mesh_Create();
    for (uint i = 0; i < 6; ++i) {
      float minV = (float)i / 6.0f;
      float maxV = (float)(i + 1) / 6.0f;
      V3 look;
      V3 up;
      V3 right;
      switch (i) {
        case CubeFace::PositiveX:
          look = V3( 1, 0, 0); up = V3(0, 1, 0); break;
        case CubeFace::NegativeX:
          look = V3(-1, 0, 0); up = V3(0, 1, 0); break;
        case CubeFace::PositiveY:
          look = V3(0,  1, 0); up = V3(0, 0, -1); break;
        case CubeFace::NegativeY:
          look = V3(0, -1, 0); up = V3(0, 0,  1); break;
        case CubeFace::PositiveZ:
          look = V3(0, 0,  1); up = V3(0, 1, 0); break;
        case CubeFace::NegativeZ:
          look = V3(0, 0, -1); up = V3(0, 1, 0); break;
      }
      right = Cross(up, look);
      uint offset = mesh->GetVertices();
      mesh->AddVertex(-up - right, look, 0, minV);
      mesh->AddVertex(-up + right, look, 1, minV);
      mesh->AddVertex( up + right, look, 1, maxV);
      mesh->AddVertex( up - right, look, 0, maxV);
      mesh->AddQuad(offset + 0, offset + 1, offset + 2, offset + 3);
    }
    mesh->Scale(0.5f * args.source->GetBound().GetSideLengths());
#else
    Mesh mesh = Mesh_Billboard();
    for (uint i = 0; i < mesh->GetVertices(); ++i)
      mesh->vertices[i].n = sides;
    for (uint i = 0; i < 8; ++i)
      mesh->AddVertex(bound[i], 0, 0, 0);
#endif

    static Shader shader = Shader_Create("npm.jsl", "imposter1.jsl");
    ShaderInstance shaderInstance = ShaderInstance_Create(shader);
    (*shaderInstance)
      ("albedoMap", albedo)
      ("normalMap", normal);
    DrawState_Link(shaderInstance);
    
    Renderable imposter = Model_Create()->Add(mesh, shaderInstance, false);
    return new Imposter(args.source, imposter);
  }
}

DefineFunction(Renderable_Imposter) {
  return Generate(args);
}
