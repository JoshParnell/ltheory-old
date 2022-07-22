#ifndef LTE_VectorNP_h__
#define LTE_VectorNP_h__

#include "Data.h"
#include "Function.h"
#include "Type.h"
#include "Vector.h"

struct VectorNP : public NullBase<VectorNP> {
  typedef VectorNP SelfType;
  typedef NullBase<VectorNP> BaseType;

  Type type;
  char* buffer;
  size_t capacity;
  size_t size;

  VectorNP() :
    type(0),
    buffer(0),
    capacity(0),
    size(0)
    {}

  VectorNP(Type const& type) :
    type(type),
    buffer(0),
    capacity(0),
    size(0)
  {
    LTE_ASSERT(type->assign);
    LTE_ASSERT(type->construct);
    LTE_ASSERT(type->destruct);
  }

  VectorNP(VectorNP const& other) :
    type(0),
    buffer(0),
    capacity(0),
    size(0)
  {
    (*this) = other;
  }

  ~VectorNP() {
    Clear();
    delete[] buffer;
  }

  DataRef operator[](size_t index) {
    return DataRef(type, &buffer[index * type->size]);
  }

  VectorNP& operator=(VectorNP const& other) {
    if (&other == this)
      return *this;

    Clear();
    type = other.type;
    Reserve(other.size);

    char const* pOther = other.buffer;
    char* pSelf = buffer;
    for (size_t i = 0; i < other.size; ++i) {
      type->Construct(pSelf);
      type->Assign(pOther, pSelf);
      pOther += type->size;
      pSelf += type->size;
    }

    size = other.size;
    return *this;
  }

  template <class T>
  VectorNP& Append(T const& t) {
    return Append(DataRef(t));
  }

  VectorNP& Append(DataRef const& t) {
    if (!type)
      type = t.type;

    LTE_ASSERT(t.type == type);
    Reserve(size + 1);

    void* ptr = &buffer[size * type->size];
    type->Construct(ptr);
    type->Assign(t.data, ptr);
    size++;
    return *this;
  }

  void Clear() {
    char* pBuffer = buffer;
    for (size_t i = 0; i < size; ++i) {
      type->Destruct(pBuffer);
      pBuffer += type->size;
    }

    size = 0;
  }

  template <class T>
  void Convert(Vector<T>& dest) const {
    LTE_ASSERT(type == Type_Get<T>());
    dest.clear();
    dest.reserve(size);
    for (size_t i = 0; i < size; ++i)
      dest.push(*(T const*)&buffer[i * type->size]);
  }

  void Pop() {
    size--;
    type->Destruct(&buffer[size * type->size]);

    if (!size)
      type = 0;
  }

  void Reserve(size_t target) {
    if (capacity >= target)
      return;

    if (!buffer) {
      size_t newCapacity = 1;
      while (newCapacity < target)
        newCapacity *= 2;
      buffer = new char[newCapacity * type->size];
      capacity = newCapacity;
      return;
    }

    size_t currSize = size;
    size_t newCapacity = capacity * 2;
    while (newCapacity < target)
      newCapacity *= 2;

    char* newBuffer = new char[newCapacity * type->size];

    /* Construct elements in new buffer. */ {
      char* pNew = newBuffer;
      char const* pOld = buffer;

      for (size_t i = 0; i < size; ++i) {
        type->Construct(pNew);
        type->Assign(pOld, pNew);
        pOld += type->size;
        pNew += type->size;
      }
    }

    /* Destruct old elements. */
    Clear();
    delete[] buffer;

    /* Replace old buffer. */
    buffer = newBuffer;
    size = currSize;
    capacity = newCapacity;
  }

  size_t Size() const {
    return size;
  }

  FIELDS {
    VectorNP* self = (VectorNP*)addr;
    m(&self->type, "type", Type_Get(self->type), aux);
    for (size_t i = 0; i < self->size; ++i)
      m(&self->buffer[i * self->type->size], "elem", self->type, aux);
  }

  template <class StreamT>
  friend void _ToStream(StreamT& s, VectorNP const& self) {
    s << "VectorNP(" << self.type->name << ", " << self.size << " elements)";
  }

  DefineMetadataInline(VectorNP)
};

struct ListNPT :
  public RefCounted,
  public VectorNP
{
  ListNPT() {}

  ListNPT(Type const& type) :
    VectorNP(type)
    {}

   // DefineMetadataInline(ListNPT)
};

#endif
