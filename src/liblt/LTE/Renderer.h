#ifndef LTE_Renderer_h__
#define LTE_Renderer_h__

#include "GLEnum.h"
#include "GLType.h"
#include "String.h"
#include "V3.h"
#include "V4.h"
#include "Vector.h"

const size_t kAttribArrays = 8;

namespace LTE {
  namespace BlendMode {
    enum Enum {
      Additive,
      Alpha,
      Complementary,
      Disabled,
      Multiplicative
    };
  }

  namespace CullMode {
    enum Enum {
      Backface,
      Frontface,
      Disabled
    };
  }

  LT_API void Renderer_Initialize();

  /* Clears only the color buffers. */
  LT_API void Renderer_Clear(V4 const& clearColor = V4(0));

  /* Clears only the depth buffer. */
  LT_API void Renderer_ClearDepth(float depth = 1.0f);

  LT_API void Renderer_DrawFSQ();

  LT_API void Renderer_DrawFSQInParts(uint width, uint height, uint parts);

  LT_API void Renderer_DrawFSQPart(uint x, uint y, uint width, uint height);

  LT_API void Renderer_DrawMesh(MeshT const* mesh);

  LT_API void Renderer_DrawQuad(
    V2 const& p1 = V2(-1),
    V2 const& p2 = V2(1),
    V2 const& t1 = V2(0),
    V2 const& t2 = V2(1),
    float depth = 0);

  LT_API void Renderer_DrawQuadOutline(
    V2 const& p1,
    V2 const& p2);

  LT_API void Renderer_DrawVertices(
    Vector<Vertex> const& vertices,
    Vector<uint> const& indices);

  LT_API void Renderer_DrawVertices(
    Vector<Vertex> const& vertices,
    Vector<ushort> const& indices);

  LT_API void Renderer_DrawVertices(
    void const* vertexData,
    Type const& vertexFormat,
    void const* indexData,
    uint indices,
    GL_IndexFormat::Enum indexFormat);

  LT_API void Renderer_Flush();

  LT_API int Renderer_GetDrawCallCount();
  LT_API int Renderer_GetPolyCount();
  LT_API void Renderer_ResetCounters();

  /* Cachable State. */
  LT_API void Renderer_BindIndexBuffer(GL_Buffer buffer, bool force = false);
  LT_API void Renderer_BindVertexBuffer(GL_Buffer buffer, bool force = false);

  LT_API void Renderer_DisableAttribArray(uint index);
  LT_API void Renderer_EnableAttribArray(uint index);

  LT_API void Renderer_SetColor(Color const& color, float alpha = 1);

  LT_API void Renderer_SetShader(ShaderT& shader);
  LT_API void Renderer_SetShaderFF();

  LT_API void Renderer_SetViewport(V2 const& origin, V2 const& size);

  LT_API void Renderer_SetWireframe(bool wireframe);

  /* Pushable state. */
  LT_API void Renderer_PushBlendMode(BlendMode::Enum mode);
  LT_API void Renderer_PopBlendMode();

  LT_API void Renderer_PopAllBuffers();
  LT_API void Renderer_PushAllBuffers();

  LT_API void Renderer_PopColorBuffers();
  LT_API void Renderer_PushColorBuffers();

  LT_API void Renderer_PushColorBuffer(
    uint index,
    GL_Texture texture,
    uint guid,
    GL_TextureTarget::Enum target = GL_TextureTarget::T2D);

  LT_API void Renderer_PopColorBuffer(uint index);

  LT_API void Renderer_PushCullMode(CullMode::Enum mode);
  LT_API void Renderer_PopCullMode();

  LT_API void Renderer_PushDepthBuffer(GL_Texture texture);
  LT_API void Renderer_PopDepthBuffer();

  LT_API void Renderer_PushScissorOff();
  LT_API void Renderer_PushScissorOn(V2 const& origin, V2 const& size);
  LT_API void Renderer_PopScissor();

  LT_API void Renderer_PushViewport(int x, int y, int w, int h);
  LT_API void Renderer_PopViewport();

  LT_API void Renderer_PushZBuffer(bool useZBuffer);
  LT_API void Renderer_PopZBuffer();

  LT_API void Renderer_PushZWritable(bool zWritable);
  LT_API void Renderer_PopZWritable();

  /* Matrices. */
  LT_API void Renderer_ClearMatrices();

  LT_API void Renderer_SetWorldTransform(Transform const& world);

  LT_API void Renderer_SetProjMatrix(Matrix const& proj);

  LT_API void Renderer_SetViewTransform(Transform const& view);

  LT_API Matrix const& Renderer_GetWorldMatrix();
  LT_API Matrix const& Renderer_GetWorldITMatrix();
  LT_API Matrix const& Renderer_GetWorldViewProjMatrix();
  LT_API Matrix const& Renderer_GetViewMatrix();
  LT_API Matrix const& Renderer_GetProjMatrix();

  struct RendererBlendMode {
    RendererBlendMode(BlendMode::Enum mode) {
      Renderer_PushBlendMode(mode);
    }

    ~RendererBlendMode() {
      Renderer_PopBlendMode();
    }
  };

  struct RendererCullMode {
    RendererCullMode(CullMode::Enum mode) {
      Renderer_PushCullMode(mode);
    }

    ~RendererCullMode() {
      Renderer_PopCullMode();
    }
  };

  struct RendererZBuffer {
    RendererZBuffer(bool zBuffer) {
      Renderer_PushZBuffer(zBuffer);
    }

    ~RendererZBuffer() {
      Renderer_PopZBuffer();
    }
  };

  struct RendererZWritable {
    RendererZWritable(bool zWritable) {
      Renderer_PushZWritable(zWritable);
    }

    ~RendererZWritable() {
      Renderer_PopZWritable();
    }
  };

  struct RendererState {
    RendererState(
      BlendMode::Enum blendMode,
      CullMode::Enum cullMode,
      bool zBuffer,
      bool zWritable)
    {
      Renderer_PushBlendMode(blendMode);
      Renderer_PushCullMode(cullMode);
      Renderer_PushZBuffer(zBuffer);
      Renderer_PushZWritable(zWritable);
    }

    ~RendererState() {
      Renderer_PopBlendMode();
      Renderer_PopCullMode();
      Renderer_PopZBuffer();
      Renderer_PopZWritable();
    }
  };
}

#endif
