#include "../Expressions.h"

#include "LTE/Array.h"
#include "LTE/AutoClass.h"
#include "LTE/Environment.h"
#include "LTE/Pool.h"
#include "LTE/ProgramLog.h"
#include "LTE/StringList.h"
#include "LTE/Vector.h"

#include <iostream>

namespace {
  AutoClassDerived(ExpressionPrint, ExpressionT,
    Array<Expression>, expressions,
    Array<Type>, types)
    DERIVED_TYPE_EX(ExpressionPrint)
    POOLED_TYPE

    ExpressionPrint() {}

    ExpressionPrint(Vector<Expression> const& expressions) {
      this->expressions.resize(expressions.size());
      this->types.resize(expressions.size());
      for (size_t i = 0; i < expressions.size(); ++i) {
        this->expressions[i] = expressions[i];
        this->types[i] = expressions[i]->GetType();
      }
    }

    void Evaluate(void* returnValue, Environment& env) const {
      for (size_t i = 0; i < expressions.size(); ++i) {
        void* lv = expressions[i]->GetLValue(env);
        if (lv)
          std::cout << types[i]->ToString(lv);
        else {
          lv = env.Allocate(types[i]);
          expressions[i]->Evaluate(lv, env);
          std::cout << types[i]->ToString(lv);
          env.Free(types[i], lv);
        }
      }
      std::cout << std::endl;
    }

    Type GetType() const {
      return Type_Get<void>();
    }

    bool IsConstant(CompileEnvironment& env) const {
      return false;
    }
  };
}

namespace LTE {
  Expression Expression_Print(
    StringList const& list,
    CompileEnvironment& env)
  {
    Vector<Expression> expressions;
    for (size_t i = 1; i < list->GetSize(); ++i) {
      Expression e = Expression_Compile(list->Get(i), env);
      if (!e) {
        Log_Message("print -- bad argument");
        return nullptr;
      }

      expressions.push(e);

      Type type = e->GetType();
      if (!type->toString) {
        Log_Message(Stringize()
          | "print -- type '" | type->name | "' does not support printing");
        return nullptr;
      }
    }

    return expressions.size() ? new ExpressionPrint(expressions) : nullptr;
  }
}
