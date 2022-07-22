#ifndef LTE_RenderPass_h__
#define LTE_RenderPass_h__

#include "BaseType.h"
#include "Reference.h"

struct RenderPassT : public RefCounted {
  BASE_TYPE(RenderPassT)

  LT_API void Render(DrawState* state);

  virtual const char* GetName() const = 0;
  virtual void OnRender(DrawState* state) = 0;
};

#endif
