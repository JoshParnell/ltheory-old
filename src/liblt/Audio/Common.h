#ifndef Audio_Common_h__
#define Audio_Common_h__

#include "LTE/Common.h"

namespace Audio {
  struct GeneratorT;
  struct Note;
  struct SignalT;

  typedef Reference<GeneratorT> Generator;
  typedef Reference<SignalT> Signal;
}

#endif
