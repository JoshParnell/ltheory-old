#ifndef LTE_Data_h__
#define LTE_Data_h__

#include "String.h"
#include "Type.h"

const unsigned long long kDataMagic = 0xDAD91633BFAFC00DULL;

namespace LTE {
  /* DataRef does not own the type's memory. */
  struct DataRef {
    typedef DataRef SelfType;

    Type type;
    void* data;

    DataRef() :
      type(0),
      data(0)
      {}

    DataRef(Type const& type, void* data) :
      type(type),
      data(data)
      {}

    template <class T>
    DataRef(T const& t) :
      type(Type_Get(t)),
      data((void*)&t)
      {}

    DataRef(Data const& other);
    DataRef& operator=(Data const& other);

    int64 CastInt() const {
      if (!type || !type->castInt)
        error("Contained type does not support integer cast");
      return type->CastInt(data);
    }

    double CastReal() const {
      if (!type || !type->castReal)
        error("Contained type does not support real cast");
      return type->CastReal(data);
    }

    template <class T>
    T& Convert() {
      if (type != Type_Get<T>())
        error("Destination type does not match data type");
      return *(T*)data;
    }

    template <class T>
    T const& Convert() const {
      return ((SelfType*)this)->Convert<T>();
    }

    template <class T>
    bool IsType() const {
      return type && type == Type_Get<T>();
    }

    template <class StreamT>
    friend void _ToStream(StreamT& stream, DataRef const& self) {
      stream << (self.type ? self.type->name : "null");
    }
  };

  struct Data : public NullBase<Data> {
    typedef Data SelfType;
    typedef NullBase<Data> BaseType;

    Type type;
    void* data;

    Data() :
      type(0),
      data(0)
      {}

    Data(Data const& other) :
      type(0),
      data(0)
    {
      (*this) = other;
    }

    Data(DataRef const& other) :
      type(0),
      data(0)
    {
      (*this) = other;
    }

    template <class T>
    Data(T const& t) :
      type(Type_Get(t))
    {
      LTE_ASSERT(type->allocate);
      data = type->Allocate();
      LTE_ASSERT(type->assign);
      type->Assign(&t, data);
    }

    Data(Type const& type, void* data) :
      type(type)
    {
      LTE_ASSERT(type->allocate);
      this->data = type->Allocate();
      LTE_ASSERT(type->assign);
      type->Assign(data, this->data);
    }

    ~Data() {
      Clear();
    }

    operator bool() {
      return data != 0;
    }

    operator bool() const {
      return data != 0;
    }

    template <class T>
    Data& operator=(T const& t) {
      Clear();
      type = Type_Get<T>();
      LTE_ASSERT(type->allocate);
      data = type->Allocate();
      LTE_ASSERT(type->assign);
      type->Assign(&t, data);
      return *this;
    }

    Data& operator=(Data const& other) {
      if (this == &other)
        return *this;

      Clear();
      type = other.type;

      if (type) {
        LTE_ASSERT(type->allocate);
        data = type->Allocate();
        LTE_ASSERT(type->assign);
        type->Assign(other.data, data);
      }
      return *this;
    }

    Data& operator=(DataRef const& other) {
      Clear();
      type = other.type;

      if (type) {
        LTE_ASSERT(type->allocate);
        data = type->Allocate();
        LTE_ASSERT(type->assign);
        type->Assign(other.data, data);
      }
      return *this;
    }

    int64 CastInt() const {
      if (!type || !type->castInt)
        error("Contained type does not support integer cast");
      return type->CastInt(data);
    }

    double CastReal() const {
      if (!type || !type->castReal)
        error("Contained type does not support real cast");
      return type->CastReal(data);
    }

    void Clear() {
      if (data && type)
        type->Deallocate(data);
      data = 0;
      type = 0;
    }

    void Construct(Type const& type) {
      Clear();
      this->type = type;
      LTE_ASSERT(type->allocate);
      data = type->Allocate();
    }

    template <class T>
    T& Convert() {
      if (type != Type_Get<T>())
        error("Destination type does not match data type");
      return *(T*)data;
    }

    template <class T>
    T const& Convert() const {
      return ((SelfType*)this)->Convert<T>();
    }

    template <class T>
    bool IsType() const {
      return type == Type_Get<T>();
    }

    FIELDS {
      Data* self = (Data*)addr;
      if (self) {
        String typeName;
        if (self->type)
          typeName = self->type->name;

        String oldTypeName = typeName;

        unsigned long long magic = kDataMagic;
        m(&magic, "magic", Type_Get<unsigned long long>(), aux);

        if (magic != kDataMagic) {
          self->Clear();
          self->type = 0;
          return;
        }

        m(&typeName, "typeName", Type_Get<String>(), aux);

        if (typeName != oldTypeName) {
          self->Clear();
          Type const& newType = Type_Find(typeName);
          if (!newType)
            error("Could not resolve dynamic type name");
          self->Construct(newType);
        }
        
        m(self->data, "data", self->type, aux);
      } else {
        MAPFIELD(type)
        MAPFIELD(data)
      }
    }

    template <class StreamT>
    friend void _ToStream(StreamT& stream, Data const& self) {
      stream << (self.type ? self.type->name : "null");
    }

    DefineMetadataInline(Data)
  };

  inline DataRef::DataRef(Data const& other) :
    type(other.type),
    data(other.data)
    {}

  inline DataRef& DataRef::operator=(Data const& other) {
    if (type != other.type)
      error("Assignment type does not match contained type");
    type->Assign(other.data, data);
    return *this;
  }
}

#endif
