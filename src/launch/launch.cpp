#include "Module/PhysicsEngine.h"
#include "Module/SoundEngine.h"

#include "LTE/Keyboard.h"
#include "LTE/Profiler.h"
#include "LTE/Program.h"
#include "LTE/OS.h"
#include "LTE/Renderer.h"
#include "LTE/Script.h"
#include "LTE/Texture2D.h"
#include "LTE/Window.h"

// #define TIME_LTSL_COMPILE

#ifdef TIME_LTSL_COMPILE
#include "LTE/Debug.h"
#include "LTE/Timer.h"
#endif

struct Launcher : public Program {
  String appName;
  ScriptFunction initialize;
  ScriptFunction update;
  Data instance;
  Module physicsEngine;
  Module soundEngine;

  Launcher(String const& appName) : appName(appName) {
    if (!OS_IsDir("resource"))
      OS_ChangeDir("../");
    window = Window_Create("App Launcher", V2U(1920, 1080), true, false);
    window->SetSync(false);
    Renderer_Initialize();
  }

  void OnInitialize() {
    Launch();
  }

  void Launch() {
    physicsEngine = nullptr;
    soundEngine = nullptr;
    physicsEngine = CreatePhysicsEngine();
    soundEngine = SoundEngine_Fmod();

    Script_ClearCache();

#ifdef TIME_LTSL_COMPILE
    Timer timer;
#endif
    ScriptFunction main = ScriptFunction_Load("App/" + appName + ":Main");
#ifdef TIME_LTSL_COMPILE
    dbg | "LTSL Compile Time: " | timer.GetElapsed() | endl;
#endif

    if (!main) {
      printf("ERROR: Launcher failed to load script %s\n", appName.c_str());
      deleted = true;
      return;
    }

    /* NOTE : We have to raw cast here due to issues with the usual
              .Convert<ScriptType>. Due to a major design flaw in the Type
              system, Type_Get<T> returns different static storage in the exe
              vs the dll, so type equality comparisons fail when they shouldn't.
              I am not going to fix this right now. */
    ScriptType app = *(ScriptType*)main->returnType->GetAux().data;
    initialize = app->GetFunction("Initialize");
    update = app->GetFunction("Update");
    instance.Construct(app->type);
    main->VoidCall(instance.data);

    if (initialize)
      initialize->VoidCall(0, instance);
    if (!update)
      deleted = true;
  }

  void OnUpdate() {
    if (Keyboard_Pressed(Key_F1))
      SaveScreenshot();
    if (Keyboard_Pressed(Key_F2))
      Profiler_Auto(1.0f);

    if (Keyboard_Pressed(Key_W) && Keyboard_Control()) {
      deleted = true;
      instance.Clear();
      return;
    }

    if (Keyboard_Down(Key_Tilde))
      debugprint;

    if (Keyboard_Pressed(Key_F5))
      Launch();

    if (update)
      update->VoidCall(0, instance);

    if (physicsEngine)
      physicsEngine->Update();
    if (soundEngine)
      soundEngine->Update();
  }

  void SaveScreenshot() {
    String prefix = OS_GetUserDataPath() + "screenshot/";
    OS_CreatePath(prefix);
    for (uint i = 0;; ++i) {
      String path = prefix + ToString(i) + ".png";
      if (!OS_FileExists(path)) {
        Texture_ScreenCapture()->SaveTo(path);
        return;
      }
    }
  }
};

int main(int argc, char const* argv[]) {
  if (argc != 2) {
    printf("ERROR: Launcher expects one argument (application name)\n");
    return 0;
  }

  Launcher(argv[1]).Execute();
  return 0;
}

#if 0
#ifdef LIBLT_WINDOWS
  #pragma comment(linker, "/SUBSYSTEM:windows")
  #include <windows.h>
  int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, PSTR cmd, int show) {
    char const* argv[] = { "program" };
    return main(0, argv);
  }
#endif
#endif
