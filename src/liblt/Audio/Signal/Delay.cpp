#include "../Signals.h"

#include "LTE/Array.h"
#include "LTE/StdMath.h"

namespace Audio {
  namespace {
    struct Delay : public SignalT {
      Signal input;
      Array<double> buffer;
      double amp;
      double feedback;
      uint offset;

      Delay(
          Signal const& input,
          uint samples,
          double amp,
          double feedback) :
        input(input),
        buffer(samples),
        amp(amp),
        feedback(feedback),
        offset(0)
        {}

      double OnGet(GlobalData const& d) {
        double s = input->Get(d);
        if (d.sampleNum >= buffer.size())
          s += (amp / feedback) * buffer[offset];
        buffer[offset++] = feedback * s;
        offset %= buffer.size();
        return s;
      }
    };
  }

  Signal Signal_Delay(
    Signal const& input,
    uint samples,
    double amp,
    double feedback)
  {
    return new Delay(input, samples, amp, feedback);
  }
}
