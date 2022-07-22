#ifndef UI_Widget_h__
#define UI_Widget_h__

#include "WidgetComponent.h"
#include "LTE/Color.h"
#include "LTE/Data.h"
#include "LTE/DeclareFunction.h"
#include "LTE/Pointer.h"
#include "LTE/V2.h"
#include "LTE/Vector.h"

struct WidgetUpdateState {
#define X(x) bool focus##x;
  WIDGET_FOCUS_X
#undef X

  WidgetUpdateState() {
#define X(x) focus##x = false;
  WIDGET_FOCUS_X
#undef X
  }
};

struct WidgetT : public RefCounted {
  typedef RefCounted BaseType;
  typedef WidgetT SelfType;

  Vector<WidgetComponent> components;
  Vector<Widget> children;
  Pointer<WidgetT> parent;

  /* Cached state. */
  V2 pos;
  V2 size;
  V2 minSize;
  V2 maxSize;
  V2 greed;
  Color color;
  float alpha;
  bool deleted;
  bool initialized;
  bool rebuild;
  bool visited;

#define X(x) bool focus##x;
  WIDGET_FOCUS_X
#undef X

  WidgetT() :
    pos(0),
    size(0),
    minSize(0),
    maxSize(0),
    greed(0),
    color(0),
    alpha(1),
    deleted(false),
    initialized(false),
    rebuild(false),
    visited(true)
  {
#define X(x) focus##x = false;
  WIDGET_FOCUS_X
#undef X
  }

#define X(name, type)                                                          \
  LT_API void name##Default(type& out);                                        \
                                                                               \
  void name(type& out) {                                                       \
    name##Default(out);                                                        \
    for (size_t i = 0; i < components.size(); ++i)                             \
      components[i]->name(this, out);                                          \
  }                                                                            \
                                                                               \
  LT_API type name();
  WIDGET_FUNCTION_X
#undef X

  LT_API void GetAlphaDefault(float&);
  LT_API void GetColorDefault(Color&);
  LT_API void GetPosDefault(V2&);
  LT_API void GetSizeDefault(V2&);

  void Add(WidgetComponent const& component) {
    components.push(component);
  }

  void CaptureFocus() {
    for (size_t i = 0; i < components.size(); ++i)
      components[i]->CaptureFocus(this);
  }

  void Create() {
    for (size_t i = 0; i < components.size(); ++i)
      components[i]->Create(this);
  }

  void Destroy() {
    for (size_t i = 0; i < components.size(); ++i)
      components[i]->Destroy(this);
  }

#define X(x)                                                                   \
  bool HasFocus##x() const {                                                   \
    if (focus##x)                                                              \
      return true;                                                             \
    for (size_t i = 0; i < children.size(); ++i)                               \
      if (children[i]->HasFocus##x())                                          \
        return true;                                                           \
    return false;                                                              \
  }
  WIDGET_FOCUS_X
#undef X

  void Rebuild() {
    rebuild = true;
  }

  void Send(Data const& data) {
    for (size_t i = 0; i < components.size(); ++i)
      components[i]->Receive(this, data);
  }

  void SendDown(Data const& data) {
    for (size_t i = 0; i < children.size(); ++i) {
      children[i]->Send(data);
      children[i]->SendDown(data);
    }
  }

  void SendSiblings(Data const& data) {
    if (parent)
      for (size_t i = 0; i < parent->children.size(); ++i)
        if (parent->children[i] != this)
          parent->children[i]->Send(data);
  }

  void SendUp(Data const& data) {
    if (parent) {
      parent->Send(data);
      parent->SendUp(data);
    }
  }

  LT_API void AddChild(Widget const&);
  LT_API void Clear();
  LT_API void Draw();
  LT_API void PrePosition();
  LT_API void PostPosition();
  LT_API void RemoveChild(Widget const&);
  LT_API void Update(WidgetUpdateState&);

  FIELDS {
    MAPFIELD(pos)
    MAPFIELD(size)
    MAPFIELD(minSize)
    MAPFIELD(maxSize)
    MAPFIELD(greed)
    MAPFIELD(color)
    MAPFIELD(alpha)
#define X(x) MAPFIELD(focus##x)
  WIDGET_FOCUS_X
#undef X
  }

  DefineMetadataInline(WidgetT)
};

DeclareFunctionNoParams(Widget_Create, Widget)

DeclareFunction(Widget_Create1, Widget,
  WidgetComponent, component)

#endif
