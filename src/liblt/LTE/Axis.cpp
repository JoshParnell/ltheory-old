#include "Axis.h"
#include "AutoPtr.h"
#include "Button.h"
#include "Joystick.h"
#include "Math.h"
#include "Mouse.h"
#include "Pool.h"
#include "Vector.h"
#include "Window.h"

const float kDragTime = 0.0f;
const float kMouseWheelSensitivity = 1.5f;

namespace {
  AutoClassDerived(AxisButton, AxisT,
    Button, button,
    bool, invert)
    DERIVED_TYPE_EX(AxisButton)
    POOLED_TYPE

    AxisButton() {}

    float GetRaw() const {
      return invert
        ? (button->Down() ? 0 : 1)
        : (button->Down() ? 1 : 0);
    }

    String ToString() const {
      return button->ToString();
    }
  };

  DERIVED_IMPLEMENT(AxisButton)

  AutoClassDerived(AxisJoystick, AxisT,
    uint, joyIndex,
    JoystickAxis, axis)
    DERIVED_TYPE_EX(AxisJoystick)
    POOLED_TYPE

    AxisJoystick() {}

    float GetRaw() const {
      Joystick* j = Joystick::Get(joyIndex);
      return j ? j->GetAxis(axis) : zeroValue;
    }

    String ToString() const {
      return Stringize() | "Joy " | joyIndex | " " | JoystickAxis_String[axis];
    }
  };

  DERIVED_IMPLEMENT(AxisJoystick)

  AutoClassDerived(AxisMouse, AxisT,
    bool, vertical)
    DERIVED_TYPE_EX(AxisMouse)
    POOLED_TYPE

    AxisMouse() {}

    float GetRaw() const {
      V2 pos = Mouse_GetPos();
      V2 size = Window_Get()->GetSize();
      float div = size.GetMin();
      return Clamp(
        2.f * (pos[vertical ? 1 : 0] - size[vertical ? 1 : 0] * .5f) / div,
        -1.f, 1.f);
    }

    String ToString() const {
      return vertical ? "Mouse X" : "Mouse Y";
    }
  };

  DERIVED_IMPLEMENT(AxisMouse)

  AutoClassDerived(AxisMouseDrag, AxisT,
    bool, vertical,
    MouseButton, button)
    DERIVED_TYPE_EX(AxisMouseDrag)
    POOLED_TYPE

    AxisMouseDrag() {}

    float GetRaw() const {
      if (Mouse_Down(button) && Mouse_GetDownTime(button) > kDragTime)
        return Mouse_GetDP()[vertical ? 1 : 0] / 8.f;
      else
        return 0.;
    }

    String ToString() const {
      return vertical ? "Mouse Drag X" : "Mouse Drag Y";
    }
  };

  DERIVED_IMPLEMENT(AxisMouseDrag)

  AutoClassDerived(AxisRange, AxisT,
    Axis, source,
    float, minValue,
    float, maxValue)
    DERIVED_TYPE_EX(AxisRange)
    POOLED_TYPE

    AxisRange() {}

    float GetRaw() const {
      return Saturate((source->Get() - minValue) / (maxValue - minValue));
    }

    String ToString() const {
      return source->ToString();
    }
  };

  DERIVED_IMPLEMENT(AxisRange)

  AutoClassDerived(AxisProduct, AxisT,
    Axis, a,
    Axis, b)
    DERIVED_TYPE_EX(AxisProduct)
    POOLED_TYPE

    AxisProduct() {}

    float GetRaw() const {
      return a->Get() * b->Get();
    }

    String ToString() const {
      return a->ToString() + " * " + b->ToString();
    }
  };

  DERIVED_IMPLEMENT(AxisProduct)

  AutoClassDerived(AxisSum, AxisT,
    Axis, a,
    Axis, b)
    DERIVED_TYPE_EX(AxisSum)
    POOLED_TYPE

    AxisSum() {}

    float GetRaw() const {
      return a->Get() + b->Get();
    }

    String ToString() const {
      return a->ToString() + " + " + b->ToString();
    }
  };

  DERIVED_IMPLEMENT(AxisSum)

  AutoClassDerivedEmpty(AxisMouseWheel, AxisT)
    DERIVED_TYPE_EX(AxisMouseWheel)
    POOLED_TYPE

    AxisMouseWheel() {}

    float GetRaw() const {
      return -kMouseWheelSensitivity * Mouse_GetScrollDelta();
    }

    String ToString() const {
      return "Mouse Wheel";
    }
  };

  DERIVED_IMPLEMENT(AxisMouseWheel)
}

namespace LTE {
  float AxisT::Get() const {
    float x = (GetRaw() - zeroValue) / (maxValue - zeroValue);
    float mag = Abs(x);
    mag = Pow(Max(0.f, mag - deadZone) / (1 - deadZone), power);
    return (invert ? -1.f : 1.f) * Sign(x) * mag;
  }

  Axis Axis_Button(Button const& button, bool inverted) {
    return new AxisButton(button, inverted);
  }

  Axis Axis_Capture() {
    if (Abs(Mouse_GetDX()) > 1)
      return Axis_MouseX();
    if (Abs(Mouse_GetDY()) > 1)
      return Axis_MouseY();

    for (uint i = 0; i < Joystick::GetCount(); ++i) {
      Joystick* joy = Joystick::Get(i);

      for (JoystickAxis axis = 0; axis < JoystickAxis_SIZE; ++axis) {
        if (Abs(joy->GetAxisDelta(axis)) > .1f)
          return Axis_Joy(i, axis);
      }
    }

    return nullptr;
  }

  Axis Axis_Joy(uint joystickIndex, JoystickAxis axis) {
    return new AxisJoystick(joystickIndex, axis);
  }

  Axis Axis_MouseX() {
    return new AxisMouse(false);
  }

  Axis Axis_MouseY() {
    return new AxisMouse(true);
  }

  Axis Axis_MouseDragX(MouseButton button) {
    return new AxisMouseDrag(false, button);
  }

  Axis Axis_MouseDragY(MouseButton button) {
    return new AxisMouseDrag(true, button);
  }

  Axis Axis_MouseWheel() {
    return new AxisMouseWheel;
  }

  Axis Axis_Range(Axis const& source, float minValue, float maxValue) {
    return new AxisRange(source, minValue, maxValue);
  }

  Axis Axis_Product(Axis const& a, Axis const& b) {
    return new AxisProduct(a, b);
  }

  Axis Axis_Sum(Axis const& a, Axis const& b) {
    return new AxisSum(a, b);
  }
}
