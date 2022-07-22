#ifndef LTE_Iterator_h__
#define LTE_Iterator_h__

#include "Common.h"

#define LIST_ITERATOR(type, nextPtr)                                           \
  struct __list_iterator {                                                     \
    type& head;                                                                \
    type prev;                                                                 \
    type curr;                                                                 \
    type next;                                                                 \
                                                                               \
    __list_iterator(type& head, type const& curr, type const& next) :          \
      head(head),                                                              \
      prev(0),                                                                 \
      curr(curr),                                                              \
      next(next) {}                                                            \
                                                                               \
    operator bool() { return curr; }                                           \
    operator bool() const { return curr; }                                     \
    operator type const&() const { return curr; }                              \
    type const& operator->() const { return curr; }                            \
    type const& operator*() const { return curr; }                             \
    void operator++() {                                                        \
      prev = curr;                                                             \
      curr = next;                                                             \
      next = curr ? (type)curr->nextPtr : (type)0;                             \
    }                                                                          \
                                                                               \
    void Remove() {                                                            \
      if (prev)                                                                \
        prev->nextPtr = next;                                                  \
      else                                                                     \
        head = next;                                                           \
      curr = prev;                                                             \
    }                                                                          \
  };

#define LIST_ITERATE(type, head, nextPtr)                                      \
  LIST_ITERATOR(type, nextPtr)                                                 \
  for (__list_iterator it(head, head, head ? head->nextPtr : (type)0); it; ++it)

#define DLIST_ITERATOR(type, nextPtr, prevPtr)                                 \
  struct __dlist_iterator {                                                    \
    type& head;                                                                \
    type& tail;                                                                \
    type prev;                                                                 \
    type curr;                                                                 \
    type next;                                                                 \
                                                                               \
    __dlist_iterator(type& head, type& tail,                                   \
        type const& prev, type const& curr, type const& next) :                \
      head(head),                                                              \
      tail(tail),                                                              \
      prev(prev),                                                              \
      curr(curr),                                                              \
      next(next) {}                                                            \
                                                                               \
    operator bool() { return curr; }                                           \
    operator bool() const { return curr; }                                     \
    operator type const&() const { return curr; }                              \
    type const& operator->() const { return curr; }                            \
    type const& operator*() const { return curr; }                             \
                                                                               \
    void operator++() {                                                        \
      prev = curr;                                                             \
      curr = next;                                                             \
      next = curr ? (type)curr->nextPtr : (type)0;                             \
    }                                                                          \
                                                                               \
    void Remove() {                                                            \
      if (prev)                                                                \
        prev->nextPtr = next;                                                  \
      else                                                                     \
        head = next;                                                           \
      if (next)                                                                \
        next->prevPtr = prev;                                                  \
      else                                                                     \
        tail = prev;                                                           \
      curr = prev;                                                             \
    }                                                                          \
  };

#define DLIST_ITERATE(type, head, tail, nextPtr, prevPtr)                      \
  DLIST_ITERATOR(type, nextPtr, prevPtr)                                       \
  for (__dlist_iterator it(head, tail, nullptr, head, head ? (type)head->nextPtr : (type)0);  \
       it; ++it)

#define LIST_INSERT(head, nextPtr, elem)                                       \
  { elem->nextPtr = head;                                                      \
    head = elem; }

#define DLIST_INSERT(head, tail, nextPtr, prevPtr, elem)                       \
  { elem->nextPtr = head;                                                      \
    if (head) head->prevPtr = elem;                                            \
    else tail = elem;                                                          \
    head = elem; }

#define DLIST_INSERT_BACK(head, tail, nextPtr, prevPtr, elem)                  \
  { elem->prevPtr = tail;                                                      \
    if (tail) tail->nextPtr = elem;                                            \
    else head = elem;                                                          \
    tail = elem; }

#define LIST_REMOVE(type, head, nextPtr, elem)                                 \
  { LIST_ITERATE(type, head, nextPtr) {                                        \
    if ((type)it == elem) {                                                    \
      it.Remove();                                                             \
      break;                                                                   \
    }                                                                          \
  } }

#define DLIST_REMOVE(type, head, tail, nextPtr, prevPtr, elem)                 \
  { DLIST_ITERATE(type, head, tail, nextPtr, prevPtr) {                        \
    if ((type)it == elem) {                                                    \
      it.Remove();                                                             \
      break;                                                                   \
    }                                                                          \
  } }

#endif
