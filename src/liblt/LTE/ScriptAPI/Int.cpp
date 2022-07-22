#include "LTE/Function.h"
#include "LTE/Math.h"

TypeAlias(signed int, int);
TypeAlias(signed int, Int);
TypeAlias(unsigned int, uint);
TypeAlias(unsigned int, Uint);
TypeAlias(int32, Int32);
TypeAlias(uint32, Uint32);
TypeAlias(int64, Int64);
TypeAlias(uint64, Uint64);

DefineConversion(int_to_double, int, double) {
  dest = (double)src;
}

DefineConversion(int_to_float, int, float) {
  dest = (float)src;
}

DefineConversion(int_to_uint, int, unsigned int) {
  dest = (unsigned int)src;
}

DefineConversion(uint_to_int, unsigned int, int) {
  dest = (int)src;
}

DefineConversion(int_to_long, int, long) {
  dest = (long)src;
}

DefineConversion(int_to_llong, int, long long) {
  dest = (long long)src;
}

DefineConversion(int32_to_int, int32, int) {
  dest = (int)src;
}

DefineConversion(int64_to_int, int64, int) {
  dest = (int)src;
}

FreeFunction(int, Int_Abs,
  "Return the absolute value of 'i'",
  int, i)
{
  return Abs(i);
} FunctionAlias(Int_Abs, Abs);

FreeFunction(int, Int_Add,
  "Return the sum of 'a' and 'b'",
  int, a,
  int, b)
{
  return a + b;
} FunctionAlias(Int_Add, +);

VoidFreeFunction(Int_AddInPlace,
  "Add 'b' to 'a'",
  int, a,
  int, b)
{
  (int&)a += b;
} FunctionAlias(Int_AddInPlace, +=);

FreeFunction(int, Int_Clamp,
  "Return 'i' clamped to the range [a, b]",
  int, i,
  int, a,
  int, b)
{
  return Clamp(i, a, b);
} FunctionAlias(Int_Clamp, Clamp);

VoidFreeFunction(Int_Decrement,
  "Decrement the value of 'i' by 1",
  int, i)
{
  ((int&)i)--;
} FunctionAlias(Int_Decrement, --);

FreeFunction(int, Int_Divide,
  "Return 'a' divided by 'b' (as an int)",
  int, a,
  int, b)
{
  return a / b;
}

FreeFunction(float, Int_DivideFloat,
  "Return 'a' divided by 'b' (as a float)",
  int, a,
  int, b)
{
  return (float)((double)a / b);
} FunctionAlias(Int_DivideFloat, /);

FreeFunction(bool, Int_Equal,
  "Return whether 'a' is equal to 'b'",
  int, a,
  int, b)
{
  return a == b;
} FunctionAlias(Int_Equal, ==);

FreeFunction(bool, Int_Greater,
  "Return whether 'a' is greater than 'b'",
  int, a,
  int, b)
{
  return a > b;
} FunctionAlias(Int_Greater, >);

FreeFunction(bool, Int_GreaterEqual,
  "Return whether 'a' is greater than or equal to 'b'",
  int, a,
  int, b)
{
  return a >= b;
} FunctionAlias(Int_GreaterEqual, >=);

VoidFreeFunction(Int_Increment,
  "Increment the value of 'i' by 1",
  int, i)
{
  ((int&)i)++;
} FunctionAlias(Int_Increment, ++);

FreeFunction(bool, Int_Less,
  "Return whether 'a' is less than 'b'",
  int, a,
  int, b)
{
  return a < b;
} FunctionAlias(Int_Less, <);

FreeFunction(bool, Int_LessEqual,
  "Return whether 'a' is less than or equal to 'b'",
  int, a,
  int, b)
{
  return a <= b;
} FunctionAlias(Int_LessEqual, <=);

FreeFunction(int, Int_Max,
  "Return the max of 'a' and 'b'",
  int, a,
  int, b)
{
  return Max(a, b);
} FunctionAlias(Int_Max, Max);

FreeFunction(int, Int_Min,
  "Return the min of 'a' and 'b'",
  int, a,
  int, b)
{
  return Min(a, b);
} FunctionAlias(Int_Min, Min);

FreeFunction(int, Int_Mod,
  "Return a modulo b",
  int, a,
  int, b)
{
  return a % b;
} FunctionAlias(Int_Mod, Mod);

FreeFunction(int, Int_Mult,
  "Return the product of 'a' and 'b'",
  int, a,
  int, b)
{
  return a * b;
} FunctionAlias(Int_Mult, *);

VoidFreeFunction(Int_MultInPlace,
  "Multiply 'a' by 'b'",
  int, a,
  int, b)
{
  (int&)a *= b;
} FunctionAlias(Int_MultInPlace, *=);

FreeFunction(bool, Int_NotEqual,
  "Return whether 'a' is not equal to 'b'",
  int, a,
  int, b)
{
  return a != b;
} FunctionAlias(Int_NotEqual, !=);

namespace Priv1 {
  FreeFunctionNoParams(int, Int_Random,
    "Return a random integer between 0 and the maximal integer value")
  {
    return Rand32();
  }
}

namespace Priv2 {
  FreeFunction(int, Int_Random,
    "Return a random integer from 0 to 'upper' - 1",
    int, upper)
  {
    return RandI(upper);
  }
}

namespace Priv3 {
  FreeFunction(int, Int_Random,
    "Return a random integer from 'lower' to 'upper'",
    int, lower,
    int, upper)
  {
    return RandI(lower, upper);
  }
}

FreeFunction(int, Int_Sign,
  "Return the sign of 'i'",
  int, i)
{
  return i > 0 ? 1 : i == 0 ? 0 : -1;
} FunctionAlias(Int_Sign, Sign);

FreeFunction(int, Int_Subtract,
  "Return the difference of 'a' and 'b'",
  int, a,
  int, b)
{
  return a - b;
} FunctionAlias(Int_Subtract, -);

VoidFreeFunction(Int_SubtractInPlace,
  "Subtract 'b' from 'a'",
  int, a,
  int, b)
{
  (int&)a -= b;
} FunctionAlias(Int_SubtractInPlace, -=);
