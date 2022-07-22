/* TODO : Unify V2, V3, V4 APIs */

#include "LTE/Data.h"
#include "LTE/Function.h"
#include "LTE/V4.h"

TypeAlias(V4F, Vec4);
TypeAlias(V4F, Vec4f);
TypeAlias(V4D, Vec4d);

DefineConversion(V4F_to_V4D, V4F, V4D) {
  dest = V4D(src);
}

DefineConversion(float_to_V4F, float, V4F) {
  dest = V4F(src);
}

DefineConversion(int_to_V4F, int, V4F) {
  dest = V4F(src);
}

FreeFunction(V4, Vec4,
  "Create a 4D vector ('x', 'y', 'z', 'w')",
  float, x,
  float, y,
  float, z,
  float, w)
{
  return V4(x, y, z, w);
}

FreeFunction(V4F, Vec4f_Abs,
  "Return the component-wise absolute value of 'v'",
  V4F, v)
{
  return Abs(v);
} FunctionAlias(Vec4f_Abs, Abs);

FreeFunction(V4D, Vec4d_Abs,
  "Return the component-wise absolute value of 'v'",
  V4D, v)
{
  return Abs(v);
} FunctionAlias(Vec4d_Abs, Abs);

FreeFunction(V4F, Vec4f_Add,
  "Return the sum of 'a' and 'b'",
  V4F, a,
  V4F, b)
{
  return a + b;
} FunctionAlias(Vec4f_Add, +);

VoidFreeFunction(Vec4f_AddInPlace,
  "Add 'b' to 'a'",
  V4F, a,
  V4F, b)
{
  Mutable(a) += b;
} FunctionAlias(Vec4f_AddInPlace, +=);

FreeFunction(V4D, Vec4d_Add,
  "Return the sum of 'a' and 'b'",
  V4D, a,
  V4D, b)
{
  return a + b;
} FunctionAlias(Vec4d_Add, +);

VoidFreeFunction(Vec4d_AddInPlace,
  "Add 'b' to 'a'",
  V4D, a,
  V4D, b)
{
  Mutable(a) += b;
} FunctionAlias(Vec4d_AddInPlace, +=);

FreeFunction(V4, Vec4_Clamp,
  "Return the component-wise clamp of 'v' and ['lower', 'upper']",
  V4, v,
  V4, lower,
  V4, upper)
{
  return Clamp(v, lower, upper);
} FunctionAlias(Vec4_Clamp, Clamp);

FreeFunction(float, Vec4_Distance,
  "Return the distance between a 'a' and 'b'",
  V4, a,
  V4, b)
{
  return Length(a - b);
} FunctionAlias(Vec4_Distance, Distance);

FreeFunction(float, Vec4f_Dot,
  "Return the dot product of 'a' and 'b'",
  V4F, a,
  V4F, b)
{
  return Dot(a, b);
} FunctionAlias(Vec4_Dot, Dot);

FreeFunction(double, Vec4d_Dot,
  "Return the dot product of 'a' and 'b'",
  V4D, a,
  V4D, b)
{
  return Dot(a, b);
} FunctionAlias(Vec4d_Dot, Dot);


FreeFunction(V4, Vec4_Floor,
  "Return the component-wise floor of 'v'",
  V4, v)
{
  return Floor(v);
} FunctionAlias(Vec4_Floor, Floor);

FreeFunction(float, Vec4f_Length,
  "Return the length of 'v'",
  V4F, v)
{
  return Length(v);
} FunctionAlias(Vec4f_Length, Length);

FreeFunction(double, Vec4d_Length,
  "Return the length of 'v'",
  V4D, v)
{
  return Length(v);
} FunctionAlias(Vec4d_Length, Length);

FreeFunction(V4, Vec4_Max,
  "Return the component-wise max of 'a' and 'b'",
  V4, a,
  V4, b)
{
  return Max(a, b);
} FunctionAlias(Vec4_Max, Max);

FreeFunction(V4, Vec4_Min,
  "Return the component-wise min of 'a' and 'b'",
  V4, a,
  V4, b)
{
  return Min(a, b);
} FunctionAlias(Vec4_Min, Min);

FreeFunction(V4, Vec4_Mix,
  "Return a linear interpolation of 'a' and 'b' with interpolant 't'",
  V4, a,
  V4, b,
  float, t)
{
  return Mix(a, b, t);
} FunctionAlias(Vec4_Mix, Mix);

namespace Priv1 {
  FreeFunction(V4F, Vec4f_Mult,
    "Return the product of 's' and 'vec'",
    float, s,
    V4F, v)
  {
    return s * v;
  }

  FreeFunction(V4D, Vec4d_Mult,
    "Return the product of 's' and 'vec'",
    double, s,
    V4D, v)
  {
    return s * v;
  }
}

namespace Priv2 {
  FreeFunction(V4F, Vec4f_Mult, "Return the product of 'a' and 'b'",
    V4F, a,
    V4F, b)
  {
    return a * b;
  }

  FreeFunction(V4D, Vec4d_Mult, "Return the product of 'a' and 'b'",
    V4D, a,
    V4D, b)
  {
    return a * b;
  }
}

FunctionAlias(Vec4f_Mult, *);
FunctionAlias(Vec4d_Mult, *);

FreeFunction(V4, Vec4f_Normalize,
  "Return a unit-length vector pointing in the same direction as 'vec'",
  V4F, v)
{
  return Normalize(v);
} FunctionAlias(Vec4f_Normalize, Normalize);

FreeFunction(V4, Vec4d_Normalize,
  "Return a unit-length vector pointing in the same direction as 'vec'",
  V4D, v)
{
  return Normalize(v);
} FunctionAlias(Vec4d_Normalize, Normalize);

FreeFunction(V4F, Vec4f_Pow,
  "Return the component-wise absolute value of 'v'",
  V4F, v,
  V4F, p)
{
  return Pow(v, p);
} FunctionAlias(Vec4f_Pow, ^);

FreeFunction(V4, Vec4f_Subtract,
  "Return the difference of 'a' and 'b'",
  V4F, a,
  V4F, b)
{
  return a - b;
} FunctionAlias(Vec4f_Subtract, -);

VoidFreeFunction(Vec4f_SubtractInPlace,
  "Subtract 'b' from 'a'",
  V4F, a,
  V4F, b)
{
  Mutable(a) -= b;
} FunctionAlias(Vec4f_SubtractInPlace, -=);

FreeFunction(V4D, Vec4d_Subtract,
  "Return the difference of 'a' and 'b'",
  V4D, a,
  V4D, b)
{
  return a - b;
} FunctionAlias(Vec4d_Subtract, -);

VoidFreeFunction(Vec4d_SubtractInPlace,
  "Subtract 'b' from 'a'",
  V4D, a,
  V4D, b)
{
  Mutable(a) -= b;
} FunctionAlias(Vec4d_SubtractInPlace, -=);

