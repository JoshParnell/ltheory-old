#ifndef LTE_TypeTraits_h__
#define LTE_TypeTraits_h__

#include "Common.h"

namespace LTE {
  template <class T>
  struct GetDereferenceType {
    typedef void Result;
  };

  template <class T>
  struct GetDereferenceType<T*> {
    typedef T Result;
  };

  template <class T>
  struct GetDereferenceType<const T> : public GetDereferenceType<T> {};

  template <class T>
  struct GetDereferenceType<AutoPtr<T> > : public GetDereferenceType<T*> {};

  template <class T>
  struct GetDereferenceType<Pointer<T> > : public GetDereferenceType<T*> {};

  template <class T>
  struct GetDereferenceType<Reference<T> > : public GetDereferenceType<T*> {};

  template <class T>
  struct GetReturnType { 
    typedef typename T::ReturnType Result;
  };

  template <class T>
  struct GetReturnType<const T> : public GetReturnType<T> {};

  template <class T>
  struct GetReturnType<AutoPtr<T> > {
    typedef T Result;
  };

  template <class T>
  struct GetReturnType<Pointer<T> > {
    typedef T Result;
  };

  template <class T>
  struct GetReturnType<Reference<T> > {
    typedef T Result;
  };

  template <class T>
  struct GetReturnType<T(*)()> {
    typedef T Result;
  };

  template <class T, class A1>
  struct GetReturnType<T(*)(A1)> {
    typedef T Result;
  };

  template <class T, class A1, class A2>
  struct GetReturnType<T(*)(A1, A2)> {
    typedef T Result;
  };

  template <class T, class A1, class A2, class A3>
  struct GetReturnType<T(*)(A1, A2, A3)> {
    typedef T Result;
  };

  template <class T, class A1, class A2, class A3, class A4>
  struct GetReturnType<T(*)(A1, A2, A3, A4)> {
    typedef T Result;
  };

  template <class T, class S>
  struct GetReturnType<T(S::*)()> {
    typedef T Result;
  };

  template <class T, class S, class A1>
  struct GetReturnType<T(S::*)(A1)> {
    typedef T Result;
  };

  template <class T, class S, class A1, class A2>
  struct GetReturnType<T(S::*)(A1, A2)> {
    typedef T Result;
  };

  template <class T, class S, class A1, class A2, class A3>
  struct GetReturnType<T(S::*)(A1, A2, A3)> {
    typedef T Result;
  };

  template <class T, class S, class A1, class A2, class A3, class A4>
  struct GetReturnType<T(S::*)(A1, A2, A3, A4)> {
    typedef T Result;
  };

  template <class T, class S>
  struct GetReturnType<T(S::*)() const> {
    typedef T Result;
  };

  template <class T, class S, class A1>
  struct GetReturnType<T(S::*)(A1) const> {
    typedef T Result;
  };

  template <class T, class S, class A1, class A2>
  struct GetReturnType<T(S::*)(A1, A2) const> {
    typedef T Result;
  };

  template <class T, class S, class A1, class A2, class A3>
  struct GetReturnType<T(S::*)(A1, A2, A3) const> {
    typedef T Result;
  };

  template <class T, class S, class A1, class A2, class A3, class A4>
  struct GetReturnType<T(S::*)(A1, A2, A3, A4) const> {
    typedef T Result;
  };

  template <class T>
  struct GetReturnType<T()> {
    typedef T Result;
  };

  template <class T, class A1>
  struct GetReturnType<T(A1)> {
    typedef T Result;
  };

  template <class T, class A1, class A2>
  struct GetReturnType<T(A1, A2)> {
    typedef T Result;
  };

  template <class T, class A1, class A2, class A3>
  struct GetReturnType<T(A1, A2, A3)> {
    typedef T Result;
  };

  template <class T, class A1, class A2, class A3, class A4>
  struct GetReturnType<T(A1, A2, A3, A4)> {
    typedef T Result;
  };

  template <class T>
  struct RemoveRef {
    typedef T Result;
  };

  template <class T> struct RemoveRef<T&> : public RemoveRef<T> {};
  template <class T> struct RemoveRef<T const&> : public RemoveRef<T> {};
}

#endif
