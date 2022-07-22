#include "LTE/Expression.h"
#include "LTE/Function.h"
#include "LTE/StringList.h"

TypeAlias(Reference<ExpressionT>, Expression);

FreeFunction(Expression, Expression_Compile,
  "Compile an expression from 'list'",
  StringList, list)
{
  return Expression_Compile(list);
} FunctionAlias(Expression_Compile, Compile);
