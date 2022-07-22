#ifndef LTE_Function_Value_h__
#define LTE_Function_Value_h__

#include "Wrapper.h"

namespace LTE {
  template <class T>
  struct ValueT {
    typedef T ReturnType;

    T t;
    
    ValueT(T const& t) :
      t(t)
      {}

    T const& operator()() {
      return t;
    }
  };

  template <class T>
  WrapperT<ValueT<T>, typename ValueT<T>::ReturnType>
  Value(T const& t) {
    return Wrapper(ValueT<T>(t));
  }
}

#endif
