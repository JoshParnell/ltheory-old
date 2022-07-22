#ifndef LTE_Enum_h__
#define LTE_Enum_h__

#include "Common.h"

template <class T, char const* (*GetString)(int)>
struct Enum {
  int value;

  Enum() :
    value(-1)
    {}

  Enum(T t) :
    value((int)t)
    {}

  Enum(int i) :
    value(i)
    {}

  operator int() const {
    return value;
  }

  Enum& operator=(int const& i) {
    value = i;
    return *this;
  }

  template <class StreamT>
  friend void _ToStream(StreamT& s, Enum const& e) {
    s << (e.value >= 0 ? GetString(e.value) : "null");
  }
};

#endif
