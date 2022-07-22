#ifndef LTE_InternalList_h__
#define LTE_InternalList_h__

#include "Pointer.h"

#define INTERNAL_LIST(type, name)                                              \
  struct name##_Link {                                                         \
    LTE::Pointer<name##_Link> prev;                                            \
    LTE::Pointer<name##_Link> next;                                            \
                                                                               \
    ~name##_Link() {                                                           \
      if (prev || next)                                                        \
        remove();                                                              \
    }                                                                          \
                                                                               \
    void insert(type& t) {                                                     \
      t.__##name.remove();                                                     \
      name##_Link* oldNext = next;                                             \
      next = &t.__##name;                                                      \
      t.__##name.prev = this;                                                  \
      t.__##name.next = oldNext;                                               \
      if (oldNext) oldNext->prev = &t.__##name;                                \
    }                                                                          \
                                                                               \
    void remove() {                                                            \
      if (prev || next)                                                        \
        _remove();                                                             \
    }                                                                          \
                                                                               \
    LT_API void _remove();                                                     \
                                                                               \
  } __##name;                                                                  \
                                                                               \
  struct name : public name##_Link {                                           \
    struct Iterator {                                                          \
      name##_Link* cur;                                                        \
      Iterator* nextIter;                                                      \
      Iterator* prevIter;                                                      \
                                                                               \
      LT_API Iterator(name##_Link* cur = nullptr);                             \
      LT_API Iterator(Iterator const& other);                                  \
      LT_API ~Iterator();                                                      \
      LT_API Iterator& operator=(Iterator const& other);                       \
                                                                               \
      operator bool() {                                                        \
        return cur != nullptr;                                                 \
      }                                                                        \
                                                                               \
      operator bool() const {                                                  \
        return cur != nullptr;                                                 \
      }                                                                        \
                                                                               \
      operator type*() {                                                       \
        return &(**this);                                                      \
      }                                                                        \
                                                                               \
      type* operator->() {                                                     \
        return &(**this);                                                      \
      }                                                                        \
                                                                               \
      type& operator*() {                                                      \
        return *((type*)((volatile char const*)cur -                           \
                         (volatile char const*)&((type*)0)->__##name));        \
      }                                                                        \
                                                                               \
      Iterator& operator++() {                                                 \
        if (cur)                                                               \
          cur = cur->next;                                                     \
        return *this;                                                          \
      }                                                                        \
    };                                                                         \
                                                                               \
    Iterator begin() {                                                         \
      return Iterator(this->next);                                             \
    }                                                                          \
  };

#define INTERNAL_LIST_IMPL(type, name)                                         \
  namespace {                                                                  \
    LTE::Pointer<type::name::Iterator> __##type##_##name##_iterators;          \
  }                                                                            \
                                                                               \
  void type::name##_Link::_remove() {                                          \
    if (next) next->prev = prev;                                               \
    if (prev) prev->next = next;                                               \
    for (type::name::Iterator* it = __##type##_##name##_iterators;             \
         it != nullptr;                                                        \
         it = it->nextIter)                                                    \
    {                                                                          \
      if (it->cur == this)                                                     \
        it->cur = prev;                                                        \
    }                                                                          \
    next = nullptr;                                                            \
    prev = nullptr;                                                            \
  }                                                                            \
                                                                               \
  type::name::Iterator::Iterator(name##_Link* cur) {                           \
    this->cur = cur;                                                           \
    nextIter = __##type##_##name##_iterators;                                  \
    prevIter = nullptr;                                                        \
    if (nextIter)                                                              \
      nextIter->prevIter = this;                                               \
    __##type##_##name##_iterators = this;                                      \
  }                                                                            \
                                                                               \
  type::name::Iterator::Iterator(Iterator const& other) {                      \
    this->cur = other.cur;                                                     \
    nextIter = __##type##_##name##_iterators;                                  \
    prevIter = nullptr;                                                        \
    if (nextIter)                                                              \
      nextIter->prevIter = this;                                               \
    __##type##_##name##_iterators = this;                                      \
  }                                                                            \
                                                                               \
  type::name::Iterator::~Iterator() {                                          \
    if (nextIter) nextIter->prevIter = prevIter;                               \
    if (prevIter) prevIter->nextIter = nextIter;                               \
    else __##type##_##name##_iterators = nextIter;                             \
  }                                                                            \
                                                                               \
  type::name::Iterator&                                                        \
  type::name::Iterator::operator=(Iterator const& other) {                     \
    this->cur = other.cur;                                                     \
    return *this;                                                              \
  }

#endif
