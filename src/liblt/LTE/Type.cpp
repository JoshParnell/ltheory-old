#include "Type.h"
#include "Data.h"
#include "Field.h"
#include "HashSet.h"
#include "Function.h"
#include "Map.h"
#include "Pointer.h"
#include "Reference.h"
#include "String.h"
#include "Vector.h"

#include <iostream>

String const kAutoPtrName = "AutoPtr";
String const kHandleName = "Handle";
String const kPointerName = "Pointer";
String const kReferenceName = "Reference";

namespace {
  Vector<Type>& GetTypeList() {
    static Vector<Type> v;
    return v;
  }

  Map<String, Type>& GetTypeMap() {
    static Map<String, Type> m;
    return m;
  }

  struct TypeExtra {
    Data aux;
    Vector<String> aliases;
    Vector<Type> derived;
    Vector<ConversionType> conversions;
    Vector<Field> fields;
    Vector<Function> functions;
    Type pointeeType;
  };

  struct TypeImpl : public TypeT {
    TypeExtra extra;
  };
}

Type Type_Create(String const& name, size_t size) {
  Reference<TypeImpl> self = new TypeImpl;

  static size_t GUIDIndex = 0;
  GetTypeList().push((TypeT*)self.t);
  GetTypeMap()[name] = (TypeT*)self.t;

  self->name = name;
  self->size = size;
  self->alignment = 1;
  self->base = 0;
  self->GUID = GUIDIndex++;
  self->pointer = false;

  self->allocate = 0;
  self->assign = 0;
  self->castInt = 0;
  self->castReal = 0;
  self->construct = 0;
  self->deallocate = 0;
  self->destruct = 0;
  self->mapper = 0;
  self->toString = 0;
  return self.t;
}

TypeT::~TypeT() {
  LTE_ASSERT(refCount == 0);
  ((TypeImpl*)this)->extra.~TypeExtra();
}

Data& TypeT::GetAux() {
  return ((TypeImpl*)this)->extra.aux;
}

Vector<String>& TypeT::GetAliases() {
  return ((TypeImpl*)this)->extra.aliases;
}

Vector<Type>& TypeT::GetDerived() {
  return ((TypeImpl*)this)->extra.derived;
}

Vector<ConversionType>& TypeT::GetConversions() {
  return ((TypeImpl*)this)->extra.conversions;
}

Vector<Field>& TypeT::GetFields() {
  return ((TypeImpl*)this)->extra.fields;
}

Vector<Function>& TypeT::GetFunctions() {
  return ((TypeImpl*)this)->extra.functions;
}

Type& TypeT::GetPointeeType() {
  return ((TypeImpl*)this)->extra.pointeeType;
}

void TypeT::AddConversion(ConversionType const& function) {
  GetConversions().push(function);
}

void TypeT::AddDerived(Type const& type) {
  GetDerived().push(type);
}

void TypeT::AddField(Field const& field) {
  GetFields().push(field);
}

void TypeT::AddFunction(Function const& fn) {
  Mutable(fn->params[0]) = Parameter("object", this);
  GetFunctions().push(fn);
}

FieldType TypeT::FindField(void* base, String const& name) {
  if (!IsComposite())
    return FieldType();
      
  struct Finder : public FieldMapper {
    char const* name;
    FieldType result;

    Finder(char const* name) :
      name(name)
      {}

    void operator()(void* field, char const* name, Type const& type, void* aux) {
      if (result.address)
        return;

      if (strcmp(name, this->name) == 0) {
        result.address = field;
        result.name = name;
        result.type = type;
      }
    }
  };

  Finder f(name);
  mapper(this, base, f, nullptr);
  return f.result;
}

String TypeT::GetAliasName() const {
  Vector<String> const& aliases = ((TypeT*)this)->GetAliases();
  return aliases.size() ? aliases.back() : name;
}

FieldType TypeT::GetField(void* base, size_t index) {
  struct Getter : public FieldMapper {
    size_t index;
    FieldType result;

    Getter(size_t index) :
      index(index)
      {}

    void operator()(void* field, char const* name, Type const& type, void* aux) {
      if (result.address)
        return;

      if (index == 0) {
        result.address = field;
        result.name = name;
        result.type = type;
      } else {
        index--;
      }
    }
  };

  LTE_ASSERT(IsComposite());
  Getter g(index);
  mapper(this, base, g, nullptr);
  return g.result;
}

size_t TypeT::GetFieldCount(void* base) {
  if (!IsComposite())
    return 0;

  struct Getter : public FieldMapper {
    size_t result;

    Getter() :
      result(0)
      {}

    void operator()(void* field, char const* name, Type const& type, void* aux) {
      result++;
    }
  };

  Getter g;
  mapper(this, base, g, nullptr);
  return g.result;
}

bool TypeT::HasField(void* base, String const& name) {
  return FindField(base, name).address != 0;
}

bool TypeT::IsPseudoPointer() const {
  return name == kAutoPtrName
      || name == kHandleName
      || name == kPointerName
      || name == kReferenceName;
}

String TypeT::ToString(void* buffer) {
  String result;
  toString(this, buffer, &result);
  return result;
}

void Type_AddAlias(Type const& type, String const& alias) {
  type->GetAliases().push(alias);
  GetTypeMap()[alias] = type;
}

Type Type_Find(String const& name) {
  return GetTypeMap()[name];
}

Vector<Type> const& Type_GetList() {
  return GetTypeList();
}

void Type_Print(
  void* base,
  Type const& type,
  int indent,
  HashSet<void*>& visited,
  uint maxDepth)
{
  if (!maxDepth)
    return;

  static bool flush = true;

  if (!type) {
    std::cout << "unknown type @ ";
    std::cout << base << std::endl;
    return;
  }

  if (!base) {
    std::cout << "null" << std::endl;
    return;
  }

  ToStream(std::cout, type);
  std::cout << " @ " << base;

  if (visited.contains(base)) {
    std::cout << std::endl;
    return;
  }

  struct Mapper : public FieldMapper {
    void* base;
    int indent;
    HashSet<void*>& visited;
    uint depth;

    Mapper(void* base, int indent, HashSet<void*>& visited, uint depth) :
      base(base),
      indent(indent),
      visited(visited),
      depth(depth)
      {}

    void operator()(
      void* field,
      char const* name,
      Type const& type,
      void* aux)
    {
      for (int i = 0; i < indent + 1; ++i)
        std::cout << "  ";
      std::cout << name << " : ";
      if (field != base)
        visited.insert(base);
      Type_Print(field, type, indent + 1, visited, depth);
    }
  };

  if (type->mapper) {
    std::cout << std::endl;
    flush = true;
    Mapper m(base, indent, visited, maxDepth - 1);
    type->mapper(type, base, m, nullptr);
    if (flush) {
      flush = false;
      std::cout << std::endl;
    }
  } else {
#define X(x) if (type->name == #x)                                             \
    std::cout << " = " << (*(x const*)base);
    PRIMITIVE_TYPE_X
#undef X
    std::cout << std::endl;
  }
}

void Type_Print(void* base, Type const& type, uint maxDepth) {
  HashSet<void*> visited;
  Type_Print(base, type, 0, visited, maxDepth);
}

MAKE_DEFAULT_REFLECTION(Type)

/* CRITICAL. */
Type _Type_Get(TypeT const& t) {
  return nullptr;
}

#define X(x) MAKE_DEFAULT_REFLECTION(x)
PRIMITIVE_TYPE_X
#undef X
