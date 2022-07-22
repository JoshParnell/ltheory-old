#ifndef LTE_Texture3D_h__
#define LTE_Texture3D_h__

#include "BaseType.h"
#include "GLType.h"
#include "GLEnum.h"
#include "Reference.h"

struct Texture3DT : public RefCounted {
  BASE_TYPE(Texture3DT)

  virtual void AddressBorder(float r, float g, float b, float a) = 0;
  virtual void AddressClamp() = 0;
  virtual void AddressRepeat() = 0;

  virtual void Bind(uint textureUnit) const = 0;

  virtual void GenerateMipmap() = 0;

  virtual uint GetWidth() const = 0;
  virtual uint GetHeight() const = 0;
  virtual uint GetDepth() const = 0;

  virtual void GetData(
    uchar* buffer,
    GL_PixelFormat::Enum format = GL_PixelFormat::RGBA,
    uint lod = 0) const = 0;

  virtual void GetData(
    float* buffer,
    GL_PixelFormat::Enum format = GL_PixelFormat::Red,
    uint lod = 0) const = 0;

  virtual void SetData(
    uint x, uint y, uint z,
    uint width, uint height, uint depth,
    GL_PixelFormat::Enum pixelFormat,
    GL_DataFormat::Enum dataFormat,
    void const* buffer) = 0;

  virtual void SetMagFilter(GL_TextureFilter::Enum) = 0;
  virtual void SetMinFilter(GL_TextureFilterMip::Enum) = 0;

  FIELDS {}
};

LT_API Texture3D Texture3D_Create(
  uint width,
  uint height,
  uint depth,
  GL_TextureFormat::Enum internalFormat);

#endif
