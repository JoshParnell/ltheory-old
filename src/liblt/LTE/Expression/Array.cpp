#include "../Expressions.h"

#include "LTE/AutoClass.h"
#include "LTE/Environment.h"
#include "LTE/Pool.h"
#include "LTE/ProgramLog.h"
#include "LTE/StringList.h"

namespace {
  AutoClassDerived(ExpressionArray, ExpressionT,
    Type, type)
    DERIVED_TYPE_EX(ExpressionArray)
    POOLED_TYPE

    ExpressionArray() {}

    void Evaluate(void* returnValue, Environment& env) const {
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
  Expression Expression_Array(
    StringList const& list,
    CompileEnvironment& env)
  {
    if (list->GetSize() != 2) {
      if (env.detail)
        Log_Message("array -- list does not have 1 argument");
      return nullptr;
    }

    Type subType;
    Type subTypePtr;
    return nullptr;
  }
}
