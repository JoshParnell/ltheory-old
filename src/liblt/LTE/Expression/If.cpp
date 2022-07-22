#include "../Expressions.h"

#include "LTE/AutoClass.h"
#include "LTE/Environment.h"
#include "LTE/Pool.h"
#include "LTE/ProgramLog.h"
#include "LTE/StringList.h"

namespace {
  AutoClassDerived(ExpressionIf, ExpressionT,
    Expression, predicate,
    Expression, statement,
    Type, statementType)
    DERIVED_TYPE_EX(ExpressionIf)
    POOLED_TYPE

    ExpressionIf() {}

    String Emit(Vector<String>& context) const {
      String predValue = predicate->Emit(context);
      context.push(Stringize() | "if (" | predValue | ") {");

      Vector<String> localContext;
      statement->Emit(localContext);
      for (size_t i = 0; i < localContext.size(); ++i)
        context.push("  " + localContext[i]);
      context.push("}");

      return "";
    }

    void Evaluate(void* returnValue, Environment& env) const {
      bool pred;
      predicate->Evaluate(&pred, env);
      if (pred) {
        if (statementType->allocate) {
          void* lv = env.Allocate(statementType);
          statement->Evaluate(lv, env);
          env.Free(statementType, lv);
        } else {
          statement->Evaluate(0, env);
        }
      }
    }

    Type GetType() const {
      return Type_Get<void>();
    }

    bool IsConstant(CompileEnvironment& env) const {
      return predicate->IsConstant(env) && statement->IsConstant(env);
    }
  };
}

namespace LTE {
  Expression Expression_If(
    Expression const& predicate,
    Expression const& statement)
  {
    return new ExpressionIf(predicate, statement, statement->GetType());
  }

  Expression Expression_If(StringList const& list, CompileEnvironment& env) {
    if (list->GetSize() < 3) {
      if (env.detail)
        Log_Message("if -- list does not have at least 2 arguments");
      return nullptr;
    }

    Expression predicate = Expression_Compile(list->Get(1), env);
    if (!predicate) {
      if (env.detail)
        Log_Message("if -- predicate did not compile");
      return nullptr;
    }

    predicate = Expression_Conversion(predicate, Type_Get<bool>());
    if (!predicate) {
      if (env.detail)
        Log_Message("if -- predicate could not be converted to bool");
      return nullptr;
    }

    Expression statement = Expression_Block(list, env, 2);
    if (!statement) {
      if (env.detail)
        Log_Message("if -- statement did not compile");
      return nullptr;
    }

    return Expression_If(predicate, statement);
  }
}
