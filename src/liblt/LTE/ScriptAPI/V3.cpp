#include "LTE/Data.h"
#include "LTE/Function.h"
#include "LTE/Math.h"
#include "LTE/V3.h"

TypeAlias(V3F, Vec3);
TypeAlias(V3F, Vec3f);
TypeAlias(V3D, Vec3d);

DefineConversion(V3F_to_V3D, V3F, V3D) {
  dest = V3D(src);
}

DefineConversion(float_to_V3F, float, V3F) {
  dest = V3F(src);
}

DefineConversion(int_to_V3F, int, V3F) {
  dest = V3F(src);
}

FreeFunction(V3, Vec3_Create,
  "Create a 3D, single-precision vector ('x', 'y', 'z')",
  float, x,
  float, y,
  float, z)
{
  return V3(x, y, z);
} FunctionAlias(Vec3_Create, Vec3);

FreeFunction(V3, Vec3d_Create,
  "Create a 3D, double-precision vector ('x', 'y', 'z')",
  float, x,
  float, y,
  float, z)
{
  return V3(x, y, z);
} FunctionAlias(Vec3d_Create, Vec3d);

FreeFunction(V3F, Vec3f_Abs,
  "Return the component-wise absolute value of 'v'",
  V3F, v)
{
  return Abs(v);
} FunctionAlias(Vec3f_Abs, Abs);

FreeFunction(V3D, Vec3d_Abs,
  "Return the component-wise absolute value of 'v'",
  V3D, v)
{
  return Abs(v);
} FunctionAlias(Vec3d_Abs, Abs);

FreeFunction(V3F, Vec3f_Add,
  "Return the sum of 'a' and 'b'",
  V3F, a,
  V3F, b)
{
  return a + b;
} FunctionAlias(Vec3f_Add, +);

VoidFreeFunction(Vec3f_AddInPlace,
  "Add 'b' to 'a'",
  V3F, a,
  V3F, b)
{
  Mutable(a) += b;
} FunctionAlias(Vec3f_AddInPlace, +=);

FreeFunction(V3D, Vec3d_Add,
  "Return the sum of 'a' and 'b'",
  V3D, a,
  V3D, b)
{
  return a + b;
} FunctionAlias(Vec3d_Add, +);

VoidFreeFunction(Vec3d_AddInPlace,
  "Add 'b' to 'a'",
  V3D, a,
  V3D, b)
{
  Mutable(a) += b;
} FunctionAlias(Vec3d_AddInPlace, +=);

FreeFunction(V3, Vec3_Clamp,
  "Return the component-wise clamp of 'v' and ['lower', 'upper']",
  V3, v,
  V3, lower,
  V3, upper)
{
  return Clamp(v, lower, upper);
} FunctionAlias(Vec3_Clamp, Clamp);

FreeFunction(V3, Vec3_Cross,
  "Return the cross-product of 'a' and 'b'",
  V3, a,
  V3, b)
{
  return Cross(a, b);
} FunctionAlias(Vec3_Cross, Cross);

FreeFunction(V3, Vec3_Cylinder,
  "Return the cylindrical vector (r, theta, height) in cartesian coordinates",
  float, radius,
  float, angle,
  float, height)
{
  return V3(radius * Cos(angle), height, radius * Sin(angle));
}

FreeFunction(float, Vec3_Distance,
  "Return the distance between a 'a' and 'b'",
  V3, a,
  V3, b)
{
  return Length(a - b);
} FunctionAlias(Vec3_Distance, Distance);

namespace Priv1 {
  FreeFunction(V3F, Vec3f_Div,
    "Return v / s",
    V3F, v,
    float, s)
  {
    return v / s;
  }

  FreeFunction(V3D, Vec3d_Div,
    "Return v / s",
    V3D, v,
    double, s)
  {
    return v / s;
  }
}

namespace Priv2 {
  FreeFunction(V3F, Vec3f_Div, "Return the component-wise division of 'a' and 'b'",
    V3F, a,
    V3F, b)
  {
    return a / b;
  }

  FreeFunction(V3D, Vec3d_Div, "Return the component-wise division of 'a' and 'b'",
    V3D, a,
    V3D, b)
  {
    return a / b;
  }
}

FunctionAlias(Vec3f_Div, /);
FunctionAlias(Vec3d_Div, /);

FreeFunction(float, Vec3f_Dot,
  "Return the dot product of 'a' and 'b'",
  V3F, a,
  V3F, b)
{
  return Dot(a, b);
} FunctionAlias(Vec3f_Dot, Dot);

