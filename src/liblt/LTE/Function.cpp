#include "Function.h"
#include "Data.h"
#include "Map.h"
#include "Pointer.h"
#include "String.h"
#include "Vector.h"

namespace {
  Vector<Function>& GetFunctionList() {
    static Vector<Function> v;
    return v;
  }

  Map<String, Vector<Function> >& GetFunctionMap() {
    static Map<String, Vector<Function> > m;
    return m;
  }

  struct FunctionExtra {
    Data aux;
  };

  struct FunctionImpl : FunctionT {
    FunctionExtra extra;
  };
}

Function Function_Create(String const& name) {
  Reference<FunctionImpl> self = new FunctionImpl;

  self->name = name;
  self->call = 0;
  self->paramCount = 0;
  self->params = 0;
  
  GetFunctionList().push(self);
  GetFunctionMap()[name].push(self);

  return self.t;
}

FunctionT::~FunctionT() {
  ((FunctionImpl*)this)->extra.~FunctionExtra();
  delete[] params;
}

Data& FunctionT::GetAux() {
  return ((FunctionImpl*)this)->extra.aux;
}

String FunctionT::GetSignature() const {
  Stringize s;
  s | returnType->GetAliasName() | " " | name | "(";
  for (uint i = 0; i < paramCount; ++i) {
    if (i) s | ", ";
    s | params[i].type->GetAliasName() | " " | params[i].name;
  }
  s | ")";
  return s;
}

void Function_AddAlias(String const& source, String const& alias) {
  GetFunctionMap()[alias].append(GetFunctionMap()[source]);
}

Vector<Function> const& Function_Find(String const& name) {
  return GetFunctionMap()[name];
}

Vector<Function> const& Function_GetList() {
  return GetFunctionList();
}
