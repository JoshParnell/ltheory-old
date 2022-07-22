#ifndef LTE_ShaderInstance_h__
#define LTE_ShaderInstance_h__

#include "Color.h"
#include "DeclareFunction.h"
#include "Enum.h"
#include "Generic.h"
#include "Reference.h"
#include "Shader.h"
#include "V2.h"
#include "V3.h"
#include "V4.h"

#define RENDERSTATESWITCH_X                                                    \
  X(BlendModeAdditive)                                                         \
  X(BlendModeAlpha)                                                            \
  X(BlendModeComplementary)                                                    \
  X(BlendModeDisabled)                                                         \
  X(CullModeBackface)                                                          \
  X(CullModeDisabled)                                                          \
  X(WireframeOn)                                                               \
  X(WireframeOff)                                                              \
  X(ZBufferOn)                                                                 \
  X(ZBufferOff)                                                                \
  X(ZWritableOn)                                                               \
  X(ZWritableOff)

#define XTYPE RenderStateSwitch
#define XLIST RENDERSTATESWITCH_X
#include "LTE/XEnum.h"
#undef XLIST
#undef XTYPE

struct ShaderInstanceT : public RefCounted {
  struct ShaderInstanceData* d;

  LT_API ShaderInstanceT(Shader const& shader);
  LT_API ~ShaderInstanceT();

  LT_API ShaderInstance Clone() const;

  LT_API void Begin();
  LT_API void End();

  LT_API Shader const& GetShader() const;
  LT_API bool HasBlending() const;
  LT_API bool HasState(RenderStateSwitch) const;
  LT_API void Remove(char const* name);

  /* Immediate value setters. */
  LT_API ShaderInstanceT& SetCubeMap(
    char const* name,
    Generic<CubeMap, void> const& e);
  LT_API ShaderInstanceT& SetCubeMap(
    int index,
    Generic<CubeMap, void> const& e);

  LT_API ShaderInstanceT& SetFloat(char const* name, GenericFloat const& f);
  LT_API ShaderInstanceT& SetFloat(int index, GenericFloat const& f);

  LT_API ShaderInstanceT& SetFloat2(char const* name, GenericV2 const& v);
  LT_API ShaderInstanceT& SetFloat2(int index, GenericV2 const& v);

  LT_API ShaderInstanceT& SetFloat3(char const* name, GenericV3 const& v);
  LT_API ShaderInstanceT& SetFloat3(int index, GenericV3 const& v);

  LT_API ShaderInstanceT& SetFloat4(char const* name, GenericV4 const& v);
  LT_API ShaderInstanceT& SetFloat4(int index, GenericV4 const& v);

  LT_API ShaderInstanceT& SetInt(char const* name, GenericInt const& i);
  LT_API ShaderInstanceT& SetInt(int index, GenericInt const& i);

  LT_API ShaderInstanceT& SetMatrix(char const* name, Generic<Matrix const*, void> const& m);
  LT_API ShaderInstanceT& SetMatrix(int index, Generic<Matrix const*, void> const& m);

  LT_API ShaderInstanceT& SetState(RenderStateSwitch state);

  LT_API ShaderInstanceT& SetTexture2D(
    char const* name,
    Generic<Texture2D, void> const& t);
  LT_API ShaderInstanceT& SetTexture2D(
    int index,
    Generic<Texture2D, void> const& t);

  LT_API ShaderInstanceT& SetTexture3D(
    char const* name,
    Generic<Texture3D, void> const& t);
  LT_API ShaderInstanceT& SetTexture3D(
    int index,
    Generic<Texture3D, void> const& t);

  /* Convenient operator setters. */
  ShaderInstanceT& operator()(int index, Generic<CubeMap, void> const& e) {
    return SetCubeMap(index, e);
  }

  ShaderInstanceT& operator()(char const* name, Generic<CubeMap, void> const& e) {
    return SetCubeMap(name, e);
  }

  ShaderInstanceT& operator()(int index, GenericFloat const& f) {
    return SetFloat(index, f);
  }

  ShaderInstanceT& operator()(char const* name, GenericFloat const& f) {
    return SetFloat(name, f);
  }

  ShaderInstanceT& operator()(int index, GenericV2 const& v) {
    return SetFloat2(index, v);
  }

  ShaderInstanceT& operator()(char const* name, GenericV2 const& v) {
    return SetFloat2(name, v);
  }

  ShaderInstanceT& operator()(int index, GenericV3 const& v) {
    return SetFloat3(index, v);
  }

  ShaderInstanceT& operator()(char const* name, GenericV3 const& v) {
    return SetFloat3(name, v);
  }

  ShaderInstanceT& operator()(int index, GenericV4 const& v) {
    return SetFloat4(index, v);
  }

  ShaderInstanceT& operator()(char const* name, GenericV4 const& v) {
    return SetFloat4(name, v);
  }

  ShaderInstanceT& operator()(
    int index,
    Generic<Matrix const*, void> const& m)
  {
    return SetMatrix(index, m);
  }

  ShaderInstanceT& operator()(
    char const* name,
    Generic<Matrix const*, void> const& m)
  {
    return SetMatrix(name, m);
  }

  ShaderInstanceT& operator()(RenderStateSwitch state) {
    return SetState(state);
  }

  ShaderInstanceT& operator()(int index, Generic<Texture2D, void> const& t) {
    return SetTexture2D(index, t);
  }

  ShaderInstanceT& operator()(char const* name, Generic<Texture2D, void> const& t) {
    return SetTexture2D(name, t);
  }

  ShaderInstanceT& operator()(int index, Generic<Texture3D, void> const& t) {
    return SetTexture3D(index, t);
  }

  ShaderInstanceT& operator()(char const* name, Generic<Texture3D, void> const& t) {
    return SetTexture3D(name, t);
  }
};

DeclareFunction(ShaderInstance_Create, ShaderInstance,
  Shader, shader)

inline ShaderInstance ShaderInstance_Create(
  String const& vs,
  String const& ps)
{
  return ShaderInstance_Create(Shader_Create(vs, ps));
}

#endif
