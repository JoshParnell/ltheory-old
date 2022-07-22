#include "RenderStyle.h"

#include "Vector.h"

namespace {
  Vector<RenderStyle>& GetStack() {
    static Vector<RenderStyle> stack;
    return stack;
  }
}

RenderStyle RenderStyle_Get() {
  return GetStack().size() ? GetStack().back() : nullptr;
}

void RenderStyle_Pop() {
  GetStack().back()->OnEnd();
  GetStack().pop();
}

void RenderStyle_Push(RenderStyle const& style) {
  GetStack().push(style);
  style->OnBegin();
}
