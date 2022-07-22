#include "../Buttons.h"

#include "LTE/AutoClass.h"
#include "LTE/Keyboard.h"
#include "LTE/Pool.h"

namespace LTE {
  namespace {
    AutoClassDerived(ButtonKey, ButtonT,
      Key, key)
      DERIVED_TYPE_EX(ButtonKey)
      POOLED_TYPE

      ButtonKey() {}

      bool Down() const {
        return Keyboard_Down(key);
      }

      float DownTime() const {
        NOT_IMPLEMENTED
        return 0;
      }

      bool Pressed() const {
        return Keyboard_Pressed(key);
      }

      bool Released() const {
        return Keyboard_Released(key);
      }

      String ToString() const {
        return Key_Name(key);
      }
    };

    DERIVED_IMPLEMENT(ButtonKey)
  }

  Button Button_Key(Key key) {
    return new ButtonKey(key);
  }
}
