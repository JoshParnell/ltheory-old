#ifndef UI_Widgets_h__
#define UI_Widgets_h__

#include "WidgetComponent.h"

#include "LTE/DeclareFunction.h"

DeclareFunction(Widget_Custom, Widget,
  Widget, widget,
  Data, data)

DeclareFunction(Widget_Dynamic, Widget,
  Widget, widget)

DeclareFunction(Widget_Layer, Widget,
  Compositor, compositor,
  Mesh, surface,
  float, resolution,
  Widget, widget)

DeclareFunction(Widget_ListHorizontal, Widget,
  float, spacing,
  Widget, widget)

DeclareFunction(Widget_ListVertical, Widget,
  float, spacing,
  Widget, widget)

DeclareFunction(Widget_Rendered, Widget,
  Vector<RenderPass>, passes)

DeclareFunction(Widget_Stack, Widget,
  Widget, widget)

#endif
