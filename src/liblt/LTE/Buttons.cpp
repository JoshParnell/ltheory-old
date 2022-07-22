#include "Buttons.h"

#include "AutoPtr.h"
#include "Axis.h"
#include "Joystick.h"
#include "Keyboard.h"
#include "Math.h"
#include "Mouse.h"
#include "Pool.h"
#include "Vector.h"

namespace {
  AutoClassDerived(AxisButton, ButtonT,
    Axis, axis,
    float, sgn)
    DERIVED_TYPE_EX(AxisButton)
    POOLED_TYPE

    AxisButton() {}

    bool Down() const {
      float x = axis->GetRaw();
      return Abs(x) > .2f && Sign(x) == sgn;
    }

    float DownTime() const {
      NOT_IMPLEMENTED
      return 0;
    }

    bool Pressed() const {
      return Down();
    }

    bool Released() const {
      return Down();
    }

    String ToString() const {
      return axis->ToString() + (sgn > 0 ? " +" : " -");
    }
  };

  DERIVED_IMPLEMENT(AxisButton)

  AutoClassDerived(KeyWithModifiers, ButtonT,
    Key, key,
    bool, shift,
    bool, alt,
    bool, control)
    DERIVED_TYPE_EX(KeyWithModifiers)
    POOLED_TYPE

    KeyWithModifiers() {}

    bool Modifiers() const {
      return (!shift || Keyboard_Shift())
          && (!alt || Keyboard_Alt())
          && (!control || Keyboard_Control());
    }

    bool Down() const {
      return Modifiers() && Keyboard_Down(key);
    }

    float DownTime() const {
      NOT_IMPLEMENTED
      return 0;
    }

    bool Pressed() const {
      return Modifiers() && Keyboard_Pressed(key);
    }

    bool Released() const {
      return Modifiers() && Keyboard_Released(key);
    }

    String ToString() const {
      String str;
      if (control)
        str += "Ctrl-";
      if (shift)
        str += "Shift-";
      if (alt)
        str += "Alt-";
      str += Key_Name(key);
      return str;
    };
  };

  DERIVED_IMPLEMENT(KeyWithModifiers)
}

namespace LTE {
  Button Button_Axis(Axis const& axis, float sign) {
    return new AxisButton(axis, sign);
  }

  Button Button_Capture() {
    Vector<Key> const& pressed = Keyboard_GetKeysPressed();
    if (pressed.size()) {
      bool shift = Keyboard_Shift();
      bool alt = Keyboard_Alt();
      bool ctrl = Keyboard_Control();

      Key key = pressed[0];

      if (key == Key_LAlt ||
          key == Key_LControl ||
          key == Key_LShift ||
          key == Key_RAlt ||
          key == Key_RControl ||
          key == Key_RShift)
        return Button_Key(key);

      if (shift || alt || ctrl) {
        /* CRITICAL : Key with modifiers. */
        NOT_IMPLEMENTED
        return nullptr;
      } else
        return Button_Key(key);
    }

    /* Try mouse buttons. */
    for (MouseButton button = 0; button < MouseButton_SIZE; ++button)
      if (Mouse_Pressed(button))
        return Button_Mouse(button);

    /* Try joystick buttons. */
    for (uint i = 0; i < Joystick::GetCount(); ++i) {
      Joystick* joy = Joystick::Get(i);
      if (!joy)
        continue;

      for (uint j = 0; j < Joystick::GetButtonCount(); ++j)
        if (joy->Down(j))
          return Button_Joy(i, j);

      /* As a last resort, try the joystick axes. We will allow these to be
         used as buttons, especially since a gamepad trigger is considered to
         be an axis. */
      for (JoystickAxis axis = 0; axis < JoystickAxis_SIZE; ++axis) {
        float delta = joy->GetAxisDelta(axis);
        float sgn = Sign(joy->GetAxis(axis));
        if (Abs(delta) > .1f && sgn == Sign(delta))
          return Button_Axis(Axis_Joy(i, axis), sgn);
      }
    }
    return nullptr;
  }
}
