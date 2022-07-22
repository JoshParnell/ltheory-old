#ifndef UI_Compositor_h__
#define UI_Compositor_h__

#include "Common.h"
#include "LTE/Reference.h"

struct CompositorT : public RefCounted {
  virtual ~CompositorT() {}

  virtual void Composite(
    Texture2D const& layer,
    Mesh const& surface) = 0;

  virtual void Update() {}
};

#endif
