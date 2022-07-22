
#include "../Expressions.h"

#include "LTE/AutoClass.h"
#include "LTE/Environment.h"
#include "LTE/Pool.h"
#include "LTE/ProgramLog.h"
#include "LTE/StringList.h"
#include "LTE/Types.h"

namespace {
  AutoClassDerived(ExpressionAddress, ExpressionT,
    Expression, location,
    Type, type)
    DERIVED_TYPE_EX(ExpressionAddress)
    POOLED_TYPE

    ExpressionAddress() {}

    ExpressionAddress(Expression const& location) :
      location(location),
      type(Type_Pointer(location->GetType()))
      {}

    String Emit(Vector<String>& scope) const {
      String label = location->Emit(scope);
      return "(&" + label + ")";
    }

    void Evaluate(void* returnValue, Environment& env) const {
      *(void**)returnValue = location->GetLValue(env);
    }

    Type GetType() const {
      return type;
    }

    bool IsConstant(CompileEnvironment& env) const {
      return false;
    }
  };
}

namespace LTE {
  Expression Expression_Address(Expression const& location) {
    return new ExpressionAddress(location);
  }

  Expression Expression_Address(
    StringList const& list,
    CompileEnvironment& env)
  {
    if (list->GetSize() != 2) {
      Log_Message("address -- list does not have 2 arguments");
      return nullptr;
    }

    Expression location = Expression_Compile(list->Get(1), env);
    if (!location) {
      Log_Message("address -- arg 1 (location) did not compile");
      return nullptr;
    }

    if (!location->IsLValue()) {
      Log_Message("address -- location is not an l-value");
      return nullptr;
    }

    return Expression_Address(location);
  }
}
