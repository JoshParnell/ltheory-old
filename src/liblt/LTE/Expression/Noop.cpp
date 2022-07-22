#include "../Expressions.h"

#include "LTE/AutoClass.h"
#include "LTE/Environment.h"
#include "LTE/Pool.h"

namespace {
  AutoClassDerivedEmpty(ExpressionNoop, ExpressionT)
    DERIVED_TYPE_EX(ExpressionNoop)
    POOLED_TYPE

    ExpressionNoop() {}

    String Emit(Vector<String>& context) const {
      return "";
    }

    void Evaluate(void* returnValue, Environment& env) const {}

    Type GetType() const {
      return Type_Get<void>();
    }

    bool IsConstant(CompileEnvironment& env) const {
      return true;
    }
  };
}

namespace LTE {
  Expression Expression_Noop() {
    return new ExpressionNoop;
  }
}
