#include "CubeMap.h"

#include "Array.h"
#include "AutoPtr.h"
#include "GL.h"
#include "Location.h"
#include "Math.h"
#include "Matrix.h"
#include "Renderer.h"
#include "Shader.h"
#include "StackFrame.h"
#include "Texture2D.h"
#include "Timer.h"
#include "Transform.h"
#include "V3.h"

#include "SFML/Graphics.hpp"

TypeAlias(Reference<CubeMapT>, CubeMap);

namespace {
  GL_TextureTarget::Enum CubeFaceToTarget(CubeFace::Enum face) {
    switch (face) {
      case CubeFace::PositiveX :
        return GL_TextureTarget::CubeMapPositiveX;
      case CubeFace::NegativeX :
        return GL_TextureTarget::CubeMapNegativeX;
      case CubeFace::PositiveY : 
        return GL_TextureTarget::CubeMapPositiveY;
      case CubeFace::NegativeY :
        return GL_TextureTarget::CubeMapNegativeY;
      case CubeFace::PositiveZ :
        return GL_TextureTarget::CubeMapPositiveZ;
      case CubeFace::NegativeZ :
        return GL_TextureTarget::CubeMapNegativeZ;
      default :
        return GL_TextureTarget::CubeMapPositiveX;
    }
  }

  inline uint GetLevelResolution(uint resolution, uint level) {
    for (uint i = 0; i < level; ++i)
      resolution /= 2;
    return resolution;
  }

  struct CubeMapImpl : public CubeMapT {
    typedef CubeMapT BaseType;
    DERIVED_TYPE_EX(CubeMapImpl)

    GL_Texture texture;
    uint resolution;
    uint guid;
    GL_TextureFormat::Enum format;

    CubeMapImpl() :
      texture(GL_NullTexture),
      resolution(0),
      format(GL_TextureFormat::RGBA8)
    {
      static uint nextGUID = 0;
      this->guid = nextGUID++;
    }

    ~CubeMapImpl() {
      if (texture != GL_NullTexture)
        GL_DeleteTexture(texture);
    }

    void BeginRender() {
      Renderer_PushBlendMode(BlendMode::Disabled);
      Renderer_PushCullMode(CullMode::Backface);
      Renderer_PushAllBuffers();
      Renderer_PushViewport(0, 0, resolution, resolution);
      Renderer_PushScissorOff();
      Renderer_PushZBuffer(false);
    }

    void Bind() const {
      GL_BindTexture(GL_TextureTargetBindable::CubeMap, texture);
    }

    void Create(uint res, GL_TextureFormat::Enum format) {
      this->texture = GL_GenTexture();
      this->resolution = res;
      this->format = format;
      
      Bind();
      GL_TexMagFilter(GL_TextureTarget::CubeMap, GL_TextureFilter::Linear);
      GL_TexMinFilter(GL_TextureTarget::CubeMap, GL_TextureFilterMip::LinearMipLinear);

      GL_TexWrapMode(
        GL_TextureTarget::CubeMap,
        GL_TextureCoordinate::S,
        GL_TextureWrapMode::ClampToEdge);
      GL_TexWrapMode(
        GL_TextureTarget::CubeMap,
        GL_TextureCoordinate::T,
        GL_TextureWrapMode::ClampToEdge);
      GL_TexWrapMode(
        GL_TextureTarget::CubeMap,
        GL_TextureCoordinate::R,
        GL_TextureWrapMode::ClampToEdge);
      
      GL_TexImage2D(GL_TextureTarget::CubeMapPositiveX, 0, format, res, res,
        GL_PixelFormat::RGBA, GL_DataFormat::UnsignedByte, nullptr);
      GL_TexImage2D(GL_TextureTarget::CubeMapPositiveY, 0, format, res, res,
        GL_PixelFormat::RGBA, GL_DataFormat::UnsignedByte, nullptr);
      GL_TexImage2D(GL_TextureTarget::CubeMapPositiveZ, 0, format, res, res,
        GL_PixelFormat::RGBA, GL_DataFormat::UnsignedByte, nullptr);
      GL_TexImage2D(GL_TextureTarget::CubeMapNegativeX, 0, format, res, res,
        GL_PixelFormat::RGBA, GL_DataFormat::UnsignedByte, nullptr);
      GL_TexImage2D(GL_TextureTarget::CubeMapNegativeY, 0, format, res, res,
        GL_PixelFormat::RGBA, GL_DataFormat::UnsignedByte, nullptr);
      GL_TexImage2D(GL_TextureTarget::CubeMapNegativeZ, 0, format, res, res,
        GL_PixelFormat::RGBA, GL_DataFormat::UnsignedByte, nullptr);
    }

