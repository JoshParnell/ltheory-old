#ifndef LTE_Function_Wrapper_h__
#define LTE_Function_Wrapper_h__

namespace LTE {
  template <class T, class ReturnType>
  struct WrapperT : public T {
    WrapperT(T const& t) :
      T(t)
      {}
  };

  template <class T>
  WrapperT<T, typename T::ReturnType>
  Wrapper(T const& t) {
    return WrapperT<T, typename T::ReturnType>(t);
  }
}

#endif
