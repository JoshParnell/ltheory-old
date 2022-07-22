#include "DrawState.h"

#include "LTE/CubeMap.h"
#include "LTE/Data.h"
#include "LTE/Map.h"
#include "LTE/Renderer.h"
#include "LTE/ShaderInstance.h"
#include "LTE/Stack.h"
#include "LTE/StringPool.h"
#include "LTE/Texture3D.h"
#include "LTE/View.h"
#include "LTE/Viewport.h"

#include "Game/Common.h"

namespace {
  typedef Map< char const*, Vector<Data> > DataMap;

  DataMap& GetDataMap() {
    static DataMap dataMap;
    return dataMap;
  }

  char const* GetString(String const& str) {
    static StringPool pool;
    return pool.Get(str);
  }

  V3 GetCamPos() {
    return 0;
  }

  V3 GetCamUp() {
    return DrawState::Get()->view->transform.up;
  }

  Texture2D GetReadBuffer() {
    return DrawState::Get()->primary;
  }

  Texture2D GetDepthBuffer() {
    return DrawState::Get()->depth;
  }

  CubeMap GetEnvMap() {
    return DrawState::Get()->envMap.size()
      ? DrawState::Get()->envMap.back()
      : nullptr;
  }

  CubeMap GetEnvMapLF() {
    return DrawState::Get()->envMapLF.size()
      ? DrawState::Get()->envMapLF.back()
      : nullptr;
  }

  Matrix const* GetInvView() {
    static Matrix viewInverse;
    viewInverse = Renderer_GetViewMatrix().Inverse();
    return &viewInverse;
  }

  Matrix const* GetInvProj() {
    static Matrix projInverse;
    projInverse = Renderer_GetProjMatrix().Inverse();
    return &projInverse;
  }

  float GetGameTime() {
    return (float)(Universe_Age() % (kTimeScale * 60))  / (float)kTimeScale;
  }

  V2 GetFrame() {
    return Viewport_Get()->size;
  }

  V2 GetRcpFrame() {
    return Viewport_Get()->GetRcpFrame();
  }

  template <class FunctionT>
  void Link(ShaderInstance const& s, char const* name, FunctionT& fn) {
    int index = s->GetShader()->QueryUniformLocation(name);
    if (index >= 0)
      (*s)(index, fn);
  }

  template <class FunctionT>
  void Link(Shader const& s, char const* name, FunctionT& fn) {
    int index = s->QueryUniformLocation(name);
    if (index >= 0)
      (*s)(index, fn());
  }

  template <class ShaderT>
  void LinkState(ShaderT const& s) {
    Link(s, "depthBuffer", GetDepthBuffer);
    Link(s, "normalBuffer", GetDepthBuffer);

    /* View. */
    Link(s, "camUp", GetCamUp);
    Link(s, "eye", GetCamPos);
    Link(s, "frame", GetFrame);
    Link(s, "rcpFrame", GetRcpFrame);
    Link(s, "envMap", GetEnvMap);
    Link(s, "envMapLF", GetEnvMapLF);

    Link(s, "INVVIEW", GetInvView);
    Link(s, "INVPROJ", GetInvProj);

    /* Time. */
    Link(s, "time", GetGameTime);
  }
}

Data const& DrawState_Get(String const& name) {
  static Data nullData;
  Vector<Data> const& dataStack = GetDataMap()[GetString(name)];
  return dataStack.size() ? dataStack.back() : nullData;
}

void DrawState_Inject(Shader const& shader) {
  DataMap& dataMap = GetDataMap();
  for (DataMap::iterator it = dataMap.begin(); it != dataMap.end(); ++it) {
    if (it->second.isEmpty())
      continue;

    int index = shader->QueryUniformLocation(it->first);
    if (index >= 0) {
      Data const& data = it->second.back();
      if (data.type == Type_Get<CubeMap>())
        shader->SetCubeMap(index, data.Convert<CubeMap>());
      else if (data.type == Type_Get<float>())
        shader->SetFloat(index, data.Convert<float>());
      else if (data.type == Type_Get<V2>())
        shader->SetFloat2(index, data.Convert<V2>());
      else if (data.type == Type_Get<V3>())
        shader->SetFloat3(index, data.Convert<V3>());
      else if (data.type == Type_Get<V4>())
        shader->SetFloat4(index, data.Convert<V4>());
      else if (data.type == Type_Get<Texture2D>())
        shader->SetTexture2D(index, data.Convert<Texture2D>());
      else if (data.type == Type_Get<Texture3D>())
        shader->SetTexture3D(index, data.Convert<Texture3D>());
    }
  }
}

void DrawState_Link(Shader const& s) {
  DrawState_Inject(s);
  LinkState(s);
}

void DrawState_Link(ShaderInstance const& s) {
  DrawState_Inject(s->GetShader());
  LinkState(s);
}

DefineFunction(DrawState_Clear) {
  GetDataMap().clear();
}

DefineFunction(DrawState_Pop) {
  GetDataMap()[GetString(args.name)].pop();
}

DefineFunction(DrawState_Push) {
  GetDataMap()[GetString(args.name)].push(args.data);
}
