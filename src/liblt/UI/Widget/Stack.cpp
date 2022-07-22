#include "../Widgets.h"

#include "LTE/AutoClass.h"
#include "LTE/Pool.h"
#include "LTE/V4.h"

#include "UI/Widget.h"

namespace {
  AutoClassDerivedEmpty(WidgetStack, WidgetComponentT)
    DERIVED_TYPE_EX(WidgetStack)
    POOLED_TYPE

    WidgetStack() {}

    void PostPosition(Widget const& self) {
      V4 padding = self->GetPadding();
      V2 childPos = self->pos + V2(padding.x, padding.y);
      V2 childSize = self->size - V2(padding.x + padding.z, padding.y + padding.w);

      for (size_t i = 0; i < self->children.size(); ++i) {
        Widget const& child = self->children[i];
        child->pos += childPos;
        child->maxSize = childSize;
      }
    }

    void PrePosition(Widget const& self) {
      V2 minSize = 0;
      V4 padding = self->GetPadding();

      for (size_t i = 0; i < self->children.size(); ++i) {
        Widget const& child = self->children[i];
        minSize = Max(minSize, child->minSize);
      }

      self->minSize = Max(self->minSize, minSize);
      self->minSize += V2(padding.x, padding.y) + V2(padding.z, padding.w);
    }
  };
}

DefineFunction(Widget_Stack) {
  args.widget->Add(new WidgetStack);
  return args.widget;
} FunctionAlias(Widget_Stack, Stack);
