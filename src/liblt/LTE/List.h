#ifndef LTE_List_h__
#define LTE_List_h__

#include "ListElement.h"

template <class T>
struct List : public NullBase<List<T> > {
  typedef NullBase<List<T> > BaseType;
  T* head;

  struct Iterator {
    T** lastPtr;
    T* elem;
    bool skip;

    Iterator(List& list) :
      lastPtr(&list.head),
      elem(list.head),
      skip(false)
      {}

    void operator++() {
      if (!skip) {
        lastPtr = &(T*&)elem->next;
        elem = elem->next;
      } else {
        skip = false;
      }
    }

    operator bool() const {
      return elem != nullptr;
    }

    T* operator*() {
      return elem;
    }
  };

  List() :
    head(0)
    {}

  T* back() {
    T* elem;
    for (elem = head; elem && elem->next; elem = elem->next) {}
    return elem;
  }

  T const* back() const {
    T const* elem;
    for (elem = head; elem && elem->next; elem = elem->next) {}
    return elem;
  }

  Iterator begin() {
    return Iterator(*this);
  }

  void deleteElements() {
    T* elem = head;
    while (elem) {
      T* next = elem->next;
      delete elem;
      elem = next;
    }
    head = 0;
  }

  bool empty() {
    return head == nullptr;
  }

  T* front() {
    return head;
  }

  T const* front() const {
    return head;
  }

  bool contains(T* t) {
    for (T* elem = head; elem; elem = elem->next)
      if (elem == t)
        return true;
    return false;
  }

  void erase(Iterator& it) {
    LTE_ASSERT(it.elem);
    *it.lastPtr = it.elem->next;
    it.elem = it.elem->next;
    it.skip = true;
  }

  template <class MapFn>
  void map(MapFn& fn) {
    for (T* elem = head; elem; elem = elem->next)
      fn(elem);
  }

  List& push(T* t) {
    t->next = head;
    head = t;
    return *this;
  }

  List& pushBack(T* t) {
    T** elem = &head;
    while (*elem)
      elem = &(T*&)(*elem)->next;
    *elem = t;
    return *this;
  }

  bool remove(T* t) {
    T** last = &head;
    for (T* elem = head; elem; elem = elem->next) {
      if (elem == t) {
        *last = elem->next;
        return true;
      }
      last = &(T*&)elem->next;
    }
    return false;
  }

  FIELDS {
    List* self = (List*)addr;
    static Type subType = Type_Get(*(T const**)0);

    T** t = &self->head;
    while (true) {
      m(t, "elem", subType, aux);
      if (*t)
        t = &(T*&)((*t)->next);
      else
        break;
    }
  }

  AUTOMATIC_REFLECTION_PARAMETRIC1(List, T)
};

/*
template <typename T>
void Serialize(StreamT& s, List<T>& l) {
  T** t = &l.head;
  while (true) {
    s & *t;
    if (*t)
      t = &(T*&)((*t)->next);
    else
      break;
  }
}
*/

#endif
