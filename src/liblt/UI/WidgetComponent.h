#ifndef UI_WidgetComponent_h__
#define UI_WidgetComponent_h__

#include "Common.h"
#include "LTE/BaseType.h"
#include "LTE/Reference.h"

#define WIDGET_FUNCTION_X                                                      \
  X(CreateChildren, Vector<Widget>)                                            \
  X(GetHash, HashT)                                                            \
  X(GetName, String)                                                           \
  X(GetPadding, V4)                                                            \
  X(IsAlive, bool)

struct WidgetComponentT : public RefCounted {
  BASE_TYPE(WidgetComponentT)

  WidgetComponent next;

#define X(name, type)                                                          \
  virtual void name(Widget const&, type&) {}
  WIDGET_FUNCTION_X
#undef X

  virtual void CaptureFocus(Widget const&) {}
  virtual void Create(Widget const&) {}
  virtual void Destroy(Widget const&) {}
  virtual void Draw(Widget const&) {}
  virtual void PostDraw(Widget const&) {}
  virtual void PostPosition(Widget const&) {}
  virtual void PostUpdate(Widget const&) {}
  virtual void PreDraw(Widget const&) {}
  virtual void PrePosition(Widget const&) {}
  virtual void PreUpdate(Widget const&) {}
  virtual void Receive(Widget const&, Data const&) {}
};

#endif
