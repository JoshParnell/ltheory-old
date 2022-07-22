#ifndef LTE_StackAlloc_h__
#define LTE_StackAlloc_h__

#include "Common.h"

namespace LTE {
  template <int capacity>
  class StackAlloc {
    char   _stack[capacity];
    char*  _heap;
    size_t _size;

  public:
    StackAlloc(size_t size) :
      _size(size),
      _heap(0)
    {
      if (size > capacity)
        _heap = new char[size];
    }

    ~StackAlloc() {
      delete[] _heap;
    }

    char* data() {
      return _heap ? _heap : _stack;
    }

    bool isHeap() const {
      return _heap != 0;
    }

    size_t size() const {
      return _size;
    }
  };
}

#endif
