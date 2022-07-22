#include "LTE/Function.h"
#include "LTE/Math.h"

TypeAlias(float, Float);

DefineConversion(double_to_float, double, float) {
  dest = (float)src;
}

DefineConversion(float_to_int, float, int) {
  dest = (int)src;
}

FreeFunction(float, Float_Abs,
  "Return the absolute value of 't'",
  float, t)
{
  return Abs(t);
} FunctionAlias(Float_Abs, Abs);

FreeFunction(float, Float_Add,
  "Return the sum of 'a' and 'b'",
  float, a,
  float, b)
{
  return a + b;
} FunctionAlias(Float_Add, +);

VoidFreeFunction(Float_AddInPlace,
  "Add 'b' to 'a'",
  float, a,
  float, b)
{
  (float&)a += b;
} FunctionAlias(Float_AddInPlace, +=);

FreeFunction(float, Float_Atan,
  "Return the quadrant-correct atan of 'y' / 'x'",
  float, y,
  float, x)
{
  return Atan(y, x);
} FunctionAlias(Float_Atan, Atan);

FreeFunction(float, Float_Ceil,
  "Return 'f' rounded up to the nearest integer",
  float, f)
{
  return Ceil(f);
} FunctionAlias(Float_Ceil, Ceil);

FreeFunction(float, Float_Clamp,
  "Return 'f' clamped to the range [lower, upper]",
  float, f,
  float, lower,
  float, upper)
{
  return Clamp(f, lower, upper);
} FunctionAlias(Float_Clamp, Clamp);

FreeFunction(float, Float_Cos,
  "Return the cosine of 'angle' (radians)",
  float, angle)
{
  return Cos(angle);
} FunctionAlias(Float_Cos, Cos);

FreeFunction(float, Float_Divide,
  "Return the dividend of 'a' and 'b'",
  float, a,
  float, b)
{
  return a / b;
} FunctionAlias(Float_Divide, /);

VoidFreeFunction(Float_DivideInPlace,
  "Divide 'b' by 'a'",
  float, a,
  float, b)
{
  (float&)a /= b;
} FunctionAlias(Float_DivideInPlace, /=);

FreeFunction(float, Float_Exp, "Return the e raised to the 't' power",
  float, t)
{
  return Exp(t);
} FunctionAlias(Float_Exp, Exp);

FreeFunction(float, Float_ExpDecay,
  "Return the e raised to the '-t / rate' power",
  float, t,
  float, rate)
{
  return Exp(-t / rate);
} FunctionAlias(Float_ExpDecay, ExpDecay);

FreeFunction(float, Float_Fract, "Return the fractional part of 't'",
  float, t)
{
  return Fract(t);
} FunctionAlias(Float_Fract, Fract);

FreeFunction(float, Float_Float, "Construct a float from 'f'", float, f) {
  return f;
} FunctionAlias(Float_Float, Float);

FreeFunction(float, Float_Int, "Convert 'i' into a float", int, i) {
  return (float)i;
} FunctionAlias(Float_Int, Float);

FreeFunction(float, Float_Floor,
  "Return 'f' rounded down to the nearest integer",
  float, f)
{
  return Floor(f);
} FunctionAlias(Float_Floor, Floor);

FreeFunction(bool, Float_Greater, "Return a > b", float, a, float, b) {
  return a > b;
} FunctionAlias(Float_Greater, >);

FreeFunction(bool, Float_GreaterEqual, "Return a >= b", float, a, float, b) {
  return a >= b;
} FunctionAlias(Float_GreaterEqual, >=);

FreeFunction(bool, Float_Less, "Return a < b", float, a, float, b) {
  return a < b;
} FunctionAlias(Float_Less, <);

FreeFunction(bool, Float_LessEqual, "Return a <= b", float, a, float, b) {
  return a <= b;
} FunctionAlias(Float_LessEqual, <=);

FreeFunction(float, Float_Max, "Return the maximum of 'a' and 'b'",
  float, a,
  float, b)
{
  return Max(a, b);
} FunctionAlias(Float_Max, Max);

FreeFunction(float, Float_Mix,
  "Return a linear interpolation 'a' and 'b' with interpolant 't'",
  float, a,
  float, b,
  float, t)
{
  return Mix(a, b, t);
} FunctionAlias(Float_Mix, Mix);

