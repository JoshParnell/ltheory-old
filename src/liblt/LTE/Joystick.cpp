#include "Joystick.h"
#include "Pointer.h"
#include "Timer.h"

#include "SFML/Window.hpp"

namespace LTE {
  struct JoystickImpl : public Joystick {
    uint id;
    Timer downTimer[sf::Joystick::ButtonCount];
    bool down[sf::Joystick::ButtonCount];
    bool lastDown[sf::Joystick::ButtonCount];
    float lastAxis[JoystickAxis_SIZE];

    JoystickImpl(uint id) :
      id(id)
    {
      for (uint i = 0; i < sf::Joystick::ButtonCount; ++i)
        down[i] = false;
      for (uint i = 0; i < sf::Joystick::ButtonCount; ++i)
        lastDown[i] = false;
      for (JoystickAxis axis = 0; axis < JoystickAxis_SIZE; ++axis)
        lastAxis[axis] = GetAxis(axis);
    }

    float GetAxis(JoystickAxis axis) const {
      return sf::Joystick::getAxisPosition(id, (sf::Joystick::Axis)axis) / 100.f;
    }

    float GetAxisDelta(JoystickAxis axis) const {
      return GetAxis(axis) - lastAxis[axis];
    }

    bool Down(uint index) const {
      return down[index];
    }

    float DownTime(uint index) const {
      return downTimer[index].GetElapsed();
    }

    bool Pressed(uint index) const {
      return down[index] && !lastDown[index];
    }

    bool Released(uint index) const {
      return !down[index] && lastDown[index];
    }

    void Update() {
      for (uint i = 0; i < sf::Joystick::ButtonCount; ++i) {
        lastDown[i] = down[i];
        down[i] = sf::Joystick::isButtonPressed(id, i);
        if (Pressed(i))
          downTimer[i].Reset();
      }

      for (JoystickAxis axis = 0; axis < JoystickAxis_SIZE; ++axis)
        lastAxis[axis] = GetAxis(axis);
    }
  };

  Joystick* Joystick::Get(uint index) {
    static Pointer<Joystick> sticks[sf::Joystick::Count];
    if (index >= sf::Joystick::Count)
      return nullptr;
    else if (sf::Joystick::isConnected(index))
      return sticks[index] ? sticks[index] :
                             sticks[index] = new JoystickImpl(index);
    else
      return nullptr;
  }

  uint Joystick::GetButtonCount() {
    return sf::Joystick::ButtonCount;
  }

  uint Joystick::GetCount() {
    return sf::Joystick::Count;
  }
}
