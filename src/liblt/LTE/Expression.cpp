#include "Expression.h"
#include "Environment.h"
#include "Expressions.h"
#include "Map.h"
#include "ProgramLog.h"
#include "Script.h"
#include "Stack.h"
#include "StackFrame.h"
#include "StringList.h"

#include <iostream>
#include "Debug.h"

namespace LTE {
  void ExpressionT::Evaluate(void* returnValue) const {
    AUTO_FRAME;
    Environment env;
    Evaluate(returnValue, env);
  }

  Expression Expression_Compile(
    StringList const& list,
    CompileEnvironment& env,
    Vector<String>* locals)
  {
    SFRAME("Compile Expression");

    if (list->IsAtom()) {
      /* Variable. */ {
        Expression e = Expression_Variable(list, env);
        if (e) return e;
      }

      /* Reference. */ {
        Expression e = Expression_Reference(list, env);
        if (e) return e;
      }

      /* Function call. */ {
        Expression e = Expression_FunctionCall(list, env);
        if (e) return e;
      }

      /* Expression call. */ {
        Expression e = Expression_ExpressionCall(list, env);
        if (e) return e;
      }

      /* Constructor. */ {
        Expression e = Expression_Constructor(list, env);
        if (e) return e;
      }

      /* Constant. */ {
        Expression e = Expression_Constant(list, env);
        if (e) return e;
      }

      return nullptr;
    }

    /* Empty lists are never used, but check for them to prevent crash. */
    if (list->GetSize() == 0)
      return nullptr;

    if (list->Get(0)->IsAtom()) {
      String const& value = list->Get(0)->GetValue();
      if (value == "#")
        return nullptr;
      if (value == "@")
        return Expression_Print(list, env);
      if (value == "address")
        return Expression_Address(list, env);
      if (value == "block")
        return Expression_Block(list, env, 1);
      if (value == "call")
        return Expression_DynamicDispatch(list, env);
      if (value == "cast")
        return Expression_Cast(list, env);
      if (value == "desc")
        return Expression_Block(list, env, 2);
      if (value == "deref")
        return Expression_DereferencePointer(list, env);
      if (value == "for")
        return Expression_For(list, env);
      if (value == "function")
        return Expression_Function(list, env);
      if (value == "if")
        return Expression_If(list, env);
      if (value == "list")
        return Expression_List(list, env);
      if (value == "ref")
        return Expression_DeclareReference(list, env, locals);
      if (value == "set" || value == "=")
        return Expression_Assign(list, env);
      if (value == "static")
        return Expression_DeclareStatic(list, env, locals);
      if (value == "switch" || value == "?")
        return Expression_Switch(list, env);
      if (value == "type")
        return Expression_Type(list, env);
      if (value == "var")
        return Expression_DeclareLocal(list, env, locals);
      if (value == "while")
        return Expression_While(list, env);
    }

    /* Function call. */ {
      Expression e = Expression_FunctionCall(list, env);
      if (e) return e;
    }

    /* Expression call. */ {
      Expression e = Expression_ExpressionCall(list, env);
      if (e) return e;
    }

    /* Field access. */ {
      Expression e = Expression_Access(list, env);
      if (e) return e;
    }

    /* Pointer field access. */ {
      Expression e = Expression_Dereference(list, env);
      if (e) return e;
    }

    /* Constructor. */ {
      Expression e = Expression_Constructor(list, env);
      if (e) return e;
    }

    /* Implicit conversion. */ {
      Expression e = Expression_Conversion(list, env);
      if (e) return e;
    }
    
    if (list->GetSize() == 1)
      return Expression_Compile(list->Get(0), env);

    if (!env.hasErrors) {
      env.hasErrors = true;
      env.detail = true;
      std::cout << "'" << env.script->name << "' -- failed to compile ";
      StringList_Print(list);
      std::cout << std::endl;
      Expression_Compile(list, env, locals);
      env.detail = false;
    }

    return nullptr;
  }

  Expression Expression_Compile(StringList const& list) {
    CompileEnvironment env;
    return Expression_Compile(list, env, nullptr);
  }
}
