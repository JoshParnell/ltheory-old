#include "../Expressions.h"

#include "LTE/AutoClass.h"
#include "LTE/Environment.h"
#include "LTE/Pool.h"
#include "LTE/ProgramLog.h"
#include "LTE/Script.h"
#include "LTE/StackFrame.h"
#include "LTE/StringList.h"
#include "LTE/Vector.h"

#include <algorithm>

namespace {
  AutoClass(ArgData,
    Expression, expression,
    Type, type,
    bool, isLValue)
    ArgData() {}

    ArgData(Expression const& expression) :
      expression(expression),
      type(expression->GetType()),
      isLValue(expression->IsLValue())
      {}
  };

  AutoClassDerived(ExpressionFunctionCall, ExpressionT,
    Function, function,
    Array<ArgData>, args)
    Array<void*> argStack;

    DERIVED_TYPE_EX(ExpressionFunctionCall)
    POOLED_TYPE


    ExpressionFunctionCall() {}

    ExpressionFunctionCall(
        Function const& function,
        Vector<Expression> const& arguments) :
      function(function)
    {
      args.resize(arguments.size());
      argStack.resize(arguments.size(), nullptr);
      for (size_t i = 0; i < arguments.size(); ++i)
        args[i] = ArgData(arguments[i]);
    }

    String Emit(Vector<String>& scope) const {
      Stringize call = Stringize() | function->name | "(";
      for (size_t i = 0; i < args.size(); ++i) {
        if (i) call | ", ";
        call | args[i].expression->Emit(scope);
      }

      call | ")";
      return call;
    }

    void Evaluate(void* returnValue, Environment& env) const {
      // SFRAME(function->name.data());

      for (size_t i = 0; i < args.size(); ++i) {
        ArgData const& arg = args[i];
        if (!arg.isLValue) {
          argStack[i] = env.Allocate(arg.type);
          arg.expression->Evaluate(argStack[i], env);
        } else {
          argStack[i] = arg.expression->GetLValue(env);
        }
      }

      function->call(argStack.data(), returnValue);

      for (size_t i = 0; i < args.size(); ++i) {
        size_t index = args.size() - i - 1;
        ArgData const& arg = args[index];
        if (!arg.isLValue)
          env.Free(arg.type, argStack[index]);
      }
    }

    Type GetType() const {
      return function->returnType;
    }

    bool IsConstant(CompileEnvironment& env) const {
      /* TODO */
      return false;

      /* NOTE : Assumes pure functions! Dangerous! */
      for (size_t i = 0; i < args.size(); ++i)
        if (!args[i].expression->IsConstant(env))
          return false;
      return true;
    }
  };

  AutoClass(FunctionMatch,
    Function, fn,
    uint, order)
    
    FunctionMatch() {}

    friend bool operator<(FunctionMatch const& a, FunctionMatch const& b) {
      return a.order < b.order;
    }
  };

  Function OverloadResolution(
    String const& name,
    Vector<Function> const& candidates,
    Vector<Expression>& expressions)
  {
    Vector<Type> types;
    for (size_t i = 0; i < expressions.size(); ++i)
      types.push(expressions[i]->GetType());

    Vector<FunctionMatch> matches;
    for (size_t i = 0; i < candidates.size(); ++i) {
      Function const& fn = candidates[i];
      if (fn->paramCount != expressions.size())
        continue;

      bool match = true;
      uint order = 0;

      for (size_t j = 0; j < expressions.size(); ++j) {
        if (types[j] != fn->params[j].type) {
          order++;
          if (!Expression_Conversion(expressions[j], fn->params[j].type))
            match = false;
        }
      }

      if (match)
        matches.push(FunctionMatch(fn, order));
    }

    if (matches.size() == 1) {
      Function const& fn = matches[0].fn;
      /* Create implicit conversion. */
      for (size_t i = 0; i < expressions.size(); ++i)
        expressions[i] = Expression_Conversion(expressions[i], fn->params[i].type);
      return fn;
    }

    if (matches.isEmpty()) {
#if 0
      String sig = "(";
      for (size_t i = 0; i < types.size(); ++i) {
        if (i) sig += ", ";
        sig += types[i]->GetAliasName();
      }
      sig += ")";

      Log_Error(Stringize() | "Could not match call to " | name | sig);

      Log_Error("Candidates are:");
      for (size_t i = 0; i < candidates.size(); ++i)
        Log_Error(candidates[i]->GetSignature());
#endif

      return nullptr;
    }

    else {
      std::sort(matches.begin(), matches.end());
      if (matches[1].order > matches[0].order)
        return matches[0].fn;

#if 0
      Log_Error(Stringize()
        | "Ambiguous function call to " | name | " arguments are:");
      for (size_t i = 0; i < types.size(); ++i)
        Log_Error(Stringize() | "  " | types[i]->GetAliasName());

      Log_Error("Matching candidates are:");
      for (size_t i = 0; i < matches.size(); ++i) {
        FunctionMatch const& match = matches[i];
        Log_Error(Stringize()
          | "(With " | match.order | " implicit conversions) "
          | match.fn->GetSignature());
      }
#endif
      return nullptr;
    }
  }
}

namespace LTE {
  Expression Expression_FunctionCall(
    Function const& function,
    Vector<Expression> const& arguments)
  {
    return new ExpressionFunctionCall(function, arguments);
  }

  Expression Expression_FunctionCall(
    StringList const& original,
    CompileEnvironment& env)
  {
    StringList list = original;

    if (!list->IsAtom() &&
        !list->Get(0)->IsAtom() &&
         list->Get(0)->GetSize() == 2)
    {
      list = original->Clone();
      ((StringListList*)list.t)->elements[0] = original->Get(0)->Get(0);
      ((StringListList*)list.t)->elements.insert(1, original->Get(0)->Get(1));
    }

    String const& name = list->IsAtom()
      ? list->GetValue()
      : list->Get(0)->GetValue();

    Vector<Function> candidates = Function_Find(name);
    if (!candidates.size()) {
      if (env.detail)
        Log_Message(Stringize()
          | "functioncall -- no candidate functions '" | name | "'");
      return nullptr;
    }

    Vector<Expression> args;
    for (size_t i = 1; i < list->GetSize(); ++i) {
      Expression e = Expression_Compile(list->Get(i), env); 
      if (!e) {
        if (env.detail)
          Log_Message(Stringize() | "functioncall -- bad argument");
        return nullptr;
      }
      args.push(e);
    }

    Function fn = OverloadResolution(name, candidates, args);
    if (!fn) {
      if (env.detail)
        Log_Message(Stringize()
          | "functioncall -- no overloads of '" | name | "' were compatible with arguments");
      return nullptr;
    }

    return Expression_FunctionCall(fn, args);
  }
}
