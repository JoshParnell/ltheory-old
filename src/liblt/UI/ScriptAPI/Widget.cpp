#include "UI/Widget.h"

#include "LTE/Color.h"
#include "LTE/Data.h"
#include "LTE/Function.h"
#include "LTE/VectorNP.h"

#include "LTE/Debug.h"

TypeAlias(Reference<WidgetT>, Widget);

AutoClass(WidgetChildIterator,
  Widget, widget,
  size_t, index)
  WidgetChildIterator() {}
};

namespace Children {
  FreeFunction(WidgetChildIterator, Widget_GetChildren,
    "Return an iterator to the children of 'widget'",
    Widget, widget)
  {
    return WidgetChildIterator(widget, 0);
  } FunctionAlias(Widget_GetChildren, GetChildren);

  FreeFunction(Widget, WidgetChildIterator_Access,
    "Return the value of 'iterator'",
    WidgetChildIterator, iterator)
  {
    return iterator.widget->children[iterator.index];
  } FunctionAlias(WidgetChildIterator_Access, Get);

  VoidFreeFunction(WidgetChildIterator_Advance,
    "Advance 'iterator",
    WidgetChildIterator, iterator)
  {
    Mutable(iterator).index++;
  } FunctionAlias(WidgetChildIterator_Advance, Advance);

  FreeFunction(bool, WidgetChildIterator_HasMore,
    "Return whether 'iterator' has more elements",
    WidgetChildIterator, iterator)
  {
    return iterator.index < iterator.widget->children.size();
  } FunctionAlias(WidgetChildIterator_HasMore, HasMore);

  FreeFunction(int, WidgetChildIterator_Size,
    "Return the total number of elements in 'iterator'",
    WidgetChildIterator, iterator)
  {
    return (int)iterator.widget->children.size();
  } FunctionAlias(WidgetChildIterator_Size, Size);
}

VoidFreeFunction(Widget_AddChild,
  "Add 'child' to 'widget'",
  Widget, widget,
  Widget, child)
{
  widget->AddChild(child);
} FunctionAlias(Widget_AddChild, AddChild);

FreeFunction(V2, Widget_Center,
  "Return the center of 'widget'",
  Widget, widget)
{
  return widget->pos + 0.5f * widget->size;
} FunctionAlias(Widget_Center, Center);

FreeFunction(ListNP, Widget_CreateChildren,
  "Return a list of children created by 'widget'",
  Widget, widget)
{
  Vector<Widget> children;
  widget->CreateChildren(children);
  ListNP list = new ListNPT(Type_Get<Widget>());
  for (size_t i = 0; i < children.size(); ++i)
    list->Append(children[i]);
  return list;
} FunctionAlias(Widget_CreateChildren, CreateChildren);

VoidFreeFunction(Widget_Delete,
  "Mark 'widget' for deletion",
  Widget, widget)
{
  widget->deleted = true;
} FunctionAlias(Widget_Delete, Delete);

FreeFunction(String, Widget_GetName,
  "Return the name of 'widget'",
  Widget, widget)
{
  return widget->GetName();
} FunctionAlias(Widget_GetName, GetName);

FreeFunction(Widget, Widget_GetParent,
  "Return the parent of 'widget'",
  Widget, widget)
{
  return widget->parent.t;
} FunctionAlias(Widget_GetParent, GetParent);

