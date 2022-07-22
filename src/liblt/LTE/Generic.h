#ifndef LTE_Generic_h__
#define LTE_Generic_h__

#include "Hash.h"
#include "Reference.h"
#include "TypeTraits.h"

namespace LTE {
  template <class ReturnT>
  struct FunctionTraits {
    typedef ReturnT ReturnType;
  };

  template <class T, class ArgT>
  struct FunctionInterface :
    public FunctionTraits<T>,
    public RefCounted
  {
    virtual ~FunctionInterface() {}

    virtual char const* GetName() const {
      return "unknown_function";
    }

    virtual T operator()(ArgT const& arg) const = 0;
  };

  /* void specialization. */
  template <class T>
  struct FunctionInterface<T, void> :
    public FunctionTraits<T>,
    public RefCounted
  {
    virtual ~FunctionInterface() {}

    virtual size_t GetHash() const {
      return 0;
    }

    virtual char const* GetName() const {
      return "unknown_function";
    }

    virtual T operator()() const = 0;
  };

  template <class T>
  struct Function0ArgT :
    public FunctionInterface<typename T::ReturnType, void>
  {
    typedef typename T::ReturnType ReturnType;

    mutable T t;

    Function0ArgT(T const& t) :
      t(t)
      {}

    size_t GetHash() const {
      return Hash(t);
    }

    char const* GetName() const {
#ifdef __PRETTY_FUNCTION__
      return __PRETTY_FUNCTION__;
#else
      return __FUNCTION__;
#endif
    }

    ReturnType operator()() const {
      return t();
    }
  };

  template <class T, class A1>
  struct Function1ArgT :
    public FunctionInterface<typename T::ReturnType, A1>
  {
    typedef typename T::ReturnType ReturnType;

    mutable T t;

    Function1ArgT(T const& t) :
      t(t)
      {}

    size_t GetHash() const {
      return Hash(t);
    }

    char const* GetName() const {
#ifdef __PRETTY_FUNCTION__
      return __PRETTY_FUNCTION__;
#else
      return __FUNCTION__;
#endif
    }

    ReturnType operator()(A1 const& a1) const {
      return t(a1);
    }
  };

  template <class T>
  FunctionInterface<typename T::ReturnType, void>*
  Function0Arg(T const& t) {
    return new Function0ArgT<T>(t);
  }

  template <class A1, class T>
  FunctionInterface<typename T::ReturnType, A1>*
  Function1Arg(T const& t) {
    return new Function1ArgT<T, A1>(t);
  }
}

#include "Function/Bind.h"
#include "Function/Cached.h"
#include "Function/Call.h"
#include "Function/Cast.h"
#include "Function/FreeFn.h"
#include "Function/Lookup.h"
#include "Function/Pointer.h"
#include "Function/MemberFn.h"
#include "Function/Value.h"

namespace LTE {
  template <class T, class ArgT>
  struct GenericBase {
    typedef T ReturnType;

    Reference<FunctionInterface<T, ArgT> > fn;

    GenericBase() {}

    GenericBase(FunctionInterface<T, ArgT>* fn) :
      fn(fn)
      {}

    GenericBase(GenericBase const& other) :
      fn(other.fn)
      {}

    operator bool() const {
      return fn != nullptr;
    }

    GenericBase& operator=(FunctionInterface<T, ArgT>* fn) {
      this->fn = fn;
      return *this;
    }

    GenericBase& operator=(GenericBase const& other) {
      if (&other != this)
        fn = other.fn;
      return *this;
    }
  };

  template <class T, class ArgT = void>
  struct Generic : public GenericBase<T, ArgT> {
    Generic() {}

    Generic(int voidPtr) {}

    Generic(T (*fn)(ArgT const& arg)) :
      GenericBase<T, ArgT>(Function1Arg<ArgT>(FreeFn(fn)))
      {}

    Generic(FunctionInterface<T, ArgT>* fn) :
      GenericBase<T, ArgT>(fn)
      {}

    template <class U>
    Generic(WrapperT<U, T> const& wrapper) :
      GenericBase<T, ArgT>(Function1Arg<ArgT>(wrapper))
      {}

    Generic(Generic const& other) :
      GenericBase<T, ArgT>(other)
      {}

    T operator()(ArgT const& arg) const {
      return (*this->fn)(arg);
    }

    template <class StreamT>
    friend void _ToStream(StreamT& stream, Generic const& t) {
      stream << "[Generic]";
    }
  };

  template <class T>
  struct Generic<T, void> : public GenericBase<T, void> {
    Generic() {}

    Generic(T const& value) :
      GenericBase<T, void>(Function0Arg(Value(value)))
      {}

    Generic(T (*fn)()) :
      GenericBase<T, void>(Function0Arg(FreeFn(fn)))
      {}

    Generic(FunctionInterface<T, void>* fn) :
      GenericBase<T, void>(fn)
      {}

    template <class U>
    Generic(WrapperT<U, T> const& wrapper) :
      GenericBase<T, void>(Function0Arg(wrapper))
      {}

    T operator()() const {
      return (*this->fn)();
    }

    template <class StreamT>
    friend void _ToStream(StreamT& stream, Generic const& t) {
      stream << "[Generic]";
    }
  };

  template <>
  struct Generic<void, void> : public GenericBase<void, void> {
    Generic() {}

    Generic(FunctionInterface<void, void>* fn) :
      GenericBase<void, void>(fn)
      {}

    template <class U>
    Generic(WrapperT<U, void> const& wrapper) :
      GenericBase<void, void>(Function0Arg(wrapper))
      {}

    Generic(Generic const& other) :
      GenericBase<void, void>(other)
      {}

    void operator()() const {
      return (*this->fn)();
    }

    template <class StreamT>
    friend void _ToStream(StreamT& stream, Generic const& t) {
      stream << "[Generic]";
    }
  };
}

#endif
