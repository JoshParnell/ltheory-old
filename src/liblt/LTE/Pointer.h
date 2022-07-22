#ifndef LTE_Pointer_h__
#define LTE_Pointer_h__

#include "Type.h"

#define DEBUG_POINTERS

template <class T>
struct Pointer : public NullBase<Pointer<T> > {
  typedef NullBase<Pointer<T> > BaseType;
  typedef Pointer SelfType;
  T* t;

  Pointer() : t(0) {}

  Pointer(T* t) : t(t) {}

  Pointer& operator=(T* t) {
    this->t = t;
    return *this;
  }

  Pointer& operator=(Pointer const& other) {
    this->t = other.t;
    return *this;
  }

  friend bool operator==(Pointer const& one, Pointer const& two) {
    return one.t == two.t;
  }

  friend bool operator==(Pointer const& p, T* t) {
    return p.t == t;
  }

  friend bool operator==(T* t, Pointer const& p) {
    return p.t == t;
  }

  friend bool operator!=(Pointer const& one, Pointer const& two) {
    return one.t != two.t;
  }

  friend bool operator!=(Pointer const& p, T* t) {
    return p.t != t;
  }

  friend bool operator!=(T* t, Pointer const& p) {
    return p.t != t;
  }

  friend bool operator< (Pointer const& one, Pointer const& two) {
    return one.t < two.t;
  }

  friend bool operator< (Pointer const& p, T* t) {
    return p.t < t;
  }

  friend bool operator< (T* t, Pointer const& p) {
    return p.t < t;
  }

  operator bool() {
    return t != 0;
  }

  operator bool() const {
    return t != 0;
  }

  operator T*() const {
    return t;
  }

  T& operator*() const {
#ifdef DEBUG_POINTERS
    if (!t)
      error("Attempt to dereference null pointer.");
#endif
    return *t;
  }

  T* operator->() const {
#ifdef DEBUG_POINTERS
    if (!t)
      error("Attempt to dereference null pointer.");
#endif
    return t;
  }

  template <class T2>
  T2* Cast() {
    return static_cast<T2*>(t);
  }

  template <class T2>
  T2 const* Cast() const {
    return static_cast<T2 const*>(t);
  }

  template <class StreamT>
  friend void _ToStream(StreamT& stream, Pointer const& t) {
    if (!t.t)
      stream << "null";
    else
      ToStream(stream, *t.t);
  }

  FIELDS {
    MAPFIELD(t)
  }

  METADATA {
    type->GetPointeeType() = Type_Get<T>();
  }

  AUTOMATIC_REFLECTION_PARAMETRIC1(Pointer, T)
};

#include "Compare_AutoPtr_Pointer.h"
#include "Compare_Pointer_Reference.h"

#endif