#define X(x)                                                                   \
  FreeFunction(bool, Widget_HasFocus##x##Recursive,                            \
    "Return whether 'widget' or any of its children have " #x " focus",        \
    Widget, widget)                                                            \
  {                                                                            \
    return widget->HasFocus##x();                                              \
  } FunctionAlias(Widget_HasFocus##x##Recursive, HasFocus##x##Recursive);
  WIDGET_FOCUS_X
#undef X

FreeFunction(bool, Widget_HasParent,
  "Return whether 'widget' has a parent",
  Widget, widget)
{
  return widget->parent != nullptr;
} FunctionAlias(Widget_HasParent, HasParent);

VoidFreeFunction(Widget_Rebuild,
  "Force 'widget' to clear and re-create all children",
  Widget, widget)
{
  widget->Rebuild();
} FunctionAlias(Widget_Rebuild, Rebuild);

VoidFreeFunction(Widget_Send,
  "Send 'message' to all components of 'widget'",
  Widget, widget,
  Data, message)
{
  widget->Send(message);
} FunctionAlias(Widget_Send, Send);

VoidFreeFunction(Widget_SendDown,
  "Send 'message' recursively to all children of 'widget'",
  Widget, widget,
  Data, message)
{
  widget->SendDown(message);
} FunctionAlias(Widget_SendDown, SendDown);

VoidFreeFunction(Widget_SendSiblings,
  "Send 'message' to all siblings of 'widget'",
  Widget, widget,
  Data, message)
{
  if (widget->parent) {
    for (size_t i = 0; i < widget->children.size(); ++i) {
      Widget const& child = widget->children[i];
      if (child != widget)
        child->Send(message);
    }
  }
} FunctionAlias(Widget_SendSiblings, SendSiblings);

VoidFreeFunction(Widget_SendUp,
  "Send 'message' recursively to all parents of 'widget'",
  Widget, widget,
  Data, message)
{
  widget->SendUp(message);
} FunctionAlias(Widget_SendUp, SendUp);

FreeFunction(V2, Widget_BottomCenter,
  "Return the position of the bottom center of 'widget'",
  Widget, widget)
{
  return widget->pos + V2(0.5f * widget->size.x, widget->size.y);
} FunctionAlias(Widget_BottomCenter, BottomCenter);

FreeFunction(V2, Widget_BottomLeft,
  "Return the position of the bottom left of 'widget'",
  Widget, widget)
{
  return widget->pos + V2(0, widget->size.y);
} FunctionAlias(Widget_BottomLeft, BottomLeft);

FreeFunction(V2, Widget_BottomRight,
  "Return the position of the bottom right of 'widget'",
  Widget, widget)
{
  return widget->pos + widget->size;
} FunctionAlias(Widget_BottomRight, BottomRight);

FreeFunction(V2, Widget_GetPoint,
  "Return the position of the interpolant on 'widget'",
  Widget, widget,
  float, x,
  float, y)
{
  return widget->pos + V2(x, y) * widget->size;
} FunctionAlias(Widget_GetPoint, GetPoint);

FreeFunction(V2, Widget_LeftCenter,
  "Return the position of the left center of 'widget'",
  Widget, widget)
{
  return widget->pos + V2(0, 0.5f * widget->size.y);
} FunctionAlias(Widget_LeftCenter, LeftCenter);

VoidFreeFunction(Widget_RemoveChild,
  "Remove 'child' from 'widget'",
  Widget, widget,
  Widget, child)
{
  widget->RemoveChild(child);
} FunctionAlias(Widget_RemoveChild, RemoveChild);

FreeFunction(V2, Widget_RightCenter,
  "Return the position of the right center of 'widget'",
  Widget, widget)
{
  return widget->pos + V2(widget->size.x, 0.5f * widget->size.y);
} FunctionAlias(Widget_RightCenter, RightCenter);

FreeFunction(V2, Widget_TopCenter,
  "Return the position of the top center of 'widget'",
  Widget, widget)
{
  return widget->pos + V2(0.5f * widget->size.x, 0);
} FunctionAlias(Widget_TopCenter, TopCenter);

FreeFunction(V2, Widget_TopLeft,
  "Return the position of the top left of 'widget'",
  Widget, widget)
{
  return widget->pos;
} FunctionAlias(Widget_TopLeft, TopLeft);

FreeFunction(V2, Widget_TopRight,
  "Return the position of the top right of 'widget'",
  Widget, widget)
{
  return widget->pos + V2(widget->size.x, 0);
} FunctionAlias(Widget_TopRight, TopRight);

FreeFunction(bool, Widget_Equal, "Return 'a' == 'b'", Widget, a, Widget, b) {
  return a == b;
} FunctionAlias(Widget_Equal, ==);

FreeFunction(bool, Widget_NotEqual, "Return 'a' != 'b'", Widget, a, Widget, b) {
  return a != b;
} FunctionAlias(Widget_NotEqual, !=);
