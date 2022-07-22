#include "Widget.h"
#include "Icon.h"

#include "LTE/Color.h"
#include "LTE/Data.h"
#include "LTE/V4.h"

#include "Module/Settings.h"

#define X(name, type)                                                          \
  type WidgetT::name() { type t; name(t); return t; }
  WIDGET_FUNCTION_X
#undef X

void WidgetT::AddChild(Widget const& child) {
  children.push(child);
  child->parent = this;
  child->Create();
}

void WidgetT::CreateChildrenDefault(Vector<Widget>& out) {}

void WidgetT::Clear() {
  Destroy();
  for (size_t i = 0; i < children.size(); ++i)
    children[i]->Clear();
  children.clear();

  deleted = false;
  initialized = false;
  parent = nullptr;
  visited = false;

#define X(x) focus##x = false;
  WIDGET_FOCUS_X
#undef X
}

void WidgetT::Draw() {
  if (!initialized)
    return;

  for (size_t i = 0; i < components.size(); ++i)
    components[components.size() - (i + 1)]->PreDraw(this);

  for (size_t i = 0; i < children.size(); ++i) {
    Widget const& child = children[i];
    child->alpha *= alpha;
    child->Draw();
  }

  for (size_t i = 0; i < components.size(); ++i)
    components[i]->PostDraw(this);
}

void WidgetT::GetAlphaDefault(float& out) {
  out = 1.0f;
}

void WidgetT::GetColorDefault(Color& out) {
  out = Settings_PrimaryColor()();
}

void WidgetT::GetHashDefault(HashT& out) {
  String name;
  GetName(name);
  out = String_Hash(name);
}

void WidgetT::GetNameDefault(String& out) {}

void WidgetT::GetPaddingDefault(V4& out) {
  out = 0;
}

void WidgetT::GetPosDefault(V2& out) {
  out = 0;
}

void WidgetT::GetSizeDefault(V2& out) {
  out = 16;
}

void WidgetT::IsAliveDefault(bool& out) {
  out = true;
}

void WidgetT::PrePosition() {
  /* Initialize cached state. */
  GetAlphaDefault(alpha);
  GetColorDefault(color);
  GetPosDefault(pos);
  size = 0;
  minSize = 0;
  maxSize = 0;
  greed = 0;

  if (rebuild) {
    rebuild = false;
    initialized = false;
    for (size_t i = 0; i < children.size(); ++i)
      children[i]->Clear();
    children.clear();
  }

  if (!initialized) {
    initialized = true;
    CreateChildren(children);
    for (size_t i = 0; i < children.size(); ++i) {
      Widget const& child = children[i];
      child->parent = this;
      child->Create();
    }
  }

  for (int i = 0; i < (int)children.size(); ++i) {
    Widget const& child = children[i];
    child->PrePosition();
    if (child->deleted) {
      child->Clear();
      children.eraseIndex(i);
      i--;
    }
  }

  for (size_t i = 0; i < components.size(); ++i)
    components[i]->PrePosition(this);

  size = minSize;
}

void WidgetT::PostPosition() {
  for (size_t i = 0; i < components.size(); ++i)
    components[components.size() - (i + 1)]->PostPosition(this);
  for (size_t i = 0; i < children.size(); ++i)
    children[i]->PostPosition();

#define X(x) focus##x = false;
  WIDGET_FOCUS_X
#undef X
}

void WidgetT::RemoveChild(Widget const& child) {
  if (child->parent == this) {
    children.erase(child);
    child->parent = nullptr;
    child->Destroy();
  }
}

void WidgetT::Update(WidgetUpdateState& state) {
  for (size_t i = 0; i < components.size(); ++i)
    components[i]->PreUpdate(this);
  for (size_t i = 0; i < children.size(); ++i)
    children[children.size() - (i + 1)]->Update(state);

  CaptureFocus();

#define X(x)                                                                   \
  if (state.focus##x)                                                          \
    focus##x = false;                                                          \
  else                                                                         \
    state.focus##x = focus##x;
  WIDGET_FOCUS_X
#undef X

  for (size_t i = 0; i < components.size(); ++i)
    components[components.size() - (i + 1)]->PostUpdate(this);
}

DefineFunction(Widget_Create) {
  return new WidgetT;
} FunctionAlias(Widget_Create, Widget);

DefineFunction(Widget_Create1) {
  Widget widget = Widget_Create();
  widget->Add(args.component);
  return widget;
}
