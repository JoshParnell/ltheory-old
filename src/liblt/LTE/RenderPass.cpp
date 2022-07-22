#include "RenderPass.h"

#include "Profiler.h"
#include "Renderer.h"
#include "StackFrame.h"

#include "Module/Settings.h"

void RenderPassT::Render(DrawState* state) {
  if (Settings_Bool((String)"Graphics/" + ToString(), true)()) {
    FRAME(GetName()) {
      OnRender(state);
      Profiler_Flush();
    }
  }
}
