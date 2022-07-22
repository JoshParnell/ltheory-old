#include "Interface.h"
#include "ClipRegion.h"
#include "Cursor.h"
#include "Widget.h"

#include "LTE/Buttons.h"
#include "LTE/DrawState.h"
#include "LTE/Math.h"
#include "LTE/Module.h"
#include "LTE/Mouse.h"
#include "LTE/Renderer.h"
#include "LTE/RenderPass.h"
#include "LTE/Shader.h"
#include "LTE/StackFrame.h"
#include "LTE/Window.h"

namespace {
  struct InterfaceImpl : public InterfaceT {
    String name;
    Vector<Widget> widgets;

    InterfaceImpl(String const& name) : name(name) {}

    ~InterfaceImpl() {
      Clear();
    }

    void Add(Widget const& widget) {
      widgets.push(widget);
      widget->Create();
    }

    void Clear() {
      for (size_t i = 0; i < widgets.size(); ++i)
        widgets[i]->Clear();
      widgets.clear();
    }

    void Draw() {
      SFRAME("InterfaceDraw");
      V2 windowSize = Window_Get()->GetSize();
      Cursor_Push(Mouse_GetPosImmediate(), Mouse_GetPosLast());
      ClipRegion_Push(0, windowSize);

      for (size_t i = 0; i < widgets.size(); ++i)
        widgets[i]->Draw();

      ClipRegion_Pop();
      Cursor_Pop();
    }

    void Update() {
      SFRAME("InterfaceUpdate");
      V2 windowSize = Window_Get()->GetSize();
      Cursor_Push(Mouse_GetPos(), Mouse_GetPosLast());
      ClipRegion_Push(0, windowSize);

      WidgetUpdateState state;
      for (int i = 0; i < (int)widgets.size(); ++i) {
        int index = widgets.size() - (i + 1);
        Widget const& widget = widgets[index];
        if (widget->deleted) {
          widget->Clear();
          widgets.eraseIndex(index);
          i--;
          continue;
        }

        widget->PrePosition();
        widget->pos = 0;
        widget->size = windowSize;
        widget->maxSize = widget->size;
        widget->PostPosition();
        widget->Update(state);
      }

      ClipRegion_Pop();
      Cursor_Pop();
    }
  };

  struct RenderPassInterface : public RenderPassT {
    Interface interface;
    Shader compositor;
    DERIVED_TYPE_EX(RenderPassInterface)

    RenderPassInterface() {}

    RenderPassInterface(Interface const& interface) :
      interface(interface),
      compositor(Shader_Create("identity.jsl", "ui/composite.jsl"))
      {}

    char const* GetName() const {
      return "Interface";
    }

    void OnRender(DrawState* state) {
      /* Draw interface to tertiary buffer. */ {
        state->tertiary->Bind(0);
        Renderer_Clear(0);
        interface->Draw();
        state->tertiary->Unbind();
      }

      /* Final composite. */ {
        DrawState_Link(compositor);
        RendererState s(BlendMode::Disabled, CullMode::Backface, false, false);
        state->secondary->Bind(0);
        (*compositor)
          ("source", state->primary)
          ("layer", state->tertiary);
        Renderer_SetShader(*compositor);
        Renderer_DrawFSQ();
        state->secondary->Unbind();
        state->Flip();
      }
    }
  };
}

DefineFunction(Interface_Create) {
  return new InterfaceImpl(args.name);
}

DefineFunction(RenderPass_Interface) {
  return new RenderPassInterface(args.interface);
}
