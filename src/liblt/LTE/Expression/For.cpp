#include "../Expressions.h"

#include "LTE/Environment.h"
#include "LTE/ProgramLog.h"
#include "LTE/StringList.h"

namespace LTE {
  Expression Expression_For(StringList const& list, CompileEnvironment& env) {
    if (list->GetSize() < 6) {
      Log_Error(Stringize()
        | "'for' expects at least 5 arguments, but got "
        | (list->GetSize() - 1));
      return nullptr;
    }

    Expression value = Expression_Compile(list->Get(2), env);
    if (!value) return nullptr;

    String const& valueName = list->Get(1)->GetValue();
    Type valueType = value->GetType();
    env.Allocate(valueName, valueType, value->IsConstant(env), false);

    Vector<Expression> expressions;
    expressions.push(Expression_DeclareLocal(value, valueName));

    Expression predicate = Expression_Compile(list->Get(3), env);
    if (!predicate) return nullptr;

    predicate = Expression_Conversion(predicate, Type_Get<bool>());
    if (!predicate) return nullptr;

    /* Compile inner block + operation. */ {
      Vector<Expression> innerExpressions;
      Vector<String> localNames;
      for (size_t i = 5; i < list->GetSize(); ++i) {
        Expression e = Expression_Compile(list->Get(i), env, &localNames);
        if (e)
          innerExpressions.push(e);
      }

      /* Operation. */ {
        Expression operation = Expression_Compile(list->Get(4), env);
        if (!operation)
          return nullptr;
        innerExpressions.push(operation);
      }

      Vector<Type> locals;
      for (size_t i = 0; i < localNames.size(); ++i) {
        String const& name = localNames[i];
        Variable const& var = env.Get(name);
        locals.push(var.reference ? nullptr : var.type);
        env.Free(name);
      }

      Expression innerBlock = Expression_Block(innerExpressions, locals);
      expressions.push(Expression_While(predicate, innerBlock));
    }

    env.Free(valueName);

    return Expression_Block(expressions, Vector<Type>(valueType));
  }
}