    void EndRender() {
      Renderer_PopZBuffer();
      Renderer_PopScissor();
      Renderer_PopViewport();
      Renderer_PopAllBuffers();
      Renderer_PopCullMode();
      Renderer_PopBlendMode();
    }

    void GenerateFromShader(
      Shader const& shader,
      bool generateMips,
      float maxJobTime)
    {
      SFRAME("Generate CubeMap");
      Renderer_SetShader(*shader);
      BeginRender();
      (*shader)
        ("halfTexel", 0.5f / (float)resolution)
        ("texelScale", (float)resolution / (float)(resolution - 1));

      for (uint i = 0; i < CubeFace::SIZE; ++i) {
        SetFace((CubeFace::Enum)i);

        Matrix invProjView =
          (Renderer_GetProjMatrix() * Renderer_GetViewMatrix()).Inverse();
        V3 upperL = invProjView.TransformV3Norm(V3(-1.0f, 1.0f, 0.5f), 1);
        V3 upperR = invProjView.TransformV3Norm(V3( 1.0f, 1.0f, 0.5f), 1);
        V3 lowerL = invProjView.TransformV3Norm(V3(-1.0f,-1.0f, 0.5f), 1);
        (*shader)
          ("origin", upperL)
          ("du", upperR - upperL)
          ("dv", lowerL - upperL);

        /* Variable job-size algorithm for making sure to achieve optimal GPU
           utilization without causing a timeout. In most cases, 1 second should
           be allowable (Windows default timeout is 2s I believe) */
        uint jobSize = 1;
        uint x = 0;

        Timer timer;
        while (x < resolution) {
          timer.Reset();
          Renderer_PushScissorOn(
            V2((float)x, 0),
            V2((float)jobSize, (float)resolution));
          Renderer_DrawQuad();
          Renderer_PopScissor();

          GL_Finish();
          x += jobSize;

          /* NOTE : This is a bit scary...if the first job terminates really
                    quickly, it seems like we may overestimate our capacity. We
                    should probably implement something to make sure the jobSize
                    scales up slowly rather than all at once. */
          float elapsed = timer.GetElapsed();
          jobSize = Min(Max((uint)((float)jobSize * (maxJobTime / elapsed)), 1U), resolution);
        }

        Renderer_PopColorBuffer(0);
      }

      EndRender();
      if (generateMips)
        GenerateMipmap();
    }

    void GenerateMipmap() {
      Bind();
      GL_GenerateMipmap(GL_TextureTarget::CubeMap);
    }

    void GetData(
      CubeFace::Enum face,
      uint level,
      void* buffer) const
    {
      Bind();
      GL_GetTexImage(
        CubeFaceToTarget(face), level,
        GL_TextureFormat::PixelFormat(format),
        GL_TextureFormat::DataFormat(format), buffer);
    }

    GL_TextureFormat::Enum GetFormat() const {
      return format;
    }

    uint GetResolution() const {
      return resolution;
    }

    void SaveTo(String const& path, uint level) const {
      uint res = GetLevelResolution(resolution, level);
      Array<uchar> imageData(4 * res * res);
      Bind();

      for (uint i = 0; i < CubeFace::SIZE; ++i) {
        GL_GetTexImage(
          CubeFaceToTarget((CubeFace::Enum)i), level,
          GL_PixelFormat::RGBA,
          GL_DataFormat::UnsignedByte,
          imageData.data());

        sf::Image image;
        image.create(res, res, (sf::Uint8*)imageData.data());
        image.saveToFile(Stringize() | path | "_" | i | ".png");
      }
    }

