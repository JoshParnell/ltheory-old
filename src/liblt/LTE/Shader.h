#ifndef LTE_Shader_h__
#define LTE_Shader_h__

#include "DeclareFunction.h"
#include "GLType.h"
#include "Reference.h"
#include "String.h"

struct ShaderT : public RefCounted {
  virtual ~ShaderT() {}

  virtual bool Create(String const& vsCode, String const& fsCode) = 0;

  virtual void BindInput(size_t attribIndex, char const* name) = 0;
  virtual void BindOutput(size_t bufferIndex, char const* name) = 0;
  
  virtual void BindMatrices(
    Matrix const& world,
    Matrix const& view,
    Matrix const& proj,
    Matrix const& worldIT,
    Matrix const& wvp) = 0;

  virtual int GetUniformLocation(char const* name) = 0;
  virtual int QueryUniformLocation(char const* name) = 0;

  virtual void PrintLogs() const = 0;
  virtual void Relink() = 0;

  virtual ShaderT& SetCubeMap(char const* var, CubeMap const& map) = 0;
  virtual ShaderT& SetCubeMap(int varIndex, CubeMap const& map) = 0;
  
  virtual ShaderT& SetFloat(char const* var, float f) = 0;
  virtual ShaderT& SetFloat(int varIndex, float f) = 0;
  
  virtual ShaderT& SetFloatArray(char const* var, float const* data, size_t size) = 0;
  virtual ShaderT& SetFloatArray(int varIndex, float const* data, size_t size) = 0;
  
  virtual ShaderT& SetFloat2(char const* var, V2 const& v) = 0;
  virtual ShaderT& SetFloat2(int varIndex, V2 const& v) = 0;
  
  virtual ShaderT& SetFloat3(char const* var, V3 const& v) = 0;
  virtual ShaderT& SetFloat3(int varIndex, V3 const& v) = 0;
  
  virtual ShaderT& SetFloat3Array(char const* var, V3 const* data, size_t size) = 0;
  virtual ShaderT& SetFloat3Array(int varIndex, V3 const* data, size_t size) = 0;
  
  virtual ShaderT& SetFloat4(char const* var, V4 const& v) = 0;
  virtual ShaderT& SetFloat4(int varIndex, V4 const& v) = 0;
  
  virtual ShaderT& SetFloat4Array(char const* var, V4 const* data, size_t size) = 0;
  virtual ShaderT& SetFloat4Array(int varIndex, V4 const* data, size_t size) = 0;
  
  virtual ShaderT& SetMatrix(char const* var, Matrix const* m) = 0;
  virtual ShaderT& SetMatrix(int varIndex, Matrix const* m) = 0;
  
  virtual ShaderT& SetInt(char const* var, int i) = 0;
  virtual ShaderT& SetInt(int varIndex, int i) = 0;
  
  virtual ShaderT& SetTexture2D(char const* var, Texture2D const& t) = 0;
  virtual ShaderT& SetTexture2D(int varIndex, Texture2D const& t) = 0;
  
  virtual ShaderT& SetTexture3D(char const* var, Texture3D const& t) = 0;
  virtual ShaderT& SetTexture3D(int varIndex, Texture3D const& t) = 0;

  virtual void Use() = 0;

  ShaderT& operator()(char const* var, CubeMap const& map) {
    return SetCubeMap(var, map);
  }

  ShaderT& operator()(int varIndex, CubeMap const& map) {
    return SetCubeMap(varIndex, map);
  }

  ShaderT& operator()(char const* var, float f) {
    return SetFloat(var, f);
  }

  ShaderT& operator()(int varIndex, float f) {
    return SetFloat(varIndex, f);
  }

  ShaderT& operator()(char const* var, V2 const& v) {
    return SetFloat2(var, v);
  }

  ShaderT& operator()(int varIndex, V2 const& v) {
    return SetFloat2(varIndex, v);
  }

  ShaderT& operator()(char const* var, V3 const& v) {
    return SetFloat3(var, v);
  }

  ShaderT& operator()(int varIndex, V3 const& v) {
    return SetFloat3(varIndex, v);
  }

  ShaderT& operator()(char const* var, V4 const& v) {
    return SetFloat4(var, v);
  }

  ShaderT& operator()(int varIndex, V4 const& v) {
    return SetFloat4(varIndex, v);
  }

  ShaderT& operator()(char const* var, Matrix const* m) {
    return SetMatrix(var, m);
  }

  ShaderT& operator()(int varIndex, Matrix const* m) {
    return SetMatrix(varIndex, m);
  }

  ShaderT& operator()(char const* var, int i) {
    return SetInt(var, i);
  }

  ShaderT& operator()(int varIndex, int i) {
    return SetInt(varIndex, i);
  }

  ShaderT& operator()(char const* var, Texture2D const& t) {
    return SetTexture2D(var, t);
  }

  ShaderT& operator()(int varIndex, Texture2D const& t) {
    return SetTexture2D(varIndex, t);
  }

  ShaderT& operator()(char const* var, Texture3D const& t) {
    return SetTexture3D(var, t);
  }

  ShaderT& operator()(int varIndex, Texture3D const& t) {
    return SetTexture3D(varIndex, t);
  }
};

DeclareFunction(Shader_Create, Shader,
  String, vsPath,
  String, fsPath)

LT_API Shader Shader_Create(
  String const& vsPath,
  String const& fsPath,
  String const& vsHeader,
  String const& fsHeader);

LT_API ShaderT* Shader_GetActive();
LT_API GL_Program Shader_GetCurrentProgram();

DeclareFunctionNoParams(Shader_RecompileAll, void)

LT_API void Shader_UseFixedFunction();

#endif
