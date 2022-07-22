#include "Signal.h"

#include "LTE/AutoPtr.h"
#include "LTE/StdMath.h"

namespace Audio {
  namespace {
    struct ProductImpl : public SignalT {
      Signal a;
      Signal b;

      ProductImpl(
          Signal const& a,
          Signal const& b) :
        a(a),
        b(b)
        {}

      double OnGet(GlobalData const& d) {
        return a->Get(d) * b->Get(d);
      }
    };

    struct SumImpl : public SignalT {
      Signal a;
      Signal b;
      double mixA;
      double mixB;

      SumImpl(
          Signal const& a,
          Signal const& b,
          double mixA,
          double mixB) :
        a(a),
        b(b),
        mixA(mixA),
        mixB(mixB)
        {}

      double OnGet(GlobalData const& d) {
        return mixA * a->Get(d) + mixB * b->Get(d);
      }
    };
  }

  Signal Signal_Product(Signal const& a, Signal const& b) {
    return new ProductImpl(a, b);
  }

  Signal Signal_Sum(
    Signal const& a,
    Signal const& b,
    double mixA,
    double mixB)
  {
    return new SumImpl(a, b, mixA, mixB);
  }
}
