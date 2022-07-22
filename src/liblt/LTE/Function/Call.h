#ifndef LTE_Function_Call_h__
#define LTE_Function_Call_h__

#include "Wrapper.h"

namespace LTE {
  template <class T>
  struct CallT {
    typedef typename T::ReturnType FnType;
    typedef typename FnType::ReturnType ReturnType;

    T t;

    CallT(T const& t) :
      t(t)
      {}

    ReturnType operator()() {
      return t()();
    }

    template <class A1>
    ReturnType operator()(A1 const& a1) {
      return t()(a1);
    }

    template <class A1, class A2>
    ReturnType operator()(A1 const& a1, A2 const& a2) {
      return t()(a1, a2);
    }
  };

  template <class T>
  WrapperT<CallT<T>, typename CallT<T>::ReturnType>
  Call(T const& t) {
    return Wrapper(CallT<T>(t));
  }
}

#endif
