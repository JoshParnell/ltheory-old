#include "Program.h"

#include "Joystick.h"
#include "GL.h"
#include "Keyboard.h"
#include "Module.h"
#include "Mouse.h"
#include "OS.h"
#include "StackFrame.h"
#include "Window.h"

#include <ctime>

namespace  {
  bool destructed = false;
}

Program::Program() : deleted(false) {
  srand((uint)time(0));
  OS_ConfigureSignalHandlers();
}

Program::~Program() {
  AUTO_FRAME;
  destructed = true;
}

void Program::Delete() {
  deleted = true;
}

void Program::Execute() {
  FRAME("Initialize") {
    Window_Push(window);
    OnInitialize();
    Window_Pop();
  }

  while (window->IsOpen()) {
    if (deleted)
      break;
    Window_Push(window);

    FRAME("InputUpdate") {
      Mouse_Update();
      Keyboard_Update(window->HasFocus());

      if (window->HasFocus())
        for (uint i = 0; i < Joystick::GetCount(); ++i)
          if (Joystick::Get(i))
            Joystick::Get(i)->Update();
    }

    FRAME("WindowUpdate") {
      Window_Pop();
      window->Update();
      Window_Push(window);
    }

    OnUpdate();

    Module_UpdateGlobal();

    FRAME("Display")
      window->Display();
    Window_Pop();
  }
}

/* TODO : Fix this ugly mess. */
bool Program_InStaticSection() {
  return destructed;
}