FreeFunction(float, Float_Log, "Return the logarithm of 'f'", float, f) {
  return Log(f);
} FunctionAlias(Float_Log, Log);

FreeFunction(float, Float_Min,
  "Return the minimum of 'a' and 'b'",
  float, a,
  float, b)
{
  return Min(a, b);
} FunctionAlias(Float_Min, Min);

FreeFunction(float, Float_Mod, "Return the remaind of 'a' / 'b'",
  float, a,
  float, b)
{
  return Mod(a, b);
} FunctionAlias(Float_Mod, Mod);

FreeFunction(float, Float_Mult,
  "Return the product of 'a' and 'b'",
  float, a,
  float, b)
{
  return a * b;
} FunctionAlias(Float_Mult, *);

VoidFreeFunction(Float_MultInPlace,
  "Multiply 'a' by 'b'",
  float, a,
  float, b)
{
  (float&)a *= b;
} FunctionAlias(Float_MultInPlace, *=);

FreeFunctionNoParams(float, Float_2Pi, "Return the value of 2 * pi") {
  return kTau;
} FunctionAlias(Float_2Pi, 2Pi);

FreeFunctionNoParams(float, Float_Pi, "Return the value of pi") {
  return kPi;
} FunctionAlias(Float_Pi, Pi);

FreeFunctionNoParams(float, Float_Pi2, "Return the value of pi / 2") {
  return kPi2;
} FunctionAlias(Float_Pi2, Pi2);

FreeFunctionNoParams(float, Float_Pi4, "Return the value of pi / 4") {
  return kPi4;
} FunctionAlias(Float_Pi4, Pi4);

FreeFunctionNoParams(float, Float_Pi6, "Return the value of pi / 6") {
  return kPi6;
} FunctionAlias(Float_Pi6, Pi6);

FreeFunction(float, Float_Pow,
  "Return 'a' raised to the 'b'",
  float, a,
  float, b)
{
  return Pow(a, b);
} FunctionAlias(Float_Pow, ^);

FreeFunction(float, Float_Pow2,
  "Return f to the 2nd power",
  float, f)
{
  return f * f;
} FunctionAlias(Float_Pow2, Pow2);

FreeFunction(float, Float_Pow4,
  "Return f to the 4th power",
  float, f)
{
  float s = f * f;
  return s * s;
} FunctionAlias(Float_Pow4, Pow4);

FreeFunctionNoParams(float, Float_Random,
  "Return a random between 0 (inclusive) and 1 (exclusive)")
{
  return Rand();
}

FreeFunction(float, Float_Round,
  "Return 'f' rounded to the nearest integer",
  float, f)
{
  return Round(f);
} FunctionAlias(Float_Round, Round);

FreeFunction(float, Float_Saturate,
  "Return 'f' clamped to the range [0, 1]",
  float, f)
{
  return Saturate(f);
} FunctionAlias(Float_Saturate, Saturate);

FreeFunction(float, Float_Sign,
  "Return the sign (-1, 0, or +1) of 'f'",
  float, f)
{
  return Sign(f);
} FunctionAlias(Float_Sign, Sign);

FreeFunction(float, Float_Sin,
  "Return the sine of 'angle' (radians)",
  float, angle)
{
  return Sin(angle);
} FunctionAlias(Float_Sin, Sin);

FreeFunction(float, Float_Sqrt,
  "Return the square root of 'f'",
  float, f)
{
  return Sqrt(f);
} FunctionAlias(Float_Sqrt, Sqrt);

FreeFunction(float, Float_Subtract,
  "Return the difference of 'a' and 'b'",
  float, a,
  float, b)
{
  return a - b;
} FunctionAlias(Float_Subtract, -);

VoidFreeFunction(Float_SubtractInPlace,
  "Subtract 'b' from 'a'",
  float, a,
  float, b)
{
  (float&)a -= b;
} FunctionAlias(Float_SubtractInPlace, -=);

FreeFunction(float, Float_Tan,
  "Return the tangent of 'angle' (radians)",
  float, angle)
{
  return Tan(angle);
} FunctionAlias(Float_Tan, Tan);