    void SetData(
      CubeFace::Enum face,
      uint level,
      void const* buffer) const
    {
      uint res = GetLevelResolution(resolution, level);
      Bind();
      GL_TexImage2D(
        CubeFaceToTarget(face), level, format, res, res,
        GL_TextureFormat::PixelFormat(format),
        GL_TextureFormat::DataFormat(format), buffer);
    }

    void SetFace(CubeFace::Enum face) {
      /* Bind the appropriate face to the framebuffer color attachment. */
      GL_TextureTarget::Enum target = CubeFaceToTarget(face);
      Renderer_PushColorBuffer(0, texture, guid, target);

      /* Set the view and projection matrices accordingly for this face. */
      V3 look = 0;
      V3 up = 0;
      if (face == CubeFace::PositiveX) {
        look = V3(1, 0, 0);
        up = V3(0, 1, 0);
      } else if (face == CubeFace::NegativeX) {
        look = V3(-1, 0, 0);
        up = V3(0, 1, 0);
      } else if (face == CubeFace::PositiveY) {
        look = V3(0, 1, 0);
        up = V3(0, 0, -1);
      } else if (face == CubeFace::NegativeY) {
        look = V3(0, -1, 0);
        up = V3(0, 0, 1);
      } else if (face == CubeFace::PositiveZ) {
        look = V3(0, 0, 1);
        up = V3(0, 1, 0);
      } else if (face == CubeFace::NegativeZ) {
        look = V3(0, 0, -1);
        up = V3(0, 1, 0);
      }

      /* NOTE : Need to have a way to change the origin. */
      /* TODO : Better yet, re-frame this in terms of a camera. Give the
                cubemap a camera from which to render. */
      V3 origin = 0;
      Renderer_SetViewTransform(Transform_LookUp(origin, look, up));
      Renderer_SetProjMatrix(Matrix::Perspective(kPi2, 1.0f, 0.1f, 100000.0f));
    }

    FIELDS {
      CubeMapImpl* self = (CubeMapImpl*)addr;

      m(&self->format, "format", Type_Get(self->format), aux);
      m(&self->resolution, "resolution", Type_Get(self->resolution), aux);

      size_t totalSize = GL_TextureFormat::Size(self->format) *
        self->resolution * self->resolution;

      Array<uchar> buf(totalSize);

      if (self->texture != GL_NullTexture) {
        self->Bind();
        for (uint i = 0; i < CubeFace::SIZE; ++i) {
          GL_TextureTarget::Enum target =
            (GL_TextureTarget::Enum)(GL_TextureTarget::CubeMapPositiveX + i);

          GL_GetTexImage(
            target, 0,
            GL_TextureFormat::PixelFormat(self->format),
            GL_TextureFormat::DataFormat(self->format), buf.data());

          m(&buf, "data", Type_Get(buf), aux);
        }
      }

      else {
        self->Create(self->resolution, self->format);
        self->Bind();

        for (uint i = 0; i < CubeFace::SIZE; ++i) {
          m(&buf, "data", Type_Get(buf), aux);

          GL_TextureTarget::Enum target =
            (GL_TextureTarget::Enum)(GL_TextureTarget::CubeMapPositiveX + i);

          GL_TexImage2D(
            target,
            0,
            self->format,
            self->resolution,
            self->resolution,
            GL_TextureFormat::PixelFormat(self->format),
            GL_TextureFormat::DataFormat(self->format),
            buf.data());
        }

        GL_GenerateMipmap(GL_TextureTarget::CubeMap);
      }
    }

    DefineMetadataInline(CubeMapImpl)
  };

  DERIVED_IMPLEMENT(CubeMapImpl)
}

CubeMap CubeMap_Create(uint resolution, GL_TextureFormat::Enum format) {
  Reference<CubeMapImpl> self = new CubeMapImpl;
  self->Create(resolution, format);
  return self;
}
