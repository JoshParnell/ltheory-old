#ifndef Audio_Generator_h__
#define Audio_Generator_h__

#include "Note.h"
#include "LTE/Reference.h"

namespace Audio {
  struct GeneratorT : public RefCounted {
    virtual ~GeneratorT() {}
    virtual double Get(Note const& note, double t) const = 0;
  };
}

#endif
