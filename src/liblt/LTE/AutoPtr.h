#ifndef LTE_AutoPtr_h__
#define LTE_AutoPtr_h__

#include "Type.h"

#define DEBUG_POINTERS

template <class T>
struct AutoPtr : public NullBase<AutoPtr<T> > {
  typedef NullBase<AutoPtr<T> > BaseType;
  typedef AutoPtr SelfType;
  T* t;

  AutoPtr(T* t = 0) : t(t) {}

  AutoPtr(AutoPtr const& other) : t(other.t) {
    ((AutoPtr&)other).t = 0;
  }

  ~AutoPtr() {
    delete t;
  }

  AutoPtr& operator=(T* t) {
    delete this->t;
    this->t = t;
    return *this;
  }

  AutoPtr& operator=(AutoPtr const& other) {
    if (this != &other) {
      delete t;
      t = other.t;
      ((AutoPtr&)other).t = 0;
    }
    return *this;
  }

  friend bool operator==(AutoPtr const& one, AutoPtr const& two) {
    return one.t == two.t;
  }

  friend bool operator==(AutoPtr const& p, T* t) {
    return p.t == t;
  }

  friend bool operator==(T* t, AutoPtr const& p) {
    return p.t == t;
  }

  operator bool() {
    return t != 0;
  }

  operator bool() const {
    return t != 0;
  }

  T* operator->() const {
#ifdef DEBUG_POINTERS
    if (!t)
      error("Attempt to access null autopointer");
#endif
    return t;
  }

  T& operator*() const {
#ifdef DEBUG_POINTERS
    if (!t)
      error("Attempt to access null autopointer");
#endif
    return *t;
  }

  operator T*() const {
    return t;
  }

  template <class StreamT>
  friend void _ToStream(StreamT& stream, AutoPtr const& t) {
    if (!t.t)
      stream << "null";
    else
      ToStream(stream, *t.t);
  }

  FIELDS {
    AutoPtr* self = (AutoPtr*)addr;
    T* oldValue = self->t;
    m(&self->t, "data", Type_Get(self->t), aux);
    if (oldValue && self->t != oldValue)
      delete oldValue;
  }

  METADATA {
    type->GetPointeeType() = Type_Get<T>();
  }

  AUTOMATIC_REFLECTION_PARAMETRIC1(AutoPtr, T)
};

#include "Compare_AutoPtr_Pointer.h"
#include "Compare_AutoPtr_Reference.h"

#endif
