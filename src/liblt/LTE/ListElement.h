#ifndef LTE_ListElement_h__
#define LTE_ListElement_h__

#include "Type.h"

namespace LTE {
  template <class T>
  struct ListElement : public NullBase<ListElement<T> > {
    typedef NullBase<ListElement<T> > BaseType;
    T* t;

    ListElement() :
      t(0)
      {}

    ListElement& operator=(T* t) {
      this->t = t;
      return *this;
    }

    operator T*&() {
      return t;
    }

    T*& operator->() {
      return t;
    }

    FIELDS {
      ListElement* self = (ListElement*)addr;
      m(&self->t, "elem", Type_Get<T>(), aux);
    }

    DefineMetadataInline(ListElement);
  };

  template <class StreamT, class T>
  void Serialize(StreamT& s, ListElement<T>& t) {
    s & t.t;
  }
}

#endif
