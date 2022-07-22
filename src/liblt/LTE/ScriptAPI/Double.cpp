#include "LTE/Function.h"
#include "LTE/Math.h"

TypeAlias(double, Double);

DefineConversion(float_to_double, float, double) {
  dest = (double)src;
}

#if 0
DefineConversion(double_to_int, double, int) {
  dest = (int)src;
}

FreeFunction(double, Double_Abs,
  "Return the absolute value of 't'",
  double, t)
{
  return Abs(t);
} FunctionAlias(Double_Abs, Abs);

FreeFunction(double, Double_Add,
  "Return the sum of 'a' and 'b'",
  double, a,
  double, b)
{
  return a + b;
} FunctionAlias(Double_Add, +);

VoidFreeFunction(Double_AddInPlace,
  "Add 'b' to 'a'",
  double, a,
  double, b)
{
  (double&)a += b;
} FunctionAlias(Double_AddInPlace, +=);

FreeFunction(double, Double_Atan,
  "Return the quadrant-correct of 'y' / 'x'",
  double, y,
  double, x)
{
  return Atan(y, x);
} FunctionAlias(Double_Atan, Atan);

FreeFunction(double, Double_Ceil,
  "Return 'f' rounded up to the nearest integer",
  double, f)
{
  return Ceil(f);
} FunctionAlias(Double_Ceil, Ceil);

FreeFunction(double, Double_Clamp,
  "Return 'f' clamped to the range [lower, upper]",
  double, f,
  double, lower,
  double, upper)
{
  return Clamp(f, lower, upper);
} FunctionAlias(Double_Clamp, Clamp);

FreeFunction(double, Double_Cos,
  "Return the cosine of 'angle' (radians)",
  double, angle)
{
  return Cos(angle);
} FunctionAlias(Double_Cos, Cos);

FreeFunction(double, Double_Divide,
  "Return the dividend of 'a' and 'b'",
  double, a,
  double, b)
{
  return a / b;
} FunctionAlias(Double_Divide, /);

VoidFreeFunction(Double_DivideInPlace,
  "Divide 'b' by 'a'",
  double, a,
  double, b)
{
  (double&)a /= b;
} FunctionAlias(Double_DivideInPlace, /=);

FreeFunction(double, Double_Exp, "Return the e raised to the 't' power",
  double, t)
{
  return Exp(t);
} FunctionAlias(Double_Exp, Exp);

FreeFunction(double, Double_ExpDecay,
  "Return the e raised to the '-t / rate' power",
  double, t,
  double, rate)
{
  return Exp(-t / rate);
} FunctionAlias(Double_ExpDecay, ExpDecay);

FreeFunction(double, Double_Fract, "Return the fractional part of 't'",
  double, t)
{
  return Fract(t);
} FunctionAlias(Double_Fract, Fract);

FreeFunction(double, Double_Double, "Construct a double from 'f'", double, f) {
  return f;
} FunctionAlias(Double_Double, Double);

FreeFunction(double, Double_Int, "Convert 'i' into a double", int, i) {
  return (double)i;
} FunctionAlias(Double_Int, Double);

FreeFunction(double, Double_Floor,
  "Return 'f' rounded down to the nearest integer",
  double, f)
{
  return Floor(f);
} FunctionAlias(Double_Floor, Floor);

FreeFunction(bool, Double_Greater, "Return a > b", double, a, double, b) {
  return a > b;
} FunctionAlias(Double_Greater, >);

FreeFunction(bool, Double_GreaterEqual, "Return a >= b", double, a, double, b) {
  return a >= b;
} FunctionAlias(Double_GreaterEqual, >=);

FreeFunction(bool, Double_Less, "Return a < b", double, a, double, b) {
  return a < b;
} FunctionAlias(Double_Less, <);

FreeFunction(bool, Double_LessEqual, "Return a <= b", double, a, double, b) {
  return a <= b;
} FunctionAlias(Double_LessEqual, <=);

FreeFunction(double, Double_Max, "Return the maximum of 'a' and 'b'",
  double, a,
  double, b)
{
  return Max(a, b);
} FunctionAlias(Double_Max, Max);

FreeFunction(double, Double_Mix,
  "Return a linear interpolation 'a' and 'b' with interpolant 't'",
  double, a,
  double, b,
  double, t)
{
  return Mix(a, b, t);
} FunctionAlias(Double_Mix, Mix);

FreeFunction(double, Double_Log, "Return the logarithm of 'f'", double, f) {
  return Log(f);
} FunctionAlias(Double_Log, Log);

FreeFunction(double, Double_Min,
  "Return the minimum of 'a' and 'b'",
  double, a,
  double, b)
{
  return Min(a, b);
} FunctionAlias(Double_Min, Min);

FreeFunction(double, Double_Mod, "Return the remaind of 'a' / 'b'",
  double, a,
  double, b)
{
  return Mod(a, b);
} FunctionAlias(Double_Mod, Mod);

FreeFunction(double, Double_Mult,
  "Return the product of 'a' and 'b'",
  double, a,
  double, b)
{
  return a * b;
} FunctionAlias(Double_Mult, *);

VoidFreeFunction(Double_MultInPlace,
  "Multiply 'a' by 'b'",
  double, a,
  double, b)
{
  (double&)a *= b;
} FunctionAlias(Double_MultInPlace, *=);

FreeFunction(double, Double_Pow,
  "Return 'a' raised to the 'b'",
  double, a,
  double, b)
{
  return Pow(a, b);
} FunctionAlias(Double_Pow, ^);

FreeFunction(double, Double_Pow2,
  "Return f to the 2nd power",
  double, f)
{
  return f * f;
} FunctionAlias(Double_Pow2, Pow2);

FreeFunction(double, Double_Pow4,
  "Return f to the 4th power",
  double, f)
{
  double s = f * f;
  return s * s;
} FunctionAlias(Double_Pow4, Pow4);

FreeFunctionNoParams(double, Double_Random,
  "Return a random between 0 (inclusive) and 1 (exclusive)")
{
  return Rand();
}

FreeFunction(double, Double_Round,
  "Return 'f' rounded to the nearest integer",
  double, f)
{
  return Round(f);
} FunctionAlias(Double_Round, Round);

FreeFunction(double, Double_Saturate,
  "Return 'f' clamped to the range [0, 1]",
  double, f)
{
  return Saturate(f);
} FunctionAlias(Double_Saturate, Saturate);

FreeFunction(double, Double_Sign,
  "Return the sign (-1, 0, or +1) of 'f'",
  double, f)
{
  return Sign(f);
} FunctionAlias(Double_Sign, Sign);

FreeFunction(double, Double_Sin,
  "Return the sine of 'angle' (radians)",
  double, angle)
{
  return Sin(angle);
} FunctionAlias(Double_Sin, Sin);

FreeFunction(double, Double_Sqrt,
  "Return the square root of 'f'",
  double, f)
{
  return Sqrt(f);
} FunctionAlias(Double_Sqrt, Sqrt);

FreeFunction(double, Double_Subtract,
  "Return the difference of 'a' and 'b'",
  double, a,
  double, b)
{
  return a - b;
} FunctionAlias(Double_Subtract, -);

VoidFreeFunction(Double_SubtractInPlace,
  "Subtract 'b' from 'a'",
  double, a,
  double, b)
{
  (double&)a -= b;
} FunctionAlias(Double_SubtractInPlace, -=);

FreeFunction(double, Double_Tan,
  "Return the tangent of 'angle' (radians)",
  double, angle)
{
  return Tan(angle);
} FunctionAlias(Double_Tan, Tan);
#endif
