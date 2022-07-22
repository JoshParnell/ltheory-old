#if defined(LTE_AutoPtr_h__) && defined(LTE_Reference_h__)
#ifndef LTE_Compare_AutoPtr_Reference_h__
#define LTE_Compare_AutoPtr_Reference_h__

namespace LTE {
  template <class T>
  bool operator==(AutoPtr<T> const& a, Reference<T> const& b) {
    return a.t == b.t;
  }

  template <class T>
  bool operator==(Reference<T> const& a, AutoPtr<T> const& b) {
    return a.t == b.t;
  }

  template <class T>
  bool operator!=(AutoPtr<T> const& a, Reference<T> const& b) {
    return a.t != b.t;
  }

  template <class T>
  bool operator!=(Reference<T> const& a, AutoPtr<T> const& b) {
    return a.t != b.t;
  }

  template <class T>
  bool operator< (AutoPtr<T> const& a, Reference<T> const& b) {
    return a.t < b.t;
  }

  template <class T>
  bool operator< (Reference<T> const& a, AutoPtr<T> const& b) {
    return a.t < b.t;
  }
}

#endif
#endif
