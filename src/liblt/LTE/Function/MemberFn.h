#ifndef LTE_Function_MemberFn_h__
#define LTE_Function_MemberFn_h__

#include "Wrapper.h"

namespace LTE {
  template <class ParentT, class T>
  struct MemberFnT {
    typedef typename GetReturnType<T>::Result ReturnType;

    ParentT receiver;
    T t;

    MemberFnT(ParentT const& receiver,
              T const& t) :
      receiver(receiver),
      t(t)
      {}

    ReturnType operator()() {
      return (receiver->*t)();
    }

    template <class A1>
    ReturnType operator()(A1 const& a1) {
      return (receiver->*t)(a1);
    }

    template <class A1, class A2>
    ReturnType operator()(A1 const& a1, A2 const& a2) {
      return (receiver->*t)(a1, a2);
    }

    template <class A1, class A2, class A3>
    ReturnType operator()(A1 const& a1, A2 const& a2, A3 const& a3) {
      return (receiver->*t)(a1, a2, a3);
    }
  };

  template <class ParentT, class T>
  WrapperT<MemberFnT<ParentT*, T>, typename MemberFnT<ParentT*, T>::ReturnType>
  MemberFn(ParentT* p, T const& t) {
    return Wrapper(MemberFnT<ParentT*, T>(p, t));
  }

  template <class ParentT, class T>
  WrapperT<MemberFnT<ParentT const*, T>,
           typename MemberFnT<ParentT const*, T>::ReturnType>
  MemberFn(ParentT const* p, T const& t) {
    return Wrapper(MemberFnT<ParentT const*, T>(p, t));
  }
}

#endif
