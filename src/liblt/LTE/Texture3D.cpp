#include "Texture3D.h"

#include "GL.h"
#include "Program.h"

namespace {
  struct Texture3DImpl : public Texture3DT {
    GL_Texture texture;
    GL_TextureFormat::Enum format;
    uint width;
    uint height;
    uint depth;
    DERIVED_TYPE_EX(Texture3DImpl)

    Texture3DImpl() {}
    
    Texture3DImpl(
        uint width,
        uint height,
        uint depth,
        GL_TextureFormat::Enum format) :
      format(format),
      width(width),
      height(height),
      depth(depth)
    {
      texture = GL_GenTexture();
      GL_ActiveTexture(0);
      GL_BindTexture(GL_TextureTargetBindable::T3D, texture);

      /* Note that these are necessary to make the textures "complete,"
         since we won't be generating mip-maps. */
      GL_TexMagFilter(GL_TextureTarget::T3D, GL_TextureFilter::Linear);
      GL_TexMinFilter(GL_TextureTarget::T3D, GL_TextureFilterMip::Linear);
      GL_TexImage3D(
        GL_TextureTarget::T3D, 0, format,
        width, height, depth,
        GL_PixelFormat::Red, GL_DataFormat::Float, nullptr);
    }

    ~Texture3DImpl() {
      if (!Program_InStaticSection())
        GL_DeleteTexture(texture);
    }

    void AddressBorder(float r, float g, float b, float a) {
      float v[] = {r, g, b, a};
      GL_BindTexture(GL_TextureTargetBindable::T3D, texture);
      GL_TexWrapMode(
        GL_TextureTarget::T3D,
        GL_TextureCoordinate::S,
        GL_TextureWrapMode::ClampToBorder);
      GL_TexWrapMode(
        GL_TextureTarget::T3D,
        GL_TextureCoordinate::T,
        GL_TextureWrapMode::ClampToBorder);
      GL_TexWrapMode(
        GL_TextureTarget::T3D,
        GL_TextureCoordinate::R,
        GL_TextureWrapMode::ClampToBorder);
      glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, v);
      DEBUG_GL_ERRORS;
    }

    void AddressClamp() {
      GL_BindTexture(GL_TextureTargetBindable::T3D, texture);
      GL_TexWrapMode(
        GL_TextureTarget::T3D,
        GL_TextureCoordinate::S,
        GL_TextureWrapMode::ClampToEdge);
      GL_TexWrapMode(
        GL_TextureTarget::T3D,
        GL_TextureCoordinate::T,
        GL_TextureWrapMode::ClampToEdge);
      GL_TexWrapMode(
        GL_TextureTarget::T3D,
        GL_TextureCoordinate::R,
        GL_TextureWrapMode::ClampToEdge);
    }

    void AddressRepeat() {
      GL_BindTexture(GL_TextureTargetBindable::T3D, texture);
      GL_TexWrapMode(
        GL_TextureTarget::T3D,
        GL_TextureCoordinate::S,
        GL_TextureWrapMode::Repeat);
      GL_TexWrapMode(
        GL_TextureTarget::T3D,
        GL_TextureCoordinate::T,
        GL_TextureWrapMode::Repeat);
      GL_TexWrapMode(
        GL_TextureTarget::T3D,
        GL_TextureCoordinate::R,
        GL_TextureWrapMode::Repeat);
    }

    void Bind(uint textureUnit) const {
      GL_ActiveTexture(textureUnit);
      GL_BindTexture(GL_TextureTargetBindable::T3D, texture);
    }

    void GenerateMipmap() {
      GL_BindTexture(GL_TextureTargetBindable::T3D, texture);
      GL_GenerateMipmap(GL_TextureTarget::T3D);
    }

    uint GetWidth() const {
      return width;
    }

    uint GetHeight() const {
      return height;
    }

    uint GetDepth() const {
      return depth;
    }

    void GetData(uchar* buffer, GL_PixelFormat::Enum format, uint lod) const {
      GL_BindTexture(GL_TextureTargetBindable::T3D, texture);
      GL_GetTexImage(GL_TextureTarget::T3D, lod, format, GL_DataFormat::UnsignedByte,
                     buffer);
    }

    void GetData(float* buffer, GL_PixelFormat::Enum format, uint lod) const {
      GL_BindTexture(GL_TextureTargetBindable::T3D, texture);
      GL_GetTexImage(GL_TextureTarget::T3D, lod, format, GL_DataFormat::Float, buffer);
    }

    void SetData(
      uint x, uint y, uint z,
      uint width, uint height, uint depth,
      GL_PixelFormat::Enum pixelFormat,
      GL_DataFormat::Enum dataFormat,
      void const* buffer)
    {
      GL_BindTexture(GL_TextureTargetBindable::T3D, texture);
      GL_TexSubImage3D(GL_TextureTarget::T3D, 0, x, y, z, width, height, depth,
        pixelFormat, dataFormat, buffer);
    }

    void SetMagFilter(GL_TextureFilter::Enum filter) {
      GL_BindTexture(GL_TextureTargetBindable::T3D, texture);
      GL_TexMagFilter(GL_TextureTarget::T3D, filter);
    }

    void SetMinFilter(GL_TextureFilterMip::Enum filter) {
      GL_BindTexture(GL_TextureTargetBindable::T3D, texture);
      GL_TexMinFilter(GL_TextureTarget::T3D, filter);
    }
  };
}

Texture3D Texture3D_Create(
  uint width,
  uint height,
  uint depth,
  GL_TextureFormat::Enum format)
{
  return new Texture3DImpl(width, height, depth, format);
}
