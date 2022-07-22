#include "../Expressions.h"

#include "LTE/AutoClass.h"
#include "LTE/Environment.h"
#include "LTE/Pool.h"
#include "LTE/ProgramLog.h"
#include "LTE/StringList.h"

namespace {
  AutoClassDerived(ExpressionDereference, ExpressionT,
    Expression, location,
    Type, type,
    Type, pointeeType)
    DERIVED_TYPE_EX(ExpressionDereference)
    POOLED_TYPE

    ExpressionDereference() {}

    String Emit(Vector<String>& scope) const {
      String label = location->Emit(scope);
      return "(*" + label + ")";
    }

    void Evaluate(void* returnValue, Environment& env) const {
      void* lv = location->GetLValue(env);
      if (lv) {
        pointeeType->Assign(*(void**)lv, returnValue);
      } else {
        lv = env.Allocate(type);
        location->Evaluate(lv, env);
        pointeeType->Assign(*(void**)lv, returnValue);
        env.Free(type, lv);
      }
    }

    void* GetLValue(Environment& env) const {
      void* lv = location->GetLValue(env);
      return lv ?  *(void**)lv : nullptr;
    }

    Type GetType() const {
      return pointeeType;
    }

    bool IsConstant(CompileEnvironment& env) const {
      return false;
    }

    bool IsLValue() const {
      return location->IsLValue();
    }
  };

  AutoClassDerived(ExpressionDereferencePointer, ExpressionT,
    Expression, location,
    Type, type)
    DERIVED_TYPE_EX(ExpressionDereferencePointer)
    POOLED_TYPE

    ExpressionDereferencePointer() {}

    ExpressionDereferencePointer(Expression const& location) :
      location(location),
      type(location->GetType()->GetPointeeType())
      {}

    String Emit(Vector<String>& scope) const {
      String label = location->Emit(scope);
      return "(*" + label + ")";
    }

    void Evaluate(void* returnValue, Environment& env) const {
      void* value;
      location->Evaluate(&value, env);
      type->Assign(value, returnValue);
    }

    void* GetLValue(Environment& env) const {
      void* value;
      location->Evaluate(&value, env);
      return value;
    }

    Type GetType() const {
      return type;
    }

    bool IsConstant(CompileEnvironment& env) const {
      return false;
    }

    bool IsLValue() const {
      return true;
    }
  };
}

namespace LTE {
  Expression Expression_Dereference(
    StringList const& list,
    CompileEnvironment& env)
  {
    if (list->GetSize() != 2) {
      if (env.detail)
        Log_Message("dereference -- list does not have 2 arguments");
      return nullptr;
    }

    Expression location = Expression_Compile(list->Get(1), env);
    if (!location) {
      if (env.detail)
        Log_Message("dereference -- arg 1 (location) did not compile");
      return nullptr;
    }

    Type const& type = location->GetType();
    if (!type->GetPointeeType()) {
      if (env.detail)
        Log_Message(Stringize()
          | "dereference -- location type '" | type->name
          | "' is not a pointer");
      return nullptr;
    }

    Type const& pointeeType = type->GetPointeeType();
    /* TODO : Recursive dereferencing. */
    if (pointeeType->GetPointeeType()) {
      if (env.detail)
        Log_Message(Stringize()
          | "dereference -- location type '" | type->name
          | "' is a pointer-to-pointer type, this is not yet supported");
      return nullptr;
    }

    String const& fieldName = list->Get(0)->GetValue();
    FieldType field = pointeeType->FindField(0, fieldName);
    if (!field.type) {
      if (env.detail)
        Log_Message(Stringize()
          | "derefrence -- location type '" | type->name
          | "' has no field '" | fieldName | "'");
      return nullptr;
    }

    return Expression_Access(
      new ExpressionDereference(location, type, pointeeType),
      (size_t)field.address, field.type, fieldName);
  }

  Expression Expression_DereferencePointer(Expression const& location) {
    return new ExpressionDereferencePointer(location);
  }

  Expression Expression_DereferencePointer(
    StringList const& list,
    CompileEnvironment& env)
  {
    if (list->GetSize() != 2) {
      Log_Message("dereferencepointer -- list does not have 2 arguments");
      return nullptr;
    }

    Expression location = Expression_Compile(list->Get(1), env);
    if (!location) {
      Log_Message("dereferencepointer -- arg 1 (location) did not compile");
      return nullptr;
    }

    Type const& type = location->GetType();
    if (!type->GetPointeeType()) {
      Log_Message(Stringize()
        | "dereferencepointer -- location type '" | type->name
        | "' is not a pointer");
      return nullptr;
    }

    return Expression_DereferencePointer(location);
  }
}
