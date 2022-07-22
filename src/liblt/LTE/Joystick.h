#ifndef LTE_Joystick_h__
#define LTE_Joystick_h__

#include "Enum.h"

namespace LTE {
  struct Joystick {
    virtual ~Joystick() {}

    LT_API static Joystick* Get(unsigned int index);
    LT_API static uint GetButtonCount();
    LT_API static uint GetCount();

    virtual float GetAxis(JoystickAxis axis) const = 0;
    virtual float GetAxisDelta(JoystickAxis axis) const = 0;
    virtual bool Down(uint buttonIndex) const = 0;
    virtual float DownTime(uint buttonIndex) const = 0;
    virtual bool Pressed(uint buttonIndex) const = 0;
    virtual bool Released(uint buttonIndex) const = 0;
    virtual void Update() = 0;
  };
}

#endif
