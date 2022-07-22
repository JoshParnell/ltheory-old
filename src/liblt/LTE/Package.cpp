#include "Package.h"
#include "Function.h"
#include "Map.h"
#include "Type.h"

namespace {
  struct PackageExtra {
    String name;
    Map<String, Function> functions;
    Map<String, Type> types;
  };

  struct PackageImpl : public PackageT {
    PackageExtra extra;
  };
}

PackageT::~PackageT() {
  ((PackageImpl*)this)->extra.~PackageExtra();
}

void PackageT::AddFunction(Function const& function) {
  ((PackageImpl*)this)->extra.functions[function->name] = function;
}

void PackageT::AddType(Type const& type) {
  ((PackageImpl*)this)->extra.types[type->name] = type;
}

Function PackageT::GetFunction(String const& name) const {
  Function const* fn = ((PackageImpl*)this)->extra.functions.get(name);
  return fn ? *fn : nullptr;
}

Type PackageT::GetType(String const& name) const {
  Type const* type = ((PackageImpl*)this)->extra.types.get(name);
  return type ? *type : nullptr;
}

Package Package_Create(String const& name) {
  PackageImpl* self = new PackageImpl;
  self->extra.name = name;
  return (PackageT*)self;
}

Package Package_Global() {
  static Package package = Package_Create("Global");
  return package;
}
