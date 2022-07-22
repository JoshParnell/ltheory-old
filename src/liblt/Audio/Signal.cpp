#include "Signal.h"

#include "LTE/Array.h"
#include "LTE/StdMath.h"

namespace Audio {
  struct SignalImpl {
    uint cacheFrame;
    double cacheValue;

    SignalImpl() :
      cacheFrame(UINT_MAX),
      cacheValue(0)
      {}
  };

  SignalT::SignalT() :
    impl(new SignalImpl)
    {}

  SignalT::~SignalT() {
    delete impl;
  }

  double SignalT::Get(GlobalData const& d) {
    if (impl->cacheFrame == d.sampleNum)
      return impl->cacheValue;
    double s = OnGet(d);
    impl->cacheFrame = d.sampleNum;
    impl->cacheValue = s;
    return s;
  }

  Array<float>* Signal_Render(
    Signal const& signal,
    uint seconds,
    uint bpm,
    uint rate)
  {
    uint samples = seconds * rate;
    Array<double> sampleBuffer(samples);
    GlobalData d = {0, rate};

    double maxAmplitude = 1.0;
    for (size_t i = 0; i < samples; ++i) {
      sampleBuffer[i] = signal->Get(d);
      maxAmplitude = Max(maxAmplitude, Abs(sampleBuffer[i]));
      d.sampleNum++;
    }

    Array<float>* output = new Array<float>(samples);
    for (size_t i = 0; i < samples; ++i)
      (*output)[i] = (float)(sampleBuffer[i] / maxAmplitude);
    return output;
  }
}
