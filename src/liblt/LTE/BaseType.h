#ifndef LTE_BaseType_h__
#define LTE_BaseType_h__

#include "String.h"
#include "Type.h"

template <class BaseT>
void __MapBase(TypeT* type, void* address, FieldMapper& m, void* aux) {
  if (address) {
    BaseT* self = (BaseT*)address;
    Type derivedType = self->GetDerivedTypeInfo();
    m(self, derivedType->name, derivedType, aux);
  } else {
    BaseT::MapFields(type, address, m, aux);
  }
}

#define BASE_TYPE_EX(x)                                                        \
  typedef x SelfType;                                                          \
  typedef x VirtualBaseType;                                                   \
                                                                               \
  friend Type _Type_Get(x const& t) {                                          \
    Type& type = Type_GetStorage<x>();                                         \
    if (!type) {                                                               \
      type = Type_Create(#x, sizeof(x));                                       \
      type->deallocate = &__type_default_deallocator<x>;                       \
      type->destruct = &__type_default_destruct<x>;                            \
      type->mapper = &__MapBase<x>;                                            \
      FillMetadata(type);                                                      \
    }                                                                          \
    return type;                                                               \
  }                                                                            \
                                                                               \
  template <class StreamT>                                                     \
  friend void _ToStream(StreamT& s, x const& t) {                              \
    s << t.ToString();                                                         \
  }                                                                            \
                                                                               \
  virtual Type GetDerivedTypeInfo() const = 0;                                 \
                                                                               \
  virtual int GetTypeGUID() const = 0;                                         \
                                                                               \
  virtual String ToString() const {                                            \
    return ToStringDerived();                                                  \
  }                                                                            \
                                                                               \
  virtual String ToStringDerived() const = 0;

#define BASE_TYPE(x)                                                           \
  virtual ~x() {}                                                              \
  BASE_TYPE_EX(x)

#define DERIVED_TYPE_EX(x)                                                     \
  static Type _GetDerivedTypeInfo() {                                          \
    Type& type = Type_GetStorage<x>();                                         \
    if (!type) {                                                               \
      type = Type_Create(#x, sizeof(x));                                       \
      type->allocate = &__type_default_allocator<x>;                           \
      type->base = Type_Get<typename x::VirtualBaseType>();                    \
      type->construct = &__type_default_construct<x>;                          \
      type->deallocate = &__type_default_deallocator<x>;                       \
      type->destruct = &__type_default_destruct<x>;                            \
      type->mapper = &x::MapFields;                                            \
      FillMetadata(type);                                                      \
      type->base->AddDerived(type);                                            \
    }                                                                          \
    return type;                                                               \
  }                                                                            \
                                                                               \
  Type GetDerivedTypeInfo() const {                                            \
    return _GetDerivedTypeInfo();                                              \
  }                                                                            \
                                                                               \
  int GetTypeGUID() const {                                                    \
    return _GetDerivedTypeInfo()->GUID;                                        \
  }                                                                            \
                                                                               \
  String ToStringDerived() const {                                             \
    return Stringize() | *this;                                                \
  }

#define DERIVED_TYPE(x, y)                                                     \
  typedef y BaseType;                                                          \
  x() {}                                                                       \
  DERIVED_TYPE_EX(x)

#define DERIVED_IMPLEMENT(x)                                                   \
  static Type __##x##_registration =                                           \
    x::_GetDerivedTypeInfo();                                                  \
                                                                               \
  inline Type __get_##x##_registration() {                                     \
    return __##x##_registration;                                               \
  }

#endif
