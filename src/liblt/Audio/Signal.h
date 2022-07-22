#ifndef Audio_Signal_h__
#define Audio_Signal_h__

#include "Common.h"
#include "LTE/Reference.h"

namespace Audio {
  struct GlobalData {
    uint sampleNum;
    uint sampleRate;
  };

  struct SignalT : public RefCounted {
    struct SignalImpl* impl;

    LT_API SignalT();
    LT_API virtual ~SignalT();

    virtual double OnGet(GlobalData const& d) = 0;

    LT_API double Get(GlobalData const& d);
  };

  LT_API Array<float>* Signal_Render(
    Signal const& signal,
    uint seconds,
    uint bpm = 120,
    uint rate = 44100);
}

#endif
