#ifndef LTE_Sparse_h__
#define LTE_Sparse_h__

#include "Common.h"

namespace LTE {
  template <class T>
  struct Sparse {
    mutable T* t;

    Sparse() :
      t(0)
      {}

    ~Sparse() {
      delete t;
    }

    Sparse(Sparse const& other) :
      t(other.t ? new T(*other.t) : 0)
      {}

    operator bool() {
      return t != 0;
    }

    operator bool() const {
      return t != 0;
    }

    T& operator*() {
      return *(t ? t : t = new T);
    }

    T* operator->() {
      return t ? t : t = new T;
    }

    Sparse& operator=(Sparse const& other) {
      delete t;
      t = other.t ? new T(*other.t) : 0;
      return *this;
    }

    T const& operator*() const {
      return *(t ? t : t = new T);
    }

    T const* operator->() const {
      return t ? t : t = new T;
    }
  };
}

#endif
