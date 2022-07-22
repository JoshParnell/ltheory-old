#include "../Expressions.h"

#include "LTE/AutoClass.h"
#include "LTE/Environment.h"
#include "LTE/Pool.h"
#include "LTE/ProgramLog.h"
#include "LTE/Script.h"
#include "LTE/StringList.h"
#include "LTE/Vector.h"

#include "LTE/Debug.h"

namespace {
  ConversionFn FindConversion(Type const& src, Type const& dst) {
    Vector<ConversionType> const& conversions = src->GetConversions();
    for (size_t i = 0; i < conversions.size(); ++i)
      if (conversions[i].other == dst)
        return conversions[i].fn;
    return nullptr;
  }

  AutoClassDerived(ExpressionConversion, ExpressionT,
    Expression, statement)
    DERIVED_TYPE_EX(ExpressionConversion)
    POOLED_TYPE

    Type srcType;
    Type dstType;
    ConversionFn function;

    ExpressionConversion() {}
    
    ExpressionConversion(Expression const& statement, Type const& dstType) :
      statement(statement),
      srcType(statement->GetType()),
      dstType(dstType),
      function(FindConversion(srcType, dstType))
      {}

    void Evaluate(void* returnValue, Environment& env) const {
      void* source = env.Allocate(srcType);
      statement->Evaluate(source, env);
      function(srcType, source, returnValue);
      env.Free(srcType, source);
    }

    Type GetType() const {
      return dstType;
    }

    bool IsConstant(CompileEnvironment& env) const {
      return statement->IsConstant(env);
    }
  };

  AutoClassDerived(ExpressionConversionFromData, ExpressionT,
    Expression, statement,
    Type, dstType)
    DERIVED_TYPE_EX(ExpressionConversionFromData)
    POOLED_TYPE

    ExpressionConversionFromData() {}

    void Evaluate(void* returnValue, Environment& env) const {
      Data source;
      statement->Evaluate(&source, env);
      dstType->Assign(source.data, returnValue);
    }

    void* GetLValue(Environment& env) const {
      return ((Data*)statement->GetLValue(env))->data;
    }

    Type GetType() const {
      return dstType;
    }

    bool IsConstant(CompileEnvironment& env) const {
      return statement->IsConstant(env);
    }

    bool IsLValue() const {
      return statement->IsLValue();
    }
  };

  AutoClassDerived(ExpressionConversionToData, ExpressionT,
    Expression, statement,
    Type, srcType)
    DERIVED_TYPE_EX(ExpressionConversionToData)
    POOLED_TYPE

    ExpressionConversionToData() {}

    void Evaluate(void* returnValue, Environment& env) const {
      void* src = env.Allocate(srcType);
      statement->Evaluate(src, env);
      ((Data*)returnValue)->Construct(srcType);
      srcType->Assign(src, ((Data*)returnValue)->data);
      env.Free(srcType, src);
    }

    Type GetType() const {
      return Type_Get<Data>();
    }

    bool IsConstant(CompileEnvironment& env) const {
      return statement->IsConstant(env);
    }
  };
}

namespace LTE {
  Expression Expression_Conversion(
    Expression const& statement,
    Type const& dst)
  {
    Type src = statement->GetType();
    if (src == dst)
      return statement;
    if (src == Type_Get<Data>())
      return Expression_ConversionFromData(statement, dst);
    if (dst == Type_Get<Data>())
      return Expression_ConversionToData(statement);
    if (FindConversion(src, dst))
      return new ExpressionConversion(statement, dst);
    return nullptr;
  }

  Expression Expression_ConversionFromData(
    Expression const& statement,
    Type const& dstType)
  {
    return new ExpressionConversionFromData(statement, dstType);
  }

  Expression Expression_ConversionToData(Expression const& statement) {
    return new ExpressionConversionToData(statement, statement->GetType());
  }

  Expression Expression_Conversion(
    StringList const& list,
    CompileEnvironment& env)
  {
    if (list->GetSize() != 2)
      return nullptr;

    Type type = env.script->ResolveType(list->Get(0));
    if (!type)
      return nullptr;

    Expression e = Expression_Compile(list->Get(1), env);
    if (!e)
      return nullptr;
    return Expression_Conversion(e, type);
  }
}
