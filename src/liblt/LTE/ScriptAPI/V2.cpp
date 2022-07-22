#include "LTE/Function.h"
#include "LTE/V2.h"

DefineConversion(float_to_V2F, float, V2F) {
  dest = V2F(src);
}

DefineConversion(int_to_V2F, int, V2F) {
  dest = V2F(src);
}

TypeAlias(V2, Vec2);

FreeFunction(V2, Vec2,
  "Create a 2D vector ('x', 'y')",
  float, x,
  float, y)
{
  return V2(x, y);
}

FreeFunction(float, Vec3_Distance,
  "Return the distance between a 'a' and 'b'",
  V2, a,
  V2, b)
{
  return Length(a - b);
} FunctionAlias(Vec2_Distance, Distance);

FreeFunction(V2, Vec2_Float,
  "Create a 2D vector ('f', 'f')",
  float, f)
{
  return V2(f, f);
} FunctionAlias(Vec2_Float, Vec2);

FreeFunction(V2, Vec2_Abs,
  "Return the component-wise absolute value of 'v'",
  V2, v)
{
  return Abs(v);
} FunctionAlias(Vec2_Abs, Abs);

FreeFunction(V2, Vec2_Add,
  "Return the sum of 'a' and 'b'",
  V2, a,
  V2, b)
{
  return a + b;
} FunctionAlias(Vec2_Add, +);

VoidFreeFunction(Vec2_AddInPlace,
  "Add 'b' to 'a'",
  V2, a,
  V2, b)
{
  Mutable(a) += b;
} FunctionAlias(Vec2_AddInPlace, +=);

FreeFunction(V2, Vec2_Clamp,
  "Return the component-wise clamp of 'v' and ['lower', 'upper']",
  V2, v,
  V2, lower,
  V2, upper)
{
  return Clamp(v, lower, upper);
} FunctionAlias(Vec2_Clamp, Clamp);

FreeFunction(float, Vec2_Dot,
  "Return the dot product of 'a' and 'b'",
  V2, a,
  V2, b)
{
  return Dot(a, b);
} FunctionAlias(Vec2_Dot, Dot);

FreeFunction(V2, Vec2_Floor,
  "Return the component-wise floor of 'v'",
  V2, v)
{
  return Floor(v);
} FunctionAlias(Vec2_Floor, Floor);

FreeFunction(bool, Vec2_Greater,
  "Return whether each component of 'a' is greater than the component of 'b'",
  V2, a,
  V2, b)
{
  return a > b;
} FunctionAlias(Vec2_Greater, >);

FreeFunction(bool, Vec2_GreaterEqual,
  "Return whether each component of 'a' is greater or equal to the component of 'b'",
  V2, a,
  V2, b)
{
  return a >= b;
} FunctionAlias(Vec2_GreaterEqual, >=);

FreeFunction(float, Vec2_Length,
  "Return the length of 'v'",
  V2, v)
{
  return Length(v);
} FunctionAlias(Vec2_Length, Length);

FreeFunction(bool, Vec2_Less,
  "Return whether each component of 'a' is less than the component of 'b'",
  V2, a,
  V2, b)
{
  return a < b;
} FunctionAlias(Vec2_Less, <);

FreeFunction(bool, Vec2_LessEqual,
  "Return whether each component of 'a' is less or equal to the component of 'b'",
  V2, a,
  V2, b)
{
  return a <= b;
} FunctionAlias(Vec2_LessEqual, <=);

FreeFunction(V2, Vec2_Max,
  "Return the component-wise max of 'a' and 'b'",
  V2, a,
  V2, b)
{
  return Max(a, b);
} FunctionAlias(Vec2_Max, Max);

FreeFunction(V2, Vec2_Min,
  "Return the component-wise min of 'a' and 'b'",
  V2, a,
  V2, b)
{
  return Min(a, b);
} FunctionAlias(Vec2_Min, Min);

FreeFunction(float, Vec2_MinComponent,
  "Return the minimum component of 'v'",
  V2, v)
{
  return Min(v.x, v.y);
} FunctionAlias(Vec2_MinComponent, MinComponent);

namespace Priv1 {
  FreeFunction(V2, Vec2_Divide,
    "Return the 'vec' divided by 'f'",
    V2, v,
    float, f)
  {
    return v / f;
  }

  VoidFreeFunction(Vec2_DivideInPlace,
    "Divide 'v' by 'f'",
    V2, v,
    float, f)
  {
    Mutable(v) /= f;
  }

  FreeFunction(V2, Vec2_Mix,
    "Return the component-wise linear interpolation of 'a' and 'b' with interpolant 't'",
    V2, a,
    V2, b,
    float, t)
  {
    return Mix(a, b, t);
  }

  FreeFunction(V2, Vec2_Mult,
    "Return the product of 'f' and 'v'",
    float, f,
    V2, v)
  {
    return f * v;
  }

  VoidFreeFunction(Vec2_MultInPlace,
    "Multiply 'v' by 'f'",
    V2, v,
    float, f)
  {
    Mutable(v) *= f;
  }
}

namespace Priv2 {
  FreeFunction(V2, Vec2_Divide,
    "Return the dividend of 'a' and 'b'",
    V2, a,
    V2, b)
  {
    return a / b;
  }

  VoidFreeFunction(Vec2_DivideInPlace,
    "Divide 'a' by 'b'",
    V2, a,
    V2, b)
  {
    Mutable(a) /= b;
  }

  FreeFunction(V2, Vec2_Mix,
    "Return the component-wise linear interpolation of 'a' and 'b' with interpolant 'v'",
    V2, a,
    V2, b,
    V2, v)
  {
    return Mix(a, b, v);
  } 

  FreeFunction(V2, Vec2_Mult,
    "Return the product of 'a' and 'b'",
    V2, a,
    V2, b)
  {
    return a * b;
  }

  VoidFreeFunction(Vec2_MultInPlace,
    "Multiply 'a' by 'b'",
    V2, a,
    V2, b)
  {
    Mutable(a) *= b;
  }
}

FunctionAlias(Vec2_Divide, /);
FunctionAlias(Vec2_DivideInPlace, /=);
FunctionAlias(Vec2_Mix, Mix);
FunctionAlias(Vec2_Mult, *);
FunctionAlias(Vec2_MultInPlace, *=);

FreeFunction(V2, Vec2_Normalize,
  "Return a unit-length vector pointing in the same direction as 'vec'",
  V2, v)
{
  return Normalize(v);
} FunctionAlias(Vec2_Normalize, Normalize);

FreeFunction(V2, Vec2_Round,
  "Return the component-wise round of 'v'",
  V2, v)
{
  return Round(v);
} FunctionAlias(Vec2_Round, Round);

FreeFunction(V2, Vec2_Polar,
  "Return the cartesian coordinates of the unit vector with 'angle'",
  float, angle)
{
  return V2(Cos(angle), Sin(angle));
} FunctionAlias(Vec2_Polar, Polar);

FreeFunction(V2, Vec2_Sign,
  "Return the component-wise sign of 'v'",
  V2, v)
{
  return Sign(v);
} FunctionAlias(Vec2_Sign, Sign);

FreeFunction(V2, Vec2_Subtract,
  "Return the difference of 'a' and 'b'",
  V2, a,
  V2, b)
{
  return a - b;
} FunctionAlias(Vec2_Subtract, -);

VoidFreeFunction(Vec2_SubtractInPlace,
  "Subtract 'b' from 'a'",
  V2, a,
  V2, b)
{
  Mutable(a) -= b;
} FunctionAlias(Vec2_SubtractInPlace, -=);

