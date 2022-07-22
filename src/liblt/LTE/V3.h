#ifndef LTE_V3_h__
#define LTE_V3_h__

#include "AutoClass.h"
#include "Finite.h"
#include "StdMath.h"
#include "V2.h"

template <class T>
AutoClass(V3T,
  T, x,
  T, y,
  T, z)

  typedef T ReturnType;
  typedef T MetricType;

  /* Constructors. */
  V3T() {}

  V3T(V2T<T> const& xy, T z) :
    x(xy.x),
    y(xy.y),
    z(z)
    {}

  V3T(T xyz) :
    x(xyz),
    y(xyz),
    z(xyz)
    {}

  template <class U>
  V3T(V3T<U> const& v) :
    x((T)v.x),
    y((T)v.y),
    z((T)v.z)
    {}

  T& operator[](int index) {
    return (&x)[index];
  }

  const T& operator[](int index) const {
    return (&x)[index];
  }

  bool operator==(V3T const& v) const {
    return x == v.x &&
           y == v.y &&
           z == v.z;
  }

  bool operator!=(V3T const& v) const {
    return x != v.x ||
           y != v.y ||
           z != v.z;
  }

  bool operator<(V3T const& v) const {
    return x < v.x &&
           y < v.y &&
           z < v.z;
  }

  bool operator<=(V3T const& v) const {
    return x <= v.x &&
           y <= v.y &&
           z <= v.z;
  }

  bool operator>(V3T const& v) const {
    return x > v.x &&
           y > v.y &&
           z > v.z;
  }

  bool operator>=(V3T const& v) const {
    return x >= v.x &&
           y >= v.y &&
           z >= v.z;
  }

  template <class T2>
  void operator+=(V3T<T2> const& v) {
    x += v.x;
    y += v.y;
    z += v.z;
  }

  template <class T2>
  void operator-=(V3T<T2> const& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
  }

  template <class T2>
  void operator*=(V3T<T2> const& v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
  }

  template <class T2>
  void operator/=(V3T<T2> const& v) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
  }

  template <class T2>
  void operator*=(T2 f) {
    x *= (T)f;
    y *= (T)f;
    z *= (T)f;
  }

  template <class T2>
  void operator/=(T2 f) {
    x /= (T)f;
    y /= (T)f;
    z /= (T)f;
  }

  V3T operator-() const {
    return V3T(-x, -y, -z);
  }

  template <class T2>
  friend V3T operator+(V3T const& v1, V3T<T2> const& v2) {
    return V3T(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
  }

  template <class T2>
  friend V3T operator-(V3T const& v1, V3T<T2> const& v2) {
    return V3T(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
  }

  template <class T2>
  friend V3T operator*(V3T const& v1, V3T<T2> const& v2) {
    return V3T(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
  }

  template <class T2>
  friend V3T operator/(V3T const& v1, V3T<T2> const& v2) {
    return V3T(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
  }

  friend V3T operator*(V3T const& v1, T s) {
    return V3T(v1.x * s, v1.y * s, v1.z * s);
  }

  friend V3T operator/(V3T const& v1, T s) {
    return V3T(v1.x / s, v1.y / s, v1.z / s);
  }

  friend V3T operator*(T s, V3T const& v1) {
    return V3T(s * v1.x, s * v1.y, s * v1.z);
  }

  friend V3T operator/(T s, V3T const& v1) {
    return V3T(s / v1.x, s / v1.y, s / v1.z);
  }

  bool IsFinite() const {
    return LTE::IsFinite(x) &&
           LTE::IsFinite(y) &&
           LTE::IsFinite(z);
  }

  T GetAverage() const {
    return (x + y + z) / 3.0f;
  }

  T GetGeometricAverage() const {
    return Pow(x * y * z, 1.0f / 3.0f);
  }

  T GetLengthSquared() const {
    return x*x + y*y + z*z;
  }

  T GetLength() const {
    return Sqrt(GetLengthSquared());
  }

  T GetMax() const {
    bool xy = x >= y;
    if ( xy && x >= z) return x;
    if (!xy && y >= z) return y;
    return z;
  }

  T GetMin() const {
    bool xy = x <= y;
    if ( xy && x <= z) return x;
    if (!xy && y <= z) return y;
    return z;
  }

  T GetProduct() const {
    return x * y * z;
  }

  T GetSum() const {
    return x + y + z;
  }

  V3T GetX() const {
    return V3T(x, 0, 0);
  }

  V3T GetY() const {
    return V3T(0, y, 0);
  }

  V3T GetZ() const {
    return V3T(0, 0, z);
  }

  V2T<T> GetXY() const {
    return V2T<T>(x, y);
  }

  V2T<T> GetXZ() const {
    return V2T<T>(x, z);
  }

  V2T<T> GetYZ() const {
    return V2T<T>(y, z);
  }

  /* Vectorized standard math. */
  friend V3T Abs(V3T const& v) {
    return V3T(
      Abs(v.x),
      Abs(v.y),
      Abs(v.z));
  }

  friend V3T Ceil(V3T const& v) {
    return V3T(
      Ceil(v.x),
      Ceil(v.y),
      Ceil(v.z));
  }

  friend V3T Clamp(V3T const& v, V3T const& lower, V3T const& upper) {
    return V3T(
      Clamp(v.x, lower.x, upper.x),
      Clamp(v.y, lower.y, upper.y),
      Clamp(v.z, lower.z, upper.z));
  }

  friend V3T Cross(V3T const& v1, V3T const& v2) {
    return V3T(v1.y*v2.z - v1.z*v2.y,
               v1.z*v2.x - v1.x*v2.z,
               v1.x*v2.y - v1.y*v2.x);
  }

  friend V3T Exp(V3T const& v1) {
    return V3T(
      Exp(v1.x),
      Exp(v1.y),
      Exp(v1.z));
  }

  friend V3T Floor(V3T const& v) {
    return V3T(
      Floor(v.x),
      Floor(v.y),
      Floor(v.z));
  }

  friend T Dot(V3T const& v1, V3T const& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
  }

  friend V3T Fract(V3T const& v) {
    return V3T(
      Fract(v.x),
      Fract(v.y),
      Fract(v.z));
  }

  friend T Length(V3T const& v) {
    return Sqrt(Dot(v, v));
  }

  friend MetricType LengthSquared(V3T const& v) {
    return v.GetLengthSquared();
  }

  friend V3T Log(V3T const& v1) {
    return V3T(
      Log(v1.x),
      Log(v1.y),
      Log(v1.z));
  }

  friend V3T Max(V3T const& v1, V3T const& v2) {
    return V3T(
      Max(v1.x, v2.x),
      Max(v1.y, v2.y),
      Max(v1.z, v2.z));
  }

  friend V3T Min(V3T const& v1, V3T const& v2) {
    return V3T(
      Min(v1.x, v2.x),
      Min(v1.y, v2.y),
      Min(v1.z, v2.z));
  }

  template <class T2>
  friend V3T Mix(V3T const& v1, V3T const& v2, T2 const& t) {
    return V3T(
      Mix(v1.x, v2.x, t),
      Mix(v1.y, v2.y, t),
      Mix(v1.z, v2.z, t));
  }

  friend V3T Mix(V3T const& v1, V3T const& v2, V3T const& t) {
    return V3T(
      Mix(v1.x, v2.x, t.x),
      Mix(v1.y, v2.y, t.y),
      Mix(v1.z, v2.z, t.z));
  }

  friend V3T Mod(V3T const& v1, V3T const& v2) {
    return V3T(
      Mod(v1.x, v2.x),
      Mod(v1.y, v2.y),
      Mod(v1.z, v2.z));
  }

  friend V3T Normalize(V3T const& v) {
    MetricType l = Length(v);
    return l > 0 ? v / l : v;
  }

  friend MetricType PNorm(V3T const& v, T p) {
    return Pow(Pow(Abs(v.x), p) +
               Pow(Abs(v.y), p) +
               Pow(Abs(v.z), p), 1 / p);
  }

  friend V3T Pow(V3T const& v1, V3T const& v2) {
    return V3T(
      Pow(v1.x, v2.x),
      Pow(v1.y, v2.y),
      Pow(v1.z, v2.z));
  }

  friend V3T Round(V3T const& v) {
    return V3T(
      Round(v.x),
      Round(v.y),
      Round(v.z));
  }

  friend V3T Saturate(V3T const& v) {
    return V3T(
      Saturate(v.x),
      Saturate(v.y),
      Saturate(v.z));
  }

  friend V3T Sign(V3T const& v) {
    return V3T(
      Sign(v.x),
      Sign(v.y),
      Sign(v.z));
  }

  friend MetricType Angle(V3T const& v1, V3T const& v2) {
    MetricType d = (MetricType)(Dot(v1, v2) / (Length(v1) * Length(v2)));
    return Acos(d < -1.f ? -1.f : (d > 1.f ? 1.f : d));
  }

  friend V3T OrthoProjection(V3T const& v, V3T const& unitDir) {
    return v - unitDir * Dot(v, unitDir);
  }
};

LT_API V3 RandV3(float min = 0, float max = 1);

#endif
