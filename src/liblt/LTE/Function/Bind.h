#ifndef LTE_Function_Bind_h__
#define LTE_Function_Bind_h__

#include "Wrapper.h"

namespace LTE {
  template <class T, class A1>
  struct Bind1T {
    typedef typename T::ReturnType ReturnType;

    T t;
    A1 a1;

    Bind1T(T const& t,
          A1 const& a1) :
      t(t),
      a1(a1)
      {}

    ReturnType operator()() {
      return t(a1);
    }

    template <class A2>
    ReturnType operator()(A2 const& a2) {
      return t(a1, a2);
    }

    template <class A2, class A3>
    ReturnType operator()(A2 const& a2, A3 const& a3) {
      return t(a1, a2, a3);
    }
  };

  template <class T, class A1, class A2>
  struct Bind2T {
    typedef typename T::ReturnType ReturnType;

    T t;
    A1 a1;
    A2 a2;

    Bind2T(T const& t,
          A1 const& a1,
          A2 const& a2) :
      t(t),
      a1(a1),
      a2(a2)
      {}

    ReturnType operator()() {
      return t(a1, a2);
    }

    template <class A3>
    ReturnType operator()(A3 const& a3) {
      return t(a1, a2, a3);
    }
  };

  template <class T, class A1, class A2, class A3>
  struct Bind3T {
    typedef typename T::ReturnType ReturnType;

    T t;
    A1 a1;
    A2 a2;
    A3 a3;

    Bind3T(T const& t,
          A1 const& a1,
          A2 const& a2,
          A3 const& a3) :
      t(t),
      a1(a1),
      a2(a2),
      a3(a3)
      {}

    ReturnType operator()() {
      return t(a1, a2, a3);
    }
  };

  template <class T, class A1>
  WrapperT<Bind1T<T, A1>, typename Bind1T<T, A1>::ReturnType>
  Bind(T const& t, A1 const& a1) {
    return Wrapper(Bind1T<T, A1>(t, a1));
  }

  template <class T, class A1, class A2>
  WrapperT<Bind2T<T, A1, A2>, typename Bind2T<T, A1, A2>::ReturnType>
  Bind(T const& t, A1 const& a1, A2 const& a2) {
    return Wrapper(Bind2T<T, A1, A2>(t, a1, a2));
  }

  template <class T, class A1, class A2, class A3>
  WrapperT<Bind3T<T, A1, A2, A3>, typename Bind3T<T, A1, A2, A3>::ReturnType>
  Bind(T const& t, A1 const& a1, A2 const& a2, A3 const& a3) {
    return Wrapper(Bind3T<T, A1, A2, A3>(t, a1, a2, a3));
  }
}

#endif
