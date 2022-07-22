#ifndef LTE_DataStack_h__
#define LTE_DataStack_h__

#include "Type.h"

const size_t kAlignment = sizeof(void*);

namespace LTE {
  struct DataStack {
    typedef DataStack SelfType;

    char* buffer;
    char* ptr;
    size_t capacity;

    DataStack(size_t capacity = 16 * 1024) :
      capacity(capacity)
    {
      buffer = new char[capacity];
      ptr = buffer;
    }

    ~DataStack() {
      delete[] buffer;
    }

    size_t Align(size_t sz) {
      return (sz + kAlignment - 1) - (sz + kAlignment - 1) % kAlignment;
    }

    template <class T>
    T* Allocate() {
      return (T*)Allocate(Type_Get<T>());
    }

    void* Allocate(Type const& type) {
      void* p = ptr;
      type->construct(type, p);
      ptr += Align(type->size);
      return p;
    }

    template <class T>
    void Free() {
      Free(Type_Get<T>());
    }

    void Free(Type const& type) {
      ptr -= Align(type->size);
      type->destruct(type, ptr);
    }
  };
}

#endif
