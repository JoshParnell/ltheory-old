#include "../Types.h"

#include "LTE/Function.h"
#include "LTE/Map.h"
#include "LTE/Type.h"

namespace {
  Type& GetCached(Type const& subType) {
    static Map<Type, Type> cache;
    return cache[subType];
  }

  inline void PointerCustom_Map(
    TypeT* type,
    void* ptr,
    FieldMapper& m,
    void* aux)
  {
    void** self = (void**)ptr;
    m(*self, "value", type->GetPointeeType(), aux);
  }
}

Type Type_Pointer(Type const& elemType) {
  Type& self = GetCached(elemType);
  if (self)
    return self;

  self = Type_Create(elemType->name + "*", sizeof(void*));
  self->alignment = AlignOf<void*>();
  self->allocate = __type_default_allocator<void*>;
  self->assign = __type_default_assign<void*>;
  self->construct = __type_default_construct<void*>;
  self->deallocate = __type_default_deallocator<void*>;
  self->destruct = __type_default_destruct<void*>;
  self->mapper = PointerCustom_Map;
  self->pointer = true;
  self->toString = __type_default_tostring<void*>;
  self->GetPointeeType() = elemType;
  return self;
}
