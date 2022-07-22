#ifndef LTE_Reference_h__
#define LTE_Reference_h__

#include "Mutable.h"
#include "Type.h"

#define DEBUG_POINTERS

struct RefCounted : public NullBase<RefCounted> {
  uint refCount;

  RefCounted() : refCount(0) {}

  ~RefCounted() {
    LTE_ASSERT(refCount == 0);
  }

  bool RefCountDecrement() {
    LTE_ASSERT(refCount > 0);
    return --refCount == 0;
  }

  void RefCountIncrement() {
    refCount++;
  }
};

template <class T>
struct Reference : public NullBase<Reference<T> > {
  typedef NullBase<Reference<T> > BaseType;
  typedef Reference SelfType;
  T* t;

  Reference(T* t = 0) : t(t) {
    Acquire();
  }

  Reference(Reference const& ref) : t(ref.t) {
    Acquire();
  }

  template <class B>
  Reference(B* t) : t(static_cast<T*>(t)) {
    Acquire();
  }

  template <class B>
  Reference(Reference<B> const& ref) : t(static_cast<T*>(ref.t)) {
    Acquire();
  }

  ~Reference() {
    Release();
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

  Reference& operator=(T* t) {
    if (t)
      Mutable(t)->RefCountIncrement();
    Release();
    this->t = t;
    return *this;
  }

  Reference& operator=(Reference const& ref) {
    if (this == &ref)
      return *this;
    if (ref.t)
      Mutable(ref.t)->RefCountIncrement();
    Release();
    t = ref.t;
    return *this;
  }

  friend bool operator==(Reference const& one, Reference const& two) {
    return one.t == two.t;
  }

  friend bool operator!=(Reference const& one, Reference const& two) {
    return one.t != two.t;
  }

  friend bool operator==(Reference const& r, T* t) {
    return r.t == t;
  }

  friend bool operator==(T* t, Reference const& r) {
    return r.t == t;
  }

  friend bool operator!=(Reference const& r, T* t) {
    return r.t != t;
  }

  friend bool operator!=(T* t, Reference const& r) {
    return r.t != t;
  }

  friend bool operator<(Reference const& one, Reference const& two) {
    return one.t < two.t;
  }

  friend bool operator<(Reference const& r, T* t) {
    return r.t < t;
  }

  friend bool operator<(T* t, Reference const& r) {
    return t < r.t;
  }

  T* operator->() const {
#ifdef DEBUG_POINTERS
    if (!t)
      error("Attempt to access null reference");
#endif
    return t;
  }

  T& operator*() const {
#ifdef DEBUG_POINTERS
    if (!t)
      error("Attempt to access null reference");
#endif
    return *t;
  }

  void Acquire() {
    if (t)
      Mutable(t)->RefCountIncrement();
  }

  template <class T2>
  T2* Cast() {
    return static_cast<T2*>(t);
  }

  template <class T2>
  T2 const* Cast() const {
    return static_cast<T2 const*>(t);
  }

  void Release() {
    if (t && Mutable(t)->RefCountDecrement())
      delete t;
  }

  template <class StreamT>
  friend void _ToStream(StreamT& stream, Reference const& t) {
    if (!t.t)
      stream << "null";
    else
      ToStream(stream, *t.t);
  }

  FIELDS {
    Reference* self = (Reference*)addr;
    T* oldPtr = self->t;
    m(&self->t, "data", Type_Get(self->t), aux);
    T* newPtr = self->t;

    if (oldPtr != newPtr) {
      self->t = oldPtr;
      self->Release();
      self->t = newPtr;
      self->Acquire();
    }
  }

  METADATA {
    type->GetPointeeType() = Type_Get<T>();
  }

  AUTOMATIC_REFLECTION_PARAMETRIC1(Reference, T)
};

#include "Compare_AutoPtr_Reference.h"
#include "Compare_Pointer_Reference.h"

#endif
