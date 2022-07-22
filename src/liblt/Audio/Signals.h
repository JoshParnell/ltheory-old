#ifndef Audio_Signals_h__
#define Audio_Signals_h__

#include "Signal.h"
#include "Note.h"

namespace Audio {

  LT_API Signal Signal_ASR(double a, double s, double r);

  LT_API Signal Signal_Compress(Signal const& input, double factor);

  LT_API Signal Signal_Delay(
    Signal const& input,
    uint ticks,
    double amp,
    double feedback);

  LT_API Signal Signal_Instrument(
    Generator const& generator,
    Pattern const& pattern,
    Signal const& envelope = nullptr);

  LT_API Signal Signal_Lowpass(Signal const& input);

  LT_API Signal Signal_Product(Signal const& a, Signal const& b);

  LT_API Signal Signal_Sum(
    Signal const& a,
    Signal const& b,
    double mixA = 1,
    double mixB = 1);

}

#endif
