#ifndef LTE_Axis_h__
#define LTE_Axis_h__

#include "BaseType.h"
#include "Joystick.h"
#include "Reference.h"
#include "String.h"

namespace LTE {
  struct AxisT : public RefCounted {
    BASE_TYPE(AxisT)
    float zeroValue;
    float maxValue;
    float deadZone;
    float power;
    bool invert;

    AxisT() :
      zeroValue(0),
      maxValue(1),
      deadZone(0),
      power(1),
      invert(false)
      {}

    LT_API float Get() const;

    virtual float GetRaw() const = 0;

    Axis Invert() {
      invert = !invert;
      return this;
    }

    Axis SetDeadZone(float deadZone) {
      this->deadZone = deadZone;
      return this;
    }

    Axis SetPower(float power) {
      this->power = power;
      return this;
    }

    Axis SetZeroValue(float zeroValue) {
      this->zeroValue = zeroValue;
      return this;
    }

    FIELDS {
      MAPFIELD(zeroValue)
      MAPFIELD(maxValue)
      MAPFIELD(deadZone)
      MAPFIELD(power)
      MAPFIELD(invert)
    }
  };

  LT_API Axis Axis_Button(Button const& button, bool inverted = false);

  LT_API Axis Axis_Capture();

  LT_API Axis Axis_Joy(uint joystickIndex, JoystickAxis axix);

  LT_API Axis Axis_MouseX();
  LT_API Axis Axis_MouseY();

  LT_API Axis Axis_MouseDragX(MouseButton button);
  LT_API Axis Axis_MouseDragY(MouseButton button);

  LT_API Axis Axis_MouseWheel();

  LT_API Axis Axis_Product(Axis const& a, Axis const& b);

  LT_API Axis Axis_Range(Axis const& source, float minValue, float maxValue);

  LT_API Axis Axis_Sum(Axis const& a, Axis const& b);

  inline Axis Axis_DpadVertical() {
#ifdef LIBLT_WINDOWS
    return Axis_Joy(0, JoystickAxis_PovX)->Invert();
#else
    return Axis_Joy(0, JoystickAxis_PovY);
#endif
  }

  inline Axis Axis_DpadHorizontal() {
#ifdef LIBLT_WINDOWS
    return Axis_Joy(0, JoystickAxis_PovY)->Invert();
#else
    return Axis_Joy(0, JoystickAxis_PovX);
#endif
  }

  inline Axis Axis_LeftStickX() {
    return Axis_Joy(0, JoystickAxis_X);
  }

  inline Axis Axis_LeftStickY() {
    return Axis_Joy(0, JoystickAxis_Y);
  }

  inline Axis Axis_LeftTrigger() {
#ifdef LIBLT_WINDOWS
    return Axis_Range(Axis_Joy(0, JoystickAxis_Z)->SetDeadZone(.2f), 0.f, 1.f);
#else
    return Axis_Joy(0, JoystickAxis_Z)->SetZeroValue(-1)->SetDeadZone(.2f);
#endif
  }

  inline Axis Axis_RightStickX() {
    return Axis_Joy(0, JoystickAxis_U);
  }

  inline Axis Axis_RightStickY() {
#ifdef LIBLT_WINDOWS
    return Axis_Joy(0, JoystickAxis_R);
#else
    return Axis_Joy(0, JoystickAxis_V);
#endif
  }

  inline Axis Axis_RightTrigger() {
#ifdef LIBLT_WINDOWS
    return Axis_Range(Axis_Joy(0, JoystickAxis_Z)->SetDeadZone(.2f), 0.f, -1.f);
#else
    return Axis_Joy(0, JoystickAxis_R)->SetZeroValue(-1)->SetDeadZone(.2f);
#endif
  }
}

#endif