FreeFunction(double, Vec3d_Dot,
  "Return the dot product of 'a' and 'b'",
  V3D, a,
  V3D, b)
{
  return Dot(a, b);
} FunctionAlias(Vec3d_Dot, Dot);


FreeFunction(V3, Vec3_Floor,
  "Return the component-wise floor of 'v'",
  V3, v)
{
  return Floor(v);
} FunctionAlias(Vec3_Floor, Floor);

FreeFunction(float, Vec3f_Length,
  "Return the length of 'v'",
  V3F, v)
{
  return Length(v);
} FunctionAlias(Vec3f_Length, Length);

FreeFunction(double, Vec3d_Length,
  "Return the length of 'v'",
  V3D, v)
{
  return Length(v);
} FunctionAlias(Vec3d_Length, Length);

FreeFunction(V3, Vec3_Max,
  "Return the component-wise max of 'a' and 'b'",
  V3, a,
  V3, b)
{
  return Max(a, b);
} FunctionAlias(Vec3_Max, Max);

FreeFunction(V3, Vec3_Min,
  "Return the component-wise min of 'a' and 'b'",
  V3, a,
  V3, b)
{
  return Min(a, b);
} FunctionAlias(Vec3_Min, Min);

FreeFunction(V3F, Vec3f_Mix,
  "Return a linear interpolation of 'a' and 'b' with interpolant 't'",
  V3F, a,
  V3F, b,
  float, t)
{
  return Mix(a, b, t);
} FunctionAlias(Vec3f_Mix, Mix);

FreeFunction(V3D, Vec3d_Mix,
  "Return a linear interpolation of 'a' and 'b' with interpolant 't'",
  V3D, a,
  V3D, b,
  double, t)
{
  return Mix(a, b, t);
} FunctionAlias(Vec3d_Mix, Mix);

namespace Priv1 {
  FreeFunction(V3F, Vec3f_Mult,
    "Return the product of 's' and 'vec'",
    float, s,
    V3F, v)
  {
    return s * v;
  }

  FreeFunction(V3D, Vec3d_Mult,
    "Return the product of 's' and 'vec'",
    double, s,
    V3D, v)
  {
    return s * v;
  }
}

namespace Priv2 {
  FreeFunction(V3F, Vec3f_Mult, "Return the product of 'a' and 'b'",
    V3F, a,
    V3F, b)
  {
    return a * b;
  }

  FreeFunction(V3D, Vec3d_Mult, "Return the product of 'a' and 'b'",
    V3D, a,
    V3D, b)
  {
    return a * b;
  }
}

FunctionAlias(Vec3f_Mult, *);
FunctionAlias(Vec3d_Mult, *);

FreeFunction(V3, Vec3f_Normalize,
  "Return a unit-length vector pointing in the same direction as 'vec'",
  V3F, v)
{
  return Normalize(v);
} FunctionAlias(Vec3f_Normalize, Normalize);

FreeFunction(V3D, Vec3d_Normalize,
  "Return a unit-length vector pointing in the same direction as 'vec'",
  V3D, v)
{
  return Normalize(v);
} FunctionAlias(Vec3d_Normalize, Normalize);

FreeFunction(V3F, Vec3f_Pow,
  "Return the component-wise absolute value of 'v'",
  V3F, v,
  V3F, p)
{
  return Pow(v, p);
} FunctionAlias(Vec3f_Pow, ^);

FreeFunction(V3, Vec3_Spherical,
  "Return the cartesian coordinates of the spherical vector (radius, pitch, yaw) in cartesian coordinates",
  float, radius,
  float, pitch,
  float, yaw)
{
  return Spherical(radius, yaw, pitch);
} FunctionAlias(Vec3_Spherical, Spherical);

FreeFunction(V3, Vec3f_Subtract,
  "Return the difference of 'a' and 'b'",
  V3F, a,
  V3F, b)
{
  return a - b;
} FunctionAlias(Vec3f_Subtract, -);

VoidFreeFunction(Vec3f_SubtractInPlace,
  "Subtract 'b' from 'a'",
  V3F, a,
  V3F, b)
{
  Mutable(a) -= b;
} FunctionAlias(Vec3f_SubtractInPlace, -=);

FreeFunction(V3D, Vec3d_Subtract,
  "Return the difference of 'a' and 'b'",
  V3D, a,
  V3D, b)
{
  return a - b;
} FunctionAlias(Vec3d_Subtract, -);

VoidFreeFunction(Vec3d_SubtractInPlace,
  "Subtract 'b' from 'a'",
  V3D, a,
  V3D, b)
{
  Mutable(a) -= b;
} FunctionAlias(Vec3d_SubtractInPlace, -=);
