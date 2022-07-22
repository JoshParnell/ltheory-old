#include "Mouse.h"
#include "Timer.h"
#include "Window.h"

#include "Module/FrameTimer.h"

#include "SFML/Graphics.hpp"

const float kDoubleClickThresh = 0.1f;

namespace {
  struct Mouse {
    int x;
    int y;
    int lastX;
    int lastY;
    float lastClickInterval;
    float scrollDelta;
    float idleTime;

    Timer downTimer[MouseButton_SIZE];
    bool down[MouseButton_SIZE];
    bool lastDown[MouseButton_SIZE];

    Timer releaseTimer[MouseButton_SIZE];

    Mouse() :
      x(0),
      y(0),
      lastX(0),
      lastY(0),
      scrollDelta(0),
      idleTime(0)
    {
      for (MouseButton i = 0; i < MouseButton_SIZE; ++i) {
        down[i] = false;
        lastDown[i] = false;
      }
    }
  } gMouse;
}

namespace LTE {
  DefineFunction(Mouse_DoubleClicked) {
    return Mouse_LeftPressed()
      && gMouse.releaseTimer[MouseButton_Left].GetElapsed() < kDoubleClickThresh;
  }

  bool Mouse_Down(MouseButton button) {
    return gMouse.down[button];
  }

  bool Mouse_Pressed(MouseButton button) {
    return gMouse.down[button] && !gMouse.lastDown[button];
  }

  bool Mouse_Released(MouseButton button) {
    return !gMouse.down[button] && gMouse.lastDown[button];
  }

  float Mouse_GetDownTime(MouseButton button) {
    return gMouse.downTimer[button].GetElapsed();
  }

  DefineFunction(Mouse_GetScrollDelta) {
    return gMouse.scrollDelta;
  }

  DefineFunction(Mouse_GetDP) {
    return V2(
      (float)(gMouse.x - gMouse.lastX),
      (float)(gMouse.y - gMouse.lastY));
  }

  DefineFunction(Mouse_GetIdleTime) {
    return gMouse.idleTime;
  }

  DefineFunction(Mouse_GetDX) {
    return gMouse.x - gMouse.lastX;
  }

  DefineFunction(Mouse_GetDY) {
    return gMouse.y - gMouse.lastY;
  }

  DefineFunction(Mouse_GetPos) {
    return V2((float)gMouse.x, (float)gMouse.y);
  }

  DefineFunction(Mouse_GetPosImmediate) {
    sf::Vector2i p = sf::Mouse::getPosition(
      *(sf::RenderWindow*)Window_Get()->GetImplData());
    return V2((float)p.x, (float)p.y);
  }

  DefineFunction(Mouse_GetPosLast) {
    return V2((float)gMouse.lastX, (float)gMouse.lastY);
  }

  DefineFunction(Mouse_GetX) {
    return gMouse.x;
  }

  DefineFunction(Mouse_GetY) {
    return gMouse.y;
  }

  void Mouse_SetPos(V2 const& v) {
    gMouse.x = (int)v.x;
    gMouse.y = (int)v.y;
    sf::Mouse::setPosition(
      sf::Vector2i(gMouse.x, gMouse.y),
      *(sf::RenderWindow*)Window_Get()->GetImplData());
  }

  void Mouse_SetPressed(MouseButton button, bool pressed) {
    gMouse.down[button] = pressed;
  }

  void Mouse_SetScrollDelta(float ds) {
    gMouse.scrollDelta = ds;
  }

  void Mouse_Update() {
    gMouse.idleTime += FrameTimer_Get();
    if (gMouse.lastX != gMouse.x || gMouse.lastY != gMouse.y)
      gMouse.idleTime = 0;
    gMouse.lastX = gMouse.x;
    gMouse.lastY = gMouse.y;

    for (MouseButton button = 0; button < MouseButton_SIZE; ++button) {
      if (gMouse.down[button] && !gMouse.lastDown[button])
        gMouse.downTimer[button].Reset();
      if (!gMouse.down[button] && gMouse.lastDown[button])
        gMouse.releaseTimer[button].Reset();
      gMouse.lastDown[button] = gMouse.down[button];
    }

    gMouse.scrollDelta = 0;
  }

  void Mouse_UpdatePos(V2I const& p) {
    gMouse.x = p.x;
    gMouse.y = p.y;
  }

  DefineFunction(Mouse_LeftDown) {
    return Mouse_Down(MouseButton_Left);
  }

  DefineFunction(Mouse_LeftPressed) {
    return Mouse_Pressed(MouseButton_Left);
  }

  DefineFunction(Mouse_LeftReleased) {
    return Mouse_Released(MouseButton_Left);
  }

  DefineFunction(Mouse_RightDown) {
    return Mouse_Down(MouseButton_Right);
  }


  DefineFunction(Mouse_RightPressed) {
    return Mouse_Pressed(MouseButton_Right);
  }

  DefineFunction(Mouse_RightReleased) {
    return Mouse_Released(MouseButton_Right);
  }
}
