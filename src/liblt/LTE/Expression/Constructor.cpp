#include "../Expressions.h"

#include "LTE/AutoClass.h"
#include "LTE/Environment.h"
#include "LTE/Pool.h"
#include "LTE/ProgramLog.h"
#include "LTE/Script.h"
#include "LTE/StringList.h"

namespace {
  AutoClassDerived(ExpressionConstructorEmpty, ExpressionT,
    Type, type)
    DERIVED_TYPE_EX(ExpressionConstructorEmpty)
    POOLED_TYPE
    
    ExpressionConstructorEmpty() {}

    String Emit(Vector<String>& context) const {
      return Stringize() | type->name | "()";
    }

    void Evaluate(void* returnValue, Environment& env) const {}

    Type GetType() const {
      return type;
    }

    bool IsConstant(CompileEnvironment& env) const {
      return true;
    }
  };

  AutoClassDerived(ExpressionConstructorDefault, ExpressionT,
    Type, type,
    Array<Expression>, expressions)
    DERIVED_TYPE_EX(ExpressionConstructorDefault)
    POOLED_TYPE
    
    ExpressionConstructorDefault() {}

    ExpressionConstructorDefault(
        Type const& type,
        Vector<Expression> const& expressions) :
      type(type)
    {
      this->expressions.resize(expressions.size());
      for (size_t i = 0; i < expressions.size(); ++i)
        this->expressions[i] = expressions[i];
    }

    void Evaluate(void* returnValue, Environment& env) const {
      Vector<Field> const& fields = type->GetFields();
      for (size_t i = 0; i < expressions.size(); ++i)
        expressions[i]->Evaluate((char*)returnValue + fields[i].offset, env);
    }

    Type GetType() const {
      return type;
    }

    bool IsConstant(CompileEnvironment& env) const {
      for (size_t i = 0; i < expressions.size(); ++i)
        if (!expressions[i]->IsConstant(env))
          return false;
      return true;
    }
  };
}

namespace LTE {
  Expression Expression_Constructor(
    StringList const& list,
    CompileEnvironment& env)
  {
    Type type;
    StringList args;

    if (list->IsAtom()) {
      type = env.script->ResolveType(list);
      args = nullptr;
    } else {
      type = env.script->ResolveType(list->Get(0));
      args = list;
      if (!type) {
        type = env.script->ResolveType(list);
        args = nullptr;
      }
    }

    if (!type) {
      if (env.detail)
        Log_Message(Stringize()
          | "constructor -- could not resolve type '" | list->GetString() | "'");
      return nullptr;
    }

    /* TODO : Remove this? */
    if (!args)
      return new ExpressionConstructorEmpty(type);

    /* TODO : Take initializers into consideration. */
    Vector<Expression> initializers;
    initializers.resize(type->GetFields().size(), nullptr);
    if (type->GetAux().IsType<ScriptType>())
      initializers = type->GetAux().Convert<ScriptType>()->initializers;

    size_t emptyInitializers = 0;
    for (size_t i = 0; i < initializers.size(); ++i)
      if (!initializers[i])
        emptyInitializers++;

    if (args->GetSize() == emptyInitializers + 1) {
      size_t fieldIndex = 0;
      for (size_t i = 1; i < args->GetSize(); ++i) {
        while (initializers[fieldIndex] && fieldIndex < initializers.size())
          fieldIndex++;

        Expression e = Expression_Compile(args->Get(i), env);
        if (!e)
          return nullptr;

        e = Expression_Conversion(e, type->GetFields()[fieldIndex].type);
        if (!e)
          return nullptr;

        initializers[fieldIndex] = e;
      }

      return new ExpressionConstructorDefault(type, initializers);
    }

    return nullptr;
  }
}
