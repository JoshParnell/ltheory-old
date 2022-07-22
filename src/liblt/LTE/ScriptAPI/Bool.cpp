#include "LTE/Function.h"

DefineConversion(int_to_bool, int, bool) {
  dest = (src != 0);
}

DefineConversion(uint_to_bool, int, bool) {
  dest = (src != 0);
}

TypeAlias(bool, Bool);

FreeFunction(bool, Bool_And,
  "Return 'a' && 'b'",
  bool, a,
  bool, b)
{
  return a && b;
} FunctionAlias(Bool_And, &&);

FreeFunction(bool, Bool_Equal,
  "Return 'a' == 'b'",
  bool, a,
  bool, b)
{
  return a == b;
} FunctionAlias(Bool_Equal, ==);

FreeFunction(bool, Bool_Not,
  "Return !'b'",
  bool, b)
{
  return !b;
} FunctionAlias(Bool_Not, !);

FreeFunction(bool, Bool_NotEqual,
  "Return 'a' != 'b'",
  bool, a,
  bool, b)
{
  return a != b;
} FunctionAlias(Bool_NotEqual, !=);

FreeFunction(bool, Bool_Or,
  "Return 'a' || 'b'",
  bool, a,
  bool, b)
{
  return a || b;
} FunctionAlias(Bool_Or, ||);
