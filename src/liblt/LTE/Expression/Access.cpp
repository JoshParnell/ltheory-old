#include "../Expressions.h"

#include "LTE/AutoClass.h"
#include "LTE/Environment.h"
#include "LTE/Pool.h"
#include "LTE/ProgramLog.h"
#include "LTE/StringList.h"

namespace {
  AutoClassDerived(ExpressionAccess, ExpressionT,
    Expression, location,
    size_t, offset,
    Type, type,
    Type, subType,
    String, name)
    DERIVED_TYPE_EX(ExpressionAccess)
    POOLED_TYPE

    ExpressionAccess() {}

    String Emit(Vector<String>& scope) const {
      return name;
    }

    void Evaluate(void* returnValue, Environment& env) const {
      void* lv = location->GetLValue(env);
      if (lv)
        subType->Assign((char const*)lv + offset, returnValue);
      else {
        lv = env.Allocate(type);
        location->Evaluate(lv, env);
        subType->Assign((char const*)lv + offset, returnValue);
        env.Free(type, lv);
      }
    }

    void* GetLValue(Environment& env) const {
      void* lv = location->GetLValue(env);
      return lv ? (char*)lv + offset : nullptr;
    }

    Type GetType() const {
      return subType;
    }

    bool IsConstant(CompileEnvironment& env) const {
      /* TODO. */
      return false;
    }

    bool IsLValue() const {
      return location->IsLValue();
    }
  };
}

namespace LTE {
  Expression Expression_Access(
    Expression const& location,
    uint offset,
    Type const& subType,
    String const& name)
  {
    return new ExpressionAccess(location, offset, location->GetType(), subType, name);
  }

  Expression Expression_Access(
    StringList const& list,
    CompileEnvironment& env)
  {
    if (list->GetSize() != 2) {
      if (env.detail)
        Log_Message("access -- list does not have 1 argument");
      return nullptr;
    }

    Expression location = Expression_Compile(list->Get(1), env);
    if (!location) {
      if (env.detail)
        Log_Message("access -- arg 1 (location) did not compile");
      return nullptr;
    }

    Type const& type = location->GetType();
    if (type->GetPointeeType()) {
      if (env.detail)
        Log_Message("access -- arg 1 (location) is a pointer");
      return nullptr;
    }

    String const& fieldName = list->Get(0)->GetValue();
    FieldType field = type->FindField(0, fieldName);
    if (!field.type) {
      if (env.detail)
        Log_Message(Stringize()
          | "access -- location type '" | type->name
          | "' has no field '" | fieldName | "'");
      return nullptr;
    }

    size_t offset = (size_t)field.address;
    return Expression_Access(location, offset, field.type, fieldName);
  }
}
