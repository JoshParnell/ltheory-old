#ifndef LTE_Function_Lookup_h__
#define LTE_Function_Lookup_h__

#include "Wrapper.h"

namespace LTE {
  template <class T, class IndexT>
  struct LookupT {
    typedef typename GetReturnType<T>::Result ExprT;
    typedef typename GetReturnType<ExprT>::Result ReturnType;

    T t;
    IndexT index;

    LookupT(T const& t,
            IndexT const& index) :
      t(t),
      index(index)
      {}

    ReturnType operator()() {
      return t()[index];
    }

    template <class A1>
    ReturnType operator()(A1 const& a1) {
      return t(a1)[index];
    }

    template <class A1, class A2>
    ReturnType operator()(A1 const& a1, A2 const& a2) {
      return t(a1, a2)[index];
    }

    template <class A1, class A2, class A3>
    ReturnType operator()(A1 const& a1, A2 const& a2, A3 const& a3) {
      return t(a1, a2, a3)[index];
    }
  };

  template <class T, class IndexT>
  WrapperT<LookupT<T, IndexT>, typename LookupT<T, IndexT>::ReturnType>
  Lookup(T const& t, IndexT const& index) {
    return Wrapper(LookupT<T, IndexT>(t, index));
  }
}

#endif
