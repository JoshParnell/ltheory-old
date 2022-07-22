#include "../Buttons.h"

#include "LTE/AutoClass.h"
#include "LTE/Pool.h"
#include "LTE/StdMath.h"

namespace LTE {
  namespace {
    AutoClassDerived(ButtonAnd, ButtonT,
      Button, a,
      Button, b)
      DERIVED_TYPE_EX(ButtonAnd)
      POOLED_TYPE

      ButtonAnd() {}

      bool Down() const {
        return a->Down() && b->Down();
      }

      float DownTime() const {
        return Min(a->DownTime(), b->DownTime());
      }

      bool Pressed() const {
        return 
          (a->Down() && b->Pressed()) ||
          (b->Down() && a->Pressed());
      }

      bool Released() const {
        return
          (a->Down() && b->Released()) ||
          (b->Down() && a->Released());
      }

      String ToString() const {
        return Stringize() | a->ToString() | " & " | b->ToString();
      }
    };

    DERIVED_IMPLEMENT(ButtonAnd)
  }

  Button Button_And(Button const& a, Button const& b) {
    return new ButtonAnd(a, b);
  }
}
