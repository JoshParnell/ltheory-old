#include "Signal.h"

#include "LTE/Array.h"
#include "LTE/AutoPtr.h"
#include "LTE/StdMath.h"

struct ASRImpl : public Audio::Signal {
  double a, s, r;

  ASRImpl(double a, double s, double r) :
    a(a),
    s(s),
    r(r)
    {}

  double OnGet(const Audio::GlobalData& d) {
    double t = (double)d.sampleNum / d.sampleRate;
    return Min(1., t * a) * Exp(-r * Max(0., t - s));
  }
};

namespace Audio {
  Signal* ASR(double a, double s, double r) {
    return new ASRImpl(a, s, r);
  }
}
