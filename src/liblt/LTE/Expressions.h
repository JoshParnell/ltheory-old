#ifndef LTE_Expressions_h__
#define LTE_Expressions_h__

#include "AutoClass.h"
#include "Expression.h"

namespace LTE {
  LT_API Expression Expression_Access(
    Expression const& location,
    uint offset,
    Type const& type,
    String const& name);

  LT_API Expression Expression_Access(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_Address(Expression const& location);

  LT_API Expression Expression_Address(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_Array(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_Assign(
    Expression const& location,
    Expression const& value);

  LT_API Expression Expression_Assign(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_Block(
    Vector<Expression> const& expressions,
    Vector<Type> const& locals);

  LT_API Expression Expression_Block(
    StringList const& list,
    CompileEnvironment& env,
    uint startIndex);

  LT_API Expression Expression_Cast(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_Conversion(
    Expression const& statement,
    Type const& destType);

  LT_API Expression Expression_Conversion(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_ConversionFromData(
    Expression const& statement,
    Type const& destType);

  LT_API Expression Expression_ConversionToData(Expression const& statement);

  LT_API Expression Expression_Constant(Data const& value);

  LT_API Expression Expression_Constant(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_Constructor(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_DeclareLocal(
    Expression const& value,
    String const& name);

  LT_API Expression Expression_DeclareLocal(
    StringList const& list,
    CompileEnvironment& env,
    Vector<String>* locals);

  LT_API Expression Expression_DeclareReference(
    Expression const& value,
    String const& name);

  LT_API Expression Expression_DeclareReference(
    StringList const& list,
    CompileEnvironment& env,
    Vector<String>* locals);

  LT_API Expression Expression_DeclareStatic(
    Expression const& value,
    String const& name);

  LT_API Expression Expression_DeclareStatic(
    StringList const& list,
    CompileEnvironment& env,
    Vector<String>* locals);

  LT_API Expression Expression_Dereference(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_DereferencePointer(Expression const& location);

  LT_API Expression Expression_DereferencePointer(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_DynamicDispatch(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_ExpressionCall(
    ScriptFunction const& function,
    Vector<Expression> const& arguments);

  LT_API Expression Expression_ExpressionCall(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_For(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_Function(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_FunctionCall(
    Function const& function,
    Vector<Expression> const& arguments);

  LT_API Expression Expression_FunctionCall(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_If(
    Expression const& predicate,
    Expression const& statement);

  LT_API Expression Expression_If(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_List(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_Noop();

  LT_API Expression Expression_Print(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_Switch(
    Vector<Expression> const& cases,
    Expression const& defaultExpression);

  LT_API Expression Expression_Switch(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_Type(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_Variable(
    uint index,
    Type const& type,
    String const& name);

  LT_API Expression Expression_Variable(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_Reference(
    uint index,
    Type const& type,
    String const& name);

  LT_API Expression Expression_Reference(
    StringList const& list,
    CompileEnvironment& env);

  LT_API Expression Expression_While(
    Expression const& predicate,
    Expression const& statement);

  LT_API Expression Expression_While(
    StringList const& list,
    CompileEnvironment& env);
}

#endif
