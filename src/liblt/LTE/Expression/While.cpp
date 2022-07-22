#include "../Expressions.h"

#include "LTE/AutoClass.h"
#include "LTE/Environment.h"
#include "LTE/Pool.h"
#include "LTE/ProgramLog.h"
#include "LTE/StringList.h"

namespace {
  AutoClassDerived(ExpressionWhile, ExpressionT,
    Expression, predicate,
    Expression, statement,
    Type, statementType)
    DERIVED_TYPE_EX(ExpressionWhile)
    POOLED_TYPE

    ExpressionWhile() {}

    String Emit(Vector<String>& context) const {
      /* Predicate. */ {
        Vector<String> predContext;
        String predCall = predicate->Emit(predContext);
        
        context.push("while (1) {");
        for (size_t i = 0; i < predContext.size(); ++i)
          context.push("  " + predContext[i]);
        context.push("  if (!" + predCall + ") break;");
      }

      /* Statement. */ {
        Vector<String> statementContext;
        statement->Emit(statementContext);
        for (size_t i = 0; i < statementContext.size(); ++i)
          context.push("  " + statementContext[i]);
        context.push("}");
      }
      return "";
    }

    void Evaluate(void* returnValue, Environment& env) const {
      bool pred;
      while (true) {
        predicate->Evaluate(&pred, env);
        if (pred) {
          if (statementType->allocate) {
            void* lv = env.Allocate(statementType);
            statement->Evaluate(lv, env);
            env.Free(statementType, lv);
          } else {
            statement->Evaluate(0, env);
          }
        } else
          break;
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
  Expression Expression_While(
    Expression const& predicate,
    Expression const& statement)
  {
    return new ExpressionWhile(predicate, statement, statement->GetType());
  }

  Expression Expression_While(
    StringList const& list,
    CompileEnvironment& env)
  {
    if (list->GetSize() < 3) {
      if (env.detail)
        Log_Message(Stringize() | "'while' -- list does not have 2 arguments");
      return nullptr;
    }

    Expression predicate = Expression_Compile(list->Get(1), env);
    if (!predicate) {
      if (env.detail)
        Log_Message(Stringize() | "'while' -- arg 1 (predicate) did not compile");
      return nullptr;
    }

    predicate = Expression_Conversion(predicate, Type_Get<bool>());
    if (!predicate) {
      if (env.detail)
        Log_Message(Stringize()
          | "'while' -- arg 1 (predicate) could not be converted to type 'bool'");
      return nullptr;
    }

    Expression statement = Expression_Block(list, env, 2);
    if (!statement) {
      if (env.detail)
        Log_Message(Stringize() | "'while' -- statement failed to compile");
      return nullptr;
    }

    return Expression_While(predicate, statement);
  }
}
