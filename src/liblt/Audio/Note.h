#ifndef Audio_Note_h__
#define Audio_Note_h__

#include "Common.h"
#include "LTE/Vector.h"

namespace Audio {
  struct Note {
    uint on, off;
    double frequency;
    uint duration;
    float velocity;

    Note() {}

    Note(uint time, double frequency, uint duration, float velocity = 1) :
      on(time),
      off(time + duration),
      frequency(frequency),
      velocity(velocity)
      {}

    friend bool operator<(Note const& a, Note const& b) {
      return a.on < b.on;
    }
  };

  typedef Vector<Note> Pattern;
}

#endif
