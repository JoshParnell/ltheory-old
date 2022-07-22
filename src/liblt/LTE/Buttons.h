#ifndef LTE_Buttons_h__
#define LTE_Buttons_h__

#include "Button.h"

namespace LTE {
  LT_API Button Button_And(Button const& a, Button const& b);

  LT_API Button Button_Axis(Axis const& axis, float sign);

  LT_API Button Button_Capture();

  LT_API Button Button_Inverted(Button const& button);

  LT_API Button Button_Joy(uint joyIndex, uint buttonIndex);

  LT_API Button Button_Key(Key key);

  LT_API Button Button_Mouse(MouseButton mouseButton);

  LT_API Button Button_Or(Button const& a, Button const& b);

  inline Button Button_AndAlt(Button const& button) {
    return Button_And(button,
      Button_Or(
        Button_Key(Key_LAlt),
        Button_Key(Key_RAlt)));
  }

  inline Button Button_AndControl(Button const& button) {
    return Button_And(button,
      Button_Or(
        Button_Key(Key_LControl),
        Button_Key(Key_RControl)));
  }

  inline Button Button_AndShift(Button const& button) {
    return Button_And(button,
      Button_Or(
        Button_Key(Key_LShift),
        Button_Key(Key_RShift)));
  }

  inline Button Button_LeftStick() {
#ifdef LIBLT_WINDOWS
    return Button_Joy(0, 8);
#else
    return Button_Joy(0, 9);
#endif
  }

  inline Button Button_RightStick() {
#ifdef LIBLT_WINDOWS
    return Button_Joy(0, 9);
#else
    return Button_Joy(0, 10);
#endif
  }
}

#endif
