#ifndef UI_Interface_h__
#define UI_Interface_h__

#include "Common.h"
#include "LTE/DeclareFunction.h"
#include "LTE/RenderPass.h"

struct InterfaceT : public RefCounted {
  virtual ~InterfaceT() {}

  virtual void Add(Widget const& widget) = 0;
  virtual void Clear() = 0;
  virtual void Draw() = 0;
  virtual void Update() = 0;
};

DeclareFunction(Interface_Create, Interface,
  String, name)

DeclareFunction(RenderPass_Interface, RenderPass,
  Interface, interface)

#endif
