#include "../Expressions.h"

#include "LTE/AutoClass.h"
#include "LTE/Environment.h"
#include "LTE/Pool.h"
#include "LTE/ProgramLog.h"
#include "LTE/StringList.h"

namespace {
  AutoClassDerived(ExpressionAssign, ExpressionT,
    Expression, location,
    Expression, value)
    DERIVED_TYPE_EX(ExpressionAssign)
    POOLED_TYPE

    ExpressionAssign() {}

    String Emit(Vector<String>& context) const {
      String locationCall = location->Emit(context);
      String valueCall = value->Emit(context);
      context.push(Stringize()
        | locationCall | " = "
        | valueCall | ";");
      return "";
    }

    void Evaluate(void* returnValue, Environment& env) const {
      value->Evaluate(location->GetLValue(env), env);
    }

    Type GetType() const {
      return Type_Get<void>();
    }

    bool IsConstant(CompileEnvironment&) const {
      return true;
    }
  };
}

namespace LTE {
  Expression Expression_Assign(
    Expression const& location,
    Expression const& value)
  {
    return new ExpressionAssign(location, value);
  }

  Expression Expression_Assign(
    StringList const& list,
    CompileEnvironment& env)
  {
    if (list->GetSize() != 3) {
      Log_Error(Stringize()
        | "'set' expects 2 arguments, but got "
        | (list->GetSize() - 1));
      return nullptr;
    }

    Expression location = Expression_Compile(list->Get(1), env);
    if (!location) return nullptr;

    Expression value = Expression_Compile(list->Get(2), env);
    if (!value) return nullptr;

    Type locationType = location->GetType();
    Type valueType = value->GetType();

    value = Expression_Conversion(value, location->GetType());
    if (!value) {
      Log_Error(Stringize()
        | "Assigning to location of type '" | locationType->name
        | "', with a value of type '" | valueType->name | "'");
      return nullptr;
    }

    /* TODO : Propagate non-constance from value to location. */
    return Expression_Assign(location, value);
  }
}
