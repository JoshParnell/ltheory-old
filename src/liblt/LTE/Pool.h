#ifndef LTE_Pool_h__
#define LTE_Pool_h__

#include "Common.h"

namespace LTE {
  template <size_t BlockSize, size_t ArenaSize = 32>
  class PoolRaw {
    union Element {
      Element* next;
      unsigned char buffer[BlockSize];
    };

    struct Arena {
      Element elements[ArenaSize];
      Arena* next;

      Arena() :
        next(nullptr)
      {
        for (size_t i = 0; i < ArenaSize - 1; ++i)
          elements[i].next = &elements[i + 1];
      }
    };

    Arena* headArena;
    Element* head;
    size_t capacity;
    size_t allocated;

  public:
    PoolRaw() :
      headArena(nullptr),
      head(nullptr),
      capacity(0),
      allocated(0)
      {}

    ~PoolRaw() {
      Arena* arena = headArena;
      while (arena) {
        Arena* next = arena->next;
        delete arena;
        arena = next;
      }
    }

    void* Allocate() {
      if (allocated >= capacity)
        Grow();

      allocated++;
      Element* thisElement = head;
      head = thisElement->next;
      return thisElement->buffer;
    }

    void Free(void* t) {
      Element* thisElement = (Element*)t;
      thisElement->next = head;
      head = thisElement;
      allocated--;
    }

    void Grow() {
      Arena* thisArena = new Arena;
      LTE_ASSERT(thisArena);
      thisArena->elements[ArenaSize - 1].next = head;
      head = &thisArena->elements[0];
      thisArena->next = headArena;
      headArena = thisArena;
      capacity += ArenaSize;
    }
  };

  template <class T, size_t ArenaSize = 32>
  class Pool {
  public:
    PoolRaw<sizeof(T), ArenaSize> pool;

    T* Allocate() {
      return new (pool.Allocate()) T;
    }

    void Free(T* t) {
      t->~T();
      pool.Free(t);
    }
  };

  template <class T, int sz>
  PoolRaw<sz>* GetTypePool() {
    static PoolRaw<sz>* pool = new PoolRaw<sz>;
    return pool;
  }
}

#define POOLED_TYPE                                                            \
  void* operator new(size_t sz) {                                              \
    return GetTypePool<SelfType, sizeof(SelfType)>()->Allocate();              \
  }                                                                            \
                                                                               \
  void* operator new(size_t sz, void* buf) {                                   \
    return ::new (buf) SelfType;                                               \
  }                                                                            \
                                                                               \
  void operator delete(void* p) {                                              \
    if (p)                                                                     \
      GetTypePool<SelfType, sizeof(SelfType)>()->Free(p);                      \
  }                                                                            \
                                                                               \
  void operator delete(void* p, void*) {}

#endif
