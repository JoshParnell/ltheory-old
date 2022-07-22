#if defined(LTE_Pointer_h__) && defined(LTE_Reference_h__)
#ifndef LTE_Compare_Pointer_Reference_h__
#define LTE_Compare_Pointer_Reference_h__

namespace LTE {
  template <class T>
  bool operator==(Pointer<T> const& a, Reference<T> const& b) {
    return a.t == b.t;
  }

  template <class T>
  bool operator==(Reference<T> const& a, Pointer<T> const& b) {
    return a.t == b.t;
  }

  template <class T>
  bool operator!=(Pointer<T> const& a, Reference<T> const& b) {
    return a.t != b.t;
  }

  template <class T>
  bool operator!=(Reference<T> const& a, Pointer<T> const& b) {
    return a.t != b.t;
  }

  template <class T>
  bool operator< (Pointer<T> const& a, Reference<T> const& b) {
    return a.t < b.t;
  }

  template <class T>
  bool operator< (Reference<T> const& a, Pointer<T> const& b) {
    return a.t < b.t;
  }
}

#endif
#endif
