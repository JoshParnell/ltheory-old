#if defined(LTE_AutoPtr_h__) && defined(LTE_Pointer_h__)
#ifndef LTE_Compare_AutoPtr_Pointer_h__
#define LTE_Compare_AutoPtr_Pointer_h__

namespace LTE {
  template <class T>
  bool operator==(AutoPtr<T> const& a, Pointer<T> const& b) {
    return a.t == b.t;
  }

  template <class T>
  bool operator==(Pointer<T> const& a, AutoPtr<T> const& b) {
    return a.t == b.t;
  }

  template <class T>
  bool operator!=(AutoPtr<T> const& a, Pointer<T> const& b) {
    return a.t != b.t;
  }

  template <class T>
  bool operator!=(Pointer<T> const& a, AutoPtr<T> const& b) {
    return a.t != b.t;
  }

  template <class T>
  bool operator< (AutoPtr<T> const& a, Pointer<T> const& b) {
    return a.t < b.t;
  }

  template <class T>
  bool operator< (Pointer<T> const& a, AutoPtr<T> const& b) {
    return a.t < b.t;
  }
}

#endif
#endif
