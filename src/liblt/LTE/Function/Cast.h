#ifndef LTE_Function_Cast_h__
#define LTE_Function_Cast_h__

#include "Wrapper.h"

namespace LTE {
  template <class T, class U>
  struct CastT {
    typedef U ReturnType;

    T t;

    CastT(T const& t) :
      t(t)
      {}

    ReturnType operator()() {
      return (U)t();
    }

    template <class ArgT>
    ReturnType operator()(ArgT const& arg) {
      return (U)t(arg);
    }
  };

  template <class U, class T>
  WrapperT<CastT<T, U>, typename CastT<T, U>::ReturnType>
  Cast(T const& t) {
    return Wrapper(CastT<T, U>(t));
  }
}

#endif
