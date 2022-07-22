#include "../Buttons.h"

#include "LTE/AutoClass.h"
#include "LTE/Joystick.h"
#include "LTE/Pool.h"

namespace LTE {
  namespace {
    AutoClassDerived(ButtonJoystick, ButtonT,
      uint, joyIndex,
      uint, buttonIndex)
      DERIVED_TYPE_EX(ButtonJoystick)
      POOLED_TYPE

      ButtonJoystick() {}

      bool Down() const {
        Joystick* j = Joystick::Get(joyIndex);
        return j ? j->Down(buttonIndex) : false;
      }

      float DownTime() const {
        Joystick* j = Joystick::Get(joyIndex);
        return j ? j->DownTime(buttonIndex) : FLT_MAX;
      }

      bool Pressed() const {
        Joystick* j = Joystick::Get(joyIndex);
        return j ? j->Pressed(buttonIndex) : false;
      }

      bool Released() const {
        Joystick* j = Joystick::Get(joyIndex);
        return j ? j->Released(buttonIndex) : false;
      }

      String ToString() const {
        return Stringize() | "Joystick " | joyIndex | " - " | buttonIndex;
      }
    };

    DERIVED_IMPLEMENT(ButtonJoystick)
  }

  Button Button_Joy(uint joyIndex, uint buttonIndex) {
    return new ButtonJoystick(joyIndex, buttonIndex);
  }
}
