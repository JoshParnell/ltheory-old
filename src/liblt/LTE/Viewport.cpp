#include "Viewport.h"

#include "Matrix.h"
#include "Renderer.h"
#include "Vector.h"
#include "Window.h"

namespace {
  Vector<Viewport>& GetStack() {
    static Vector<Viewport> stack;
    return stack;
  }
}

Matrix ViewportT::GetMatrix() const {
  return
    Matrix::Translation(V3(-1.0f, windowSpace ? 1.0f : -1.0f, 0.0f)) *
    Matrix::Scale(V3(2.0f / size.x, (windowSpace ? -2.0f : 2.0f) / size.y, 1.0f)) *
    Matrix::Translation(V3(-position.x, -position.y, 0));
}

void ViewportT::LoadMatrix() const {
  Renderer_SetProjMatrix(GetMatrix());
}

V2 ViewportT::FromNDC(V2 const& p) const {
  return Transform(V2(0.5f * p.x + 0.5f,
                      windowSpace ? 0.5f - 0.5f * p.y : 0.5f + 0.5f * p.y));
}

V2 ViewportT::ToNDC(V2 const& point) const {
  V2 p = InvTransform(point);
  return V2(2.0f * p.x - 1.0f,
            windowSpace ? 1.0f - 2.0f * p.y : 2.0f * p.y - 1.0f);
}

V2 ViewportT::Transform(V2 const& p) const {
  return (p * size) + position;
}

V2 ViewportT::InvTransform(V2 const& p) const {
  return (p - position) / size;
}

Viewport Viewport_Get() {
  return GetStack().size() ? GetStack().back() : nullptr;
}

void Viewport_Pop() {
  GetStack().pop();
  Renderer_PopViewport();
  if (GetStack().size())
    GetStack().back()->LoadMatrix();
}

void Viewport_Push(Viewport const& vp) {
  GetStack().push(vp);

  /* TODO : Nasty. */
  if (vp->windowSpace) {
    V2U windowSize = Window_Get()->GetSize();
    Renderer_PushViewport(
      (int)vp->position.x,
      windowSize.y - (int)(vp->position.y + vp->size.y),
      (int)vp->size.x,
      (int)vp->size.y);
  } else {
    Renderer_PushViewport(
      (int)vp->position.x, (int)vp->position.y,
      (int)vp->size.x,
      (int)vp->size.y);
  }

  vp->LoadMatrix();
}
