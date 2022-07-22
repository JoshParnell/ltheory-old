#ifndef LTE_IntrusiveList_h__
#define LTE_IntrusiveList_h__

#include "Common.h"

template <class T, int N>
struct IntrusiveListOwner {
  T next;

  IntrusiveListOwner() :
    next(0)
    {}
};

template <class T, int N>
struct IntrusiveList {
  T front;

  IntrusiveList() :
    front(0)
    {}

  struct Iterator {
    T& head;
    T prev;
    T curr;
    T next;

    Iterator(T& head, T const& curr, T const& next) :
      head(head),
      prev(0),
      curr(curr),
      next(next)
      {}

    operator bool() {
      return curr;
    }

    operator bool() const {
      return curr;
    }

    operator type const&() const {
      return curr;
    }

    type const& operator->() const {
      return curr;
    }

    type const& operator*() const {
      return curr;
    }

    void operator++() {
      prev = curr;
      curr = next;
      next = curr ? (T)((IntrusiveListOwner<T, N>*)curr)->next>
  };

}

#endif
