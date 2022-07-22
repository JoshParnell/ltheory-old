#ifndef LTE_Texture_h__
#define LTE_Texture_h__

#include "BaseType.h"
#include "DeclareFunction.h"
#include "GLEnum.h"
#include "GLType.h"
#include "Reference.h"

struct Texture2DT : public RefCounted {
  BASE_TYPE(Texture2DT)

  virtual void Bind(uint bufferIndex = 0) = 0;
  virtual void Unbind() = 0;

  virtual void BindInput(uint unitIndex) const = 0;

  virtual void GenerateMipmap() = 0;

  virtual void GetData(void* buffer) const = 0;

  virtual GL_TextureFormat::Enum GetFormat() const = 0;

  virtual GL_Texture GetGLData() const = 0;

  virtual uint GetHeight() const = 0;

  virtual size_t GetMemory() const = 0;

  virtual uint GetWidth() const = 0;

  virtual void SaveTo(String const& path, bool flip = false) = 0;

  virtual void SetData(
    uint x,
    uint y,
    uint width,
    uint height,
    void const* buffer) = 0;

  virtual void SetData(
    uint x,
    uint y,
    uint width,
    uint height,
    GL_PixelFormat::Enum pixelFormat,
    GL_DataFormat::Enum dataFormat,
    void const* buffer) = 0;

  virtual void SetLodBias(float bias) = 0;
  virtual void SetMagFilter(GL_TextureFilter::Enum) = 0;
  virtual void SetMaxLod(int) = 0;
  virtual void SetMinFilter(GL_TextureFilterMip::Enum) = 0;
  virtual void SetMinLod(int) = 0;
  virtual void SetWrapMode(GL_TextureWrapMode::Enum) = 0;

  FIELDS {}
};

LT_API Texture2D Texture_Atlas(Vector<Texture2D*> const& souces);

LT_API Texture2D Texture_Create(
  uint width,
  uint height,
  GL_TextureFormat::Enum format = GL_TextureFormat::RGBA8,
  void const* data = nullptr);

LT_API Texture2D Texture2D_Filter(
  Texture2D const& texture,
  Shader const& shader);

LT_API void Texture_Generate(
  Texture2D const& source,
  Shader const& shader,
  bool generateMips = true,
  bool segmented = true,
  float maxJobTime = 1);

LT_API void Texture_Generate(
  Texture2D const& source1,
  Texture2D const& source2,
  Shader const& shader,
  bool generateMips = true,
  bool segmented = true,
  float maxJobTime = 1);

DeclareFunction(Texture_LoadFrom, Texture2D,
  Location, source)

DeclareFunctionNoParams(Texture_ScreenCapture, Texture2D)

#endif
