#include "../Signals.h"

#include "Audio/Generator.h"

#include "LTE/AutoPtr.h"
#include "LTE/StdMath.h"

namespace Audio {
  namespace {
    struct Instrument : public SignalT {
      Generator generator;
      Signal envelope;
      Pattern pattern;
      size_t offset;

      Instrument(
          Generator const& generator,
          Pattern const& pattern,
          Signal const& envelope) :
        generator(generator),
        envelope(envelope),
        pattern(pattern)
        {}

      double OnGet(GlobalData const& d) {
        double sum = 0;
        for (size_t i = 0; i < pattern.size(); ++i) {
          const Note& note = pattern[i];
          if (note.on <= d.sampleNum && d.sampleNum <= note.off) {
            uint tick = d.sampleNum - note.on;
            double lt = (double)tick / d.sampleRate;
            double signal = generator->Get(note, lt);
            if (envelope) {
              GlobalData ld = {tick, d.sampleRate};
              signal *= envelope->Get(ld);
            }
            sum += signal;
          }
        }
        return sum;
      }
    };
  }

  Signal Signal_Instrument(
    Generator const& generator,
    Pattern const& pattern,
    Signal const& envelope)
  {
    return new Instrument(generator, pattern, envelope);
  }
}
