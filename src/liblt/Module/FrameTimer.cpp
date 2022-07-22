#include "FrameTimer.h"

#include "LTE/Module.h"
#include "LTE/Timer.h"

namespace {
  struct FrameTimerModule : public ModuleT {
    Timer timer;
    float dt;
    float ema01;
    float ema1;
    float ema10;

    FrameTimerModule() :
      dt(0),
      ema01(0),
      ema1(0),
      ema10(0)
      {}

    char const* GetName() const {
      return "Frame Timer";
    }

    void Update() {
      dt = timer.GetElapsed();
      ema01 = Mix(ema01, dt, 1.0 - Exp(-dt / 0.5f));
      ema1 = Mix(ema1, dt, 1.0 - Exp(-dt));
      ema10 = Mix(ema10, dt, 1.0 - Exp(-dt / 10.0f));
      timer.Reset();
    }
  };

  FrameTimerModule* FrameTimer_GetModule() {
    static Reference<FrameTimerModule> module;
    if (!module) {
      module = new FrameTimerModule;
      Module_RegisterGlobal(module);
    }
    return module;
  }
}

DefineFunction(FrameTimer_Get) {
  return FrameTimer_GetModule()->dt;
}

DefineFunction(FrameTimer_GetEMA01) {
  return FrameTimer_GetModule()->ema01;
}

DefineFunction(FrameTimer_GetEMA1) {
  return FrameTimer_GetModule()->ema1;
}

DefineFunction(FrameTimer_GetEMA10) {
  return FrameTimer_GetModule()->ema10;
}
