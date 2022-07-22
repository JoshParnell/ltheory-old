#ifndef LTE_Viewport_h__
#define LTE_Viewport_h__

#include "Reference.h"
#include "V2.h"

struct ViewportT : public RefCounted {
  V2 position;
  V2 size;
  V2 resolution;
  bool windowSpace;

  float GetAspect() const {
    return size.x / size.y;
  }

  float GetHeight() const {
    return size.y;
  }

  float GetWidth() const {
    return size.x;
  }

  V2 GetRcpFrame() const {
    return V2(1) / size;
  }

  LT_API Matrix GetMatrix() const;
  LT_API void LoadMatrix() const;

  LT_API V2 FromNDC(V2 const& point) const;
  LT_API V2 ToNDC(V2 const& point) const;

  LT_API V2 Transform(V2 const& point) const;
  LT_API V2 InvTransform(V2 const& point) const;

};

inline Viewport Viewport_Create(
  V2 const& position,
  V2 const& size,
  V2 resolution,
  bool windowSpace)
{
  Viewport self = new ViewportT;
  self->position = position;
  self->size = size;
  self->resolution = resolution;
  self->windowSpace = windowSpace;
  return self;
}

LT_API Viewport Viewport_Get();
LT_API void Viewport_Pop();
LT_API void Viewport_Push(Viewport const&);

#endif
