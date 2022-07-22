#ifndef LTE_StackArray_h__
#define LTE_StackArray_h__

#define DEBUG_BOUNDS_CHECK

#include "Common.h"

namespace LTE {
  template <class T, size_t capacity>
  struct StackArray {
    T stack[capacity];

    StackArray() {}

    StackArray(T const & t) {
      for (size_t i = 0; i < capacity; ++i)
        stack[i] = t;
    }

    T& operator[](size_t index) {
#ifdef DEBUG_BOUNDS_CHECK
      LTE_ASSERT(index < capacity);
#endif
      return stack[index];
    }

    T const& operator[](size_t index) const {
#ifdef DEBUG_BOUNDS_CHECK
      LTE_ASSERT(index < capacity);
#endif
      return stack[index];
    }

    T* data() {
      return stack;
    }

    T const* data() const {
      return stack;
    }

    size_t size() const {
      return capacity;
    }

    template <class StreamT>
    friend void _ToStream(StreamT& s, StackArray const& self) {
      s << '[';
      for (size_t i = 0; i < capacity; ++i) {
        if (i)
          s << ", ";
        ToStream(s, self[i]);
      }
      s << ']';
    }
  };
}

#endif
