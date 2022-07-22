#include "../Signals.h"

#include "LTE/StdMath.h"

namespace Audio {
  namespace {
    struct Lowpass : public SignalT {
      Signal input;
      double s;

      Lowpass(Signal const& input) :
        input(input),
        s(0)
        {}

      double OnGet(GlobalData const& d) {
        return s = Mix(s, input->Get(d), Exp(-(d.sampleRate / 5000.0)));
      }
    };
  }

  Signal Signal_Lowpass(Signal const& input) {
    return new Lowpass(input);
  }
}
