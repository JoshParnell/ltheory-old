#include "../Widgets.h"

#include "LTE/AutoClass.h"
#include "LTE/Pool.h"
#include "LTE/V4.h"

#include "UI/Widget.h"

namespace {
  AutoClassDerived(WidgetList, WidgetComponentT,
    float, spacing,
    bool, vertical)
    DERIVED_TYPE_EX(WidgetList)
    POOLED_TYPE

    WidgetList() {}

    void PostPosition(Widget const& self) {
      V4 padding = self->GetPadding();
      V2 pos = self->pos + V2(padding.x, padding.y);
      V2 innerSize = self->size - V2(padding.x + padding.z, padding.y + padding.w);
      float totalGreed = 0;
      float totalSize = 0;

      for (size_t i = 0; i < self->children.size(); ++i) {
        Widget const& child = self->children[i];
        child->pos += pos;

        if (vertical) {
          pos.y += child->size.y + spacing;
          child->maxSize.x = innerSize.x;
          child->maxSize.y = child->size.y;
          totalGreed += child->greed.y;
          totalSize += child->size.y;
        } else {
          pos.x += child->size.x + spacing;
          child->maxSize.x = child->size.x;
          child->maxSize.y = innerSize.y;
          totalGreed += child->greed.x;
          totalSize += child->size.x;
        }

        if (i)
          totalSize += spacing;
      }

      float remainder = vertical
        ? innerSize.y - totalSize
        : innerSize.x - totalSize;


      if (remainder > 0 && totalGreed > 0) {
        remainder /= totalGreed;
        float shift = 0;

        for (size_t i = 0; i < self->children.size(); ++i) {
          Widget const& child = self->children[i];
          if (vertical) {
            child->pos.y += shift;
            child->maxSize.y += child->greed.y * remainder;
            shift += child->greed.y * remainder;
          } else {
            child->pos.x += shift;
            child->maxSize.x += child->greed.x * remainder;
            shift += child->greed.x * remainder;
          }
        }
      }
    }

    void PrePosition(Widget const& self) {
      V2 minSize = 0;
      V4 padding = self->GetPadding();

      for (size_t i = 0; i < self->children.size(); ++i) {
        Widget const& child = self->children[i];
        if (vertical) {
          minSize.x = Max(minSize.x, child->size.x);
          minSize.y += child->size.y;
          if (i) minSize.y += spacing;
        } else {
          minSize.y = Max(minSize.y, child->size.y);
          minSize.x += child->size.x;
          if (i) minSize.x += spacing;
        }
      }

      self->minSize = Max(self->minSize, minSize);
      self->minSize += V2(padding.x, padding.y) + V2(padding.z, padding.w);
    }
  };
}

DefineFunction(Widget_ListHorizontal) {
  args.widget->Add(new WidgetList(args.spacing, false));
  return args.widget;
} FunctionAlias(Widget_ListHorizontal, ListH);

DefineFunction(Widget_ListVertical) {
  args.widget->Add(new WidgetList(args.spacing, true));
  return args.widget;
} FunctionAlias(Widget_ListVertical, ListV);
