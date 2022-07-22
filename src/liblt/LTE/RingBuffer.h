#ifndef LTE_RingBuffer_h__
#define LTE_RingBuffer_h__

#include "Array.h"

template <class T>
struct RingBuffer : public Array<T> {
  typedef Array<T> BaseType;
  typedef RingBuffer SelfType;
  size_t index;

  RingBuffer() :
    index(0)
    {}

  void Advance() {
    index++;
  }

  T& GetCurrent() {
    return (*this)[index % this->size()];
  }

  T const& GetCurrent() const {
    return (*this)[index % this->size()];
  }

  T& GetLast() {
    return (*this)[(index + this->size() - 1) % this->size()];
  }

  T const& GetLast() const {
    return (*this)[(index + this->size() - 1) % this->size()];
  }

  T& GetNext() {
    return (*this)[(index + 1) % this->size()];
  }

  T const& GetNext() const {
    return (*this)[(index + 1) % this->size()];
  }

  T& GetRelative(int offset) {
    return (*this)[(index + offset + this->size()) % this->size()];
  }

  T const& GetRelative(int offset) const {
    return (*this)[(index + offset + this->size()) % this->size()];
  }

  FIELDS {
    RingBuffer* self = (RingBuffer*)addr;
    static Type sizeType = Type_Get(self->index);
    m(&self->index, "index", sizeType, aux);
  }

  template <class StreamT>
  friend void _ToStream(StreamT& s, RingBuffer const& v) {
    ToStream(s, v.index);
    s << " : ";
    ToStream(s, (Array<T> const&)v);
  }

  DefineMetadataInline(RingBuffer)
};

#endif
