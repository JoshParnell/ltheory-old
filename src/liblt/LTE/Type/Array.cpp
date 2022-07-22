#include "../Types.h"

#include "LTE/AutoClass.h"
#include "LTE/Data.h"
#include "LTE/Function.h"
#include "LTE/Map.h"
#include "LTE/Type.h"
#include "LTE/Vector.h"

#include "LTE/Debug.h"

namespace {
  Type& GetCached(Type const& subType) {
    static Map<Type, Type> cache;
    return cache[subType];
  }

  AutoClass(ArrayCustom,
    Type, type,
    char*, buffer,
    size_t, capacity,
    size_t, size)

    ArrayCustom() :
      buffer(0),
      capacity(0),
      size(0)
      {}

    ~ArrayCustom() {
      Clear();
      delete[] buffer;
    }

    ArrayCustom& operator=(ArrayCustom const& other) {
      if (&other == this)
        return *this;

      Clear();
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

    void Append(void* data) {
      Reserve(size + 1);

      void* ptr = &buffer[size * type->size];
      type->Construct(ptr);
      type->Assign(data, ptr);
      size++;
    }

    void Append(ArrayCustom* other) {
      Reserve(size + other->size);

      char const* pOther = (char const*)other->Begin();
      char* pSelf = (char*)End();
      for (size_t i = 0; i < other->size; ++i) {
        type->Construct(pSelf);
        type->Assign(pOther, pSelf);
        pOther += type->size;
        pSelf += type->size;
      }

      size += other->size;
    }

    void* Begin() {
      return buffer;
    }

    void Clear() {
      char* pBuffer = buffer;
      for (size_t i = 0; i < size; ++i) {
        type->Destruct(pBuffer);
        pBuffer += type->size;
      }

      size = 0;
    }

    void* End() {
      return &buffer[size * type->size];
    }

    void* Get(size_t index) {
      return &buffer[index * type->size];
    }

    void Pop() {
      size--;
      type->Destruct(&buffer[size * type->size]);

      if (!size)
        type = 0;
    }

    void Remove(size_t index) {
      char* pBuffer = &buffer[index * type->size];
      for (size_t i = index; i + 1 < size; i++) {
        void* dst = (void*)pBuffer;
        void* src = (void*)(pBuffer + type->size);
        type->Assign(src, dst);
        pBuffer += type->size;
      }

      Pop();
    }

    void Resize(size_t newSize) {
      if (size != newSize) {
        Clear();
        Reserve(newSize);
        for (size_t i = 0; i < newSize; i++) {
          void* ptr = &buffer[i * type->size];
          type->Construct(ptr);
          size++;
        }
      }
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
  };

  inline void ArrayCustom_Append(void** in, void* out) {
    ((ArrayCustom*)in[0])->Append(in[1]);
  }

  inline void ArrayCustom_AppendArray(void** in, void* out) {
    ((ArrayCustom*)in[0])->Append((ArrayCustom*)in[1]);
  }

  inline void ArrayCustom_Clear(void** in, void* out) {
    ((ArrayCustom*)in[0])->Clear();
  }

  inline void ArrayCustom_Get(void** in, void* out) {
    *(void**)out = ((ArrayCustom*)in[0])->Get(*(int*)in[1]);
  }

  inline void ArrayCustom_Remove(void** in, void* out) {
    ((ArrayCustom*)in[0])->Remove(*(int*)in[1]);
  }

  inline void ArrayCustom_Size(void** in, void* out) {
    *(int*)out = ((ArrayCustom*)in[0])->Size();
  }

  inline void* ArrayCustom_Allocator(TypeT* type) {
    ArrayCustom* self = new ArrayCustom;
    self->type = type->GetAux().Convert<Type>();
    return (void*)self;
  }

  inline void ArrayCustom_Construct(TypeT* type, void* buf) {
    new (buf) ArrayCustom;
    ((ArrayCustom*)buf)->type = type->GetAux().Convert<Type>();
  }

  inline void ArrayCustom_Map(TypeT*, void* addr, FieldMapper& m, void* aux) {
    if (addr) {
      ArrayCustom* self = (ArrayCustom*)addr;
      size_t oldSize = self->size;
      size_t sz = oldSize;
      m(&sz, "size", Type_Get(sz), aux);
      if (sz != oldSize)
        self->Resize(sz);

      for (size_t i = 0; i < sz; i++) {
        void* ptr = self->Get(i);
        m(ptr, "data", self->type, aux);
      }
    }
  }

  inline void ArrayCustom_ToString(TypeT* type, void* buf, String* str) {
    *str = type->name + "[";
    ArrayCustom* self = (ArrayCustom*)buf;
    Type const& elemType = type->GetAux().Convert<Type>();
    for (size_t i = 0; i < self->Size(); ++i) {
      if (i)
        (*str) += ", ";
      (*str) += elemType->ToString(self->Get(i));
    }
    *str += "]";
  }
}

Type Type_Array(Type const& elemType) {
  Type& self = GetCached(elemType);
  if (self)
    return self;

  Type elemTypePtr = Type_Pointer(elemType);
  self = Type_Create("Array<" + elemType->name + ">", sizeof(ArrayCustom));
  self->GetAux() = elemType;
  self->alignment = AlignOf<ArrayCustom>();
  self->allocate = ArrayCustom_Allocator;
  self->assign = __type_default_assign<ArrayCustom>;
  self->construct = ArrayCustom_Construct;
  self->deallocate = __type_default_deallocator<ArrayCustom>;
  self->destruct = __type_default_destruct<ArrayCustom>;
  self->mapper = ArrayCustom_Map;
  self->pointer = false;
  self->toString = ArrayCustom_ToString;

  /* Append. */ {
    Function fn = Function_Create("+=");
    fn->call = ArrayCustom_Append;
    fn->paramCount = 2;
    fn->params = new Parameter[2];
    fn->returnType = Type_Get<void>();
    Mutable(fn->params[0]) = Parameter("object", self);
    Mutable(fn->params[1]) = Parameter("element", elemType);
    self->GetFunctions().push(fn);
  }

  /* Append. */ {
    Function fn = Function_Create("+=");
    fn->call = ArrayCustom_AppendArray;
    fn->paramCount = 2;
    fn->params = new Parameter[2];
    fn->returnType = Type_Get<void>();
    Mutable(fn->params[0]) = Parameter("object", self);
    Mutable(fn->params[1]) = Parameter("element", self);
    self->GetFunctions().push(fn);
  }

  /* Clear. */ {
    Function fn = Function_Create("Clear");
    fn->call = ArrayCustom_Clear;
    fn->paramCount = 1;
    fn->params = new Parameter[1];
    fn->returnType = Type_Get<void>();
    Mutable(fn->params[0]) = Parameter("object", self);
    self->GetFunctions().push(fn);
  }

  /* Get. */ {
    Function fn = Function_Create("Get");
    fn->call = ArrayCustom_Get;
    fn->paramCount = 2;
    fn->params = new Parameter[2];
    fn->returnType = elemTypePtr;
    Mutable(fn->params[0]) = Parameter("object", self);
    Mutable(fn->params[1]) = Parameter("index", Type_Get<int>());
    self->GetFunctions().push(fn);
  }

  /* Remove. */ {
    Function fn = Function_Create("Remove");
    fn->call = ArrayCustom_Remove;
    fn->paramCount = 2;
    fn->params = new Parameter[2];
    fn->returnType = Type_Get<void>();
    Mutable(fn->params[0]) = Parameter("object", self);
    Mutable(fn->params[1]) = Parameter("index", Type_Get<int>());
    self->GetFunctions().push(fn);
  }

  /* Size. */ {
    Function fn = Function_Create("Size");
    fn->call = ArrayCustom_Size;
    fn->paramCount = 1;
    fn->params = new Parameter[1];
    fn->returnType = Type_Get<int>();
    Mutable(fn->params[0]) = Parameter("object", self);
    self->GetFunctions().push(fn);
  }

  return self;
}
