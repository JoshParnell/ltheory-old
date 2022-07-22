#include "../Signals.h"

#include "LTE/StdMath.h"

namespace Audio {
  namespace {
    struct Compress : public SignalT {
      Signal input;
      double factor;

      Compress(Signal const& input, double factor) :
        input(input),
        factor(factor)
        {}

      double OnGet(GlobalData const& d) {
        double s = input->Get(d);
        return Sign(s) * (1.0 - Exp(-factor * Abs(s)));
      }
    };
  }

  Signal Signal_Compress(Signal const& input, double factor) {
    return new Compress(input, factor);
  }
}
