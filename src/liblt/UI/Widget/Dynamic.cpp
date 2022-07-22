#include "../Widgets.h"

#include "LTE/AutoClass.h"
#include "LTE/Pool.h"
#include "LTE/HashMap.h"

#include "UI/Widget.h"

#include "LTE/Debug.h"

namespace {
  typedef HashMap<HashT, Widget> ChildMapT;

  AutoClassDerived(WidgetDynamic, WidgetComponentT,
    ChildMapT, childrenMap)
    DERIVED_TYPE_EX(WidgetDynamic)
    POOLED_TYPE

    WidgetDynamic() {}

    void PreUpdate(Widget const& self) {
      for (size_t i = 0; i < self->children.size(); ++i)
        self->children[i]->visited = false;

      /* Dynamically map new children. */ {
        Vector<Widget> newChildren;
        self->CreateChildren(newChildren);

        for (size_t i = 0; i < newChildren.size(); ++i) {
          HashT hash = newChildren[i]->GetHash();

          Widget& child = childrenMap[hash];
          if (child)
            child->visited = true;
          else {
            child = newChildren[i];
            self->AddChild(child);
          }
        }
      }

      /* Delete orphaned or invalidated children. */ {
        for (int i = 0; i < (int)self->children.size(); ++i) {
          Widget const& child = self->children[i];
          if (!child->visited) {
            childrenMap.erase(child->GetHash());
            child->Clear();
            self->children.eraseIndex(i);
            i--;
          }
        }

        for (size_t i = 0; i < self->children.size(); ++i)
          childrenMap[self->children[i]->GetHash()] = self->children[i];
      }
    }
  };
}

DefineFunction(Widget_Dynamic) {
  args.widget->Add(new WidgetDynamic());
  return args.widget;
} FunctionAlias(Widget_Dynamic, Dynamic);
