#include "../Expressions.h"

#include "LTE/AutoClass.h"
#include "LTE/Environment.h"
#include "LTE/Pool.h"
#include "LTE/ProgramLog.h"
#include "LTE/StringList.h"
#include "LTE/Vector.h"

/* TODO : Split into homogeneous and heterogeneous for performance. */
namespace {
  AutoClass(SwitchCase,
    Expression, predicate,
    Expression, statement,
    Type, type)

    SwitchCase() {}
  };

  AutoClassDerived(ExpressionSwitch, ExpressionT,
    Vector<SwitchCase>, cases,
    Expression, defaultExpression,
    Type, returnType)
    DERIVED_TYPE_EX(ExpressionSwitch)
    POOLED_TYPE

    ExpressionSwitch() {}

    ExpressionSwitch(
        Vector<Expression> const& expressions,
        Expression const& defaultExpression) :
      defaultExpression(defaultExpression)
    {
      for (size_t i = 0; i < expressions.size() / 2; ++i)
        cases.push(SwitchCase(
          expressions[2*i + 0],
          expressions[2*i + 1],
          expressions[2*i + 1]->GetType()));
      returnType = GetType();
    }

    void Evaluate(void* returnValue, Environment& env) const {
      bool pred;
      for (size_t i = 0; i < cases.size(); ++i) {
        SwitchCase const& thisCase = cases[i];
        thisCase.predicate->Evaluate(&pred, env);
        if (pred) {
          if (thisCase.type != returnType && thisCase.type->allocate) {
            void* lv = env.Allocate(thisCase.type);
            thisCase.statement->Evaluate(lv, env);
            env.Free(thisCase.type, lv);
          } else {
            thisCase.statement->Evaluate(returnValue, env);
          }
          return;
        }
      }

      if (defaultExpression)
        defaultExpression->Evaluate(returnValue, env);
    }

    Type GetType() const {
      if (!defaultExpression)
        return Type_Get<void>();

      Type type = cases[0].type;
      for (size_t i = 1; i < cases.size(); ++i)
        if (type != cases[i].type)
          return Type_Get<void>();

      return type == defaultExpression->GetType()
        ? type : Type_Get<void>();
    }

    bool IsConstant(CompileEnvironment& env) const {
      if (defaultExpression && !defaultExpression->IsConstant(env))
        return false;
      for (size_t i = 0; i < cases.size(); ++i)
        if (!cases[i].predicate->IsConstant(env) ||
            !cases[i].statement->IsConstant(env))
          return false;
      return true;
    }
  };
}

namespace LTE {
  Expression Expression_Switch(
    Vector<Expression> const& cases,
    Expression const& defaultExpression)
  {
    return new ExpressionSwitch(cases, defaultExpression);
  }

  Expression Expression_Switch(
    StringList const& list,
    CompileEnvironment& env)
  {
    Vector<Expression> expressions;
    Expression defaultExpression;

    for (size_t i = 1; i < list->GetSize(); ++i) {
      StringList const& sub = list->Get(i);
      String const& value = sub->Get(0)->GetValue();
      
      if (value == "otherwise") {
        if (defaultExpression) {
          Log_Message("switch -- already has 'otherwise' statement");
          return nullptr;
        }

        defaultExpression = Expression_Block(sub, env, 1);
      } else {
        if (sub->GetSize() < 2) {
          Log_Message("switch -- case list does not have at least 2 arguments");
          continue;
        }

        Expression predicate = Expression_Compile(sub->Get(0), env);
        if (!predicate) {
          Log_Message("switch -- case predicate did not compile");
          continue;
        }

        Expression statement = Expression_Block(sub, env, 1);
        if (!statement) {
          Log_Message("switch -- case statement did not compile");
          continue;
        }

        expressions.push(predicate);
        expressions.push(statement);
      }
    }

    if (!expressions.size()) {
      Log_Message("switch -- cond has no inner expressions");
      return nullptr;
    }

    return Expression_Switch(expressions, defaultExpression);
  }
}
