#ifndef LTE_Function_FreeFn_h__
#define LTE_Function_FreeFn_h__

namespace LTE {
  template <class T>
  struct FreeFnT {
    typedef typename GetReturnType<T>::Result ReturnType;

    T* t;

    FreeFnT(T* t) :
      t(t)
      {}

    ReturnType operator()() {
      return t();
    }

    template <class A1>
    ReturnType operator()(A1 const& a1) {
      return t(a1);
    }

    template <class A1, class A2>
    ReturnType operator()(A1 const& a1, A2 const& a2) {
      return t(a1, a2);
    }

    template <class A1, class A2, class A3>
    ReturnType operator()(A1 const& a1, A2 const& a2, A3 const& a3) {
      return t(a1, a2, a3);
    }
  };

  template <class T>
  WrapperT<FreeFnT<T>, typename FreeFnT<T>::ReturnType>
  FreeFn(T* t) {
    return Wrapper(FreeFnT<T>(t));
  }
}

#endif
