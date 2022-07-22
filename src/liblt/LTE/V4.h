#ifndef LTE_V4_h__
#define LTE_V4_h__

#include "Common.h"
#include "AutoClass.h"
#include "Finite.h"
#include "StdMath.h"
#include "V3.h"

template <class T>
AutoClass(V4T,
  T, x,
  T, y,
  T, z,
  T, w)

  typedef T ReturnType;
  typedef T MetricType;

  /* Constructors. */
  V4T() {}

  V4T(V3T<T> const& xyz, T w) :
    x(xyz.x),
    y(xyz.y),
    z(xyz.z),
    w(w)
    {}

  V4T(T xyzw) :
    x(xyzw),
    y(xyzw),
    z(xyzw),
    w(xyzw)
    {}

  template <class U>
  V4T(V4T<U> const& v) :
    x((T)v.x),
    y((T)v.y),
    z((T)v.z),
    w((T)v.w)
    {}

  T& operator[](int index) {
    return (&x)[index];
  }

  T const& operator[](int index) const {
    return (&x)[index];
  }

  bool operator==(V4T const& v) const {
    return x == v.x && y == v.y && z == v.z && w == v.w;
  }

  bool operator!=(V4T const& v) const {
    return x != v.x || y != v.y || z != v.z || w != v.w;
  }

  bool operator<(V4T const& v) const {
    return x < v.x && y < v.y && z < v.z &&  w < v.w;
  }

  bool operator<=(V4T const& v) const {
    return x <= v.x && y <= v.y && z <= v.z && w <= v.w;
  }

  bool operator>(V4T const& v) const {
    return x > v.x && y > v.y && z > v.z && w > v.w;
  }

  bool operator>=(V4T const& v) const {
    return x >= v.x && y >= v.y && z >= v.z && w >= v.w;
  }

  void operator+=(V4T const& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
  }

  void operator-=(V4T const& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
  }

  void operator*=(V4T const& v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
  }

  void operator/=(V4T const& v) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
  }

  void operator*=(T f) {
    x *= f;
    y *= f;
    z *= f;
    w *= f;
  }

  void operator/=(T f) {
    x /= f;
    y /= f;
    z /= f;
    w /= f;
  }

  V4T operator-() const {
    return V4T(-x, -y, -z, -w);
  }

  /* Member functions. */
  bool IsFinite() const {
    return LTE::IsFinite(x) &&
           LTE::IsFinite(y) &&
           LTE::IsFinite(z) &&
           LTE::IsFinite(w);
  }

  V3T<T> GetXYZ() const {
    return V3T<T>(x, y, z);
  }

  friend V4T operator+(V4T const& v1, V4T const& v2) {
    return V4T(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
  }

  friend V4T operator-(V4T const& v1, V4T const& v2) {
    return V4T(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
  }

  friend V4T operator*(V4T const& v1, V4T const& v2) {
    return V4T(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
  }

  friend V4T operator/(V4T const& v1, V4T const& v2) {
    return V4T(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
  }

  friend V4T operator*(V4T const& v1, T s) {
    return V4T(v1.x * s, v1.y * s, v1.z * s, v1.w * s);
  }

  friend V4T operator/(V4T const& v1, T s) {
    return V4T(v1.x / s, v1.y / s, v1.z / s, v1.w / s);
  }

  friend V4T operator*(T s, V4T const& v1) {
    return V4T(s * v1.x, s * v1.y, s * v1.z, s * v1.w);
  }

  friend V4T operator/(T s, V4T const& v1) {
    return V4T(s / v1.x, s / v1.y, s / v1.z, s / v1.w);
  }

  /* Vectorized standard math. */
  friend V4T Abs(V4T const& v) {
    return V4T(
      Abs(v.x),
      Abs(v.y),
      Abs(v.z),
      Abs(v.w));
  }

  friend V4T Ceil(V4T const& v) {
    return V4T(
      Ceil(v.x),
      Ceil(v.y),
      Ceil(v.z),
      Ceil(v.w));
  }

  friend V4T Clamp(V4T const& v, V4T const& lower, V4T const& upper) {
    return V4T(
      Clamp(v.x, lower.x, upper.x),
      Clamp(v.y, lower.y, upper.y),
      Clamp(v.z, lower.z, upper.z),
      Clamp(v.w, lower.w, upper.w));
  }

  friend MetricType Dot(V4T const& v1, V4T const& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
  }

  friend V4T Exp(V4T const& v) {
    return V4T(
      Exp(v.x),
      Exp(v.y),
      Exp(v.z),
      Exp(v.w));
  }

  friend V4T Floor(V4T const& v) {
    return V4T(
      Floor(v.x),
      Floor(v.y),
      Floor(v.z),
      Floor(v.w));
  }

  friend V4T Fract(V4T const& v) {
    return V4T(
      Fract(v.x),
      Fract(v.y),
      Fract(v.z),
      Fract(v.w));
  }

  friend MetricType Length(V4T const& v) {
    return Sqrt(Dot(v, v));
  }

  friend V4T Log(V4T const& v) {
    return V4T(
      Log(v.x),
      Log(v.y),
      Log(v.z),
      Log(v.w));
  }

  friend V4T Max(V4T const& v1, V4T const& v2) {
    return V4T(
      Max(v1.x, v2.x),
      Max(v1.y, v2.y),
      Max(v1.z, v2.z),
      Max(v1.w, v2.w));
  }

  friend V4T Min(V4T const& v1, V4T const& v2) {
    return V4T(
      Min(v1.x, v2.x),
      Min(v1.y, v2.y),
      Min(v1.z, v2.z),
      Min(v1.w, v2.w));
  }

  friend V4T Mix(V4T const& v1, V4T const& v2, V4T const& t) {
    return V4T(
      Mix(v1.x, v2.x, t.x),
      Mix(v1.y, v2.y, t.y),
      Mix(v1.z, v2.z, t.z),
      Mix(v1.w, v2.w, t.w));
  }

  friend V4T Mod(V4T const& v1, V4T const& v2) {
    return V4T(
      Mod(v1.x, v2.x),
      Mod(v1.y, v2.y),
      Mod(v1.z, v2.z),
      Mod(v1.w, v2.w));
  }

  friend V4T Normalize(V4T const& v) {
    MetricType l = Length(v);
    return l > 0 ? v / l : v;
  }

  friend V4T Pow(V4T const& v1, V4T const& v2) {
    return V4T(
      Pow(v1.x, v2.x),
      Pow(v1.y, v2.y),
      Pow(v1.z, v2.z),
      Pow(v1.w, v2.w));
  }

  friend V4T Round(V4T const& v) {
    return V4T(
      Round(v.x),
      Round(v.y),
      Round(v.z),
      Round(v.w));
  }

  friend V4T Saturate(V4T const& v) {
    return V4T(
      Saturate(v.x),
      Saturate(v.y),
      Saturate(v.z),
      Saturate(v.w));
  }

  friend V4T Sign(V4T const& v) {
    return V4T(
      Sign(v.x),
      Sign(v.y),
      Sign(v.z),
      Sign(v.w));
  }
};

#endif
