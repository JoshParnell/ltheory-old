#include "../Buttons.h"

#include "LTE/AutoClass.h"
#include "LTE/Pool.h"
#include "LTE/StdMath.h"

namespace LTE {
  namespace {
    AutoClassDerived(ButtonOr, ButtonT,
      Button, a,
      Button, b)
      DERIVED_TYPE_EX(ButtonOr)
      POOLED_TYPE

      ButtonOr() {}

      bool Down() const {
        return a->Down() || b->Down();
      }

      float DownTime() const {
        return Min(a->DownTime(), b->DownTime());
      }

      bool Pressed() const {
        return a->Pressed() || b->Pressed();
      }

      bool Released() const {
        return a->Released() || b->Released();
      }

      String ToString() const {
        return a->ToString() + ", " + b->ToString();
      }
    };

    DERIVED_IMPLEMENT(ButtonOr)
  }

  Button Button_Or(Button const& a, Button const& b) {
    return new ButtonOr(a, b);
  }
}
