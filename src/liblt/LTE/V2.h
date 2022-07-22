#ifndef LTE_V2_h__
#define LTE_V2_h__

#include "Common.h"
#include "AutoClass.h"
#include "Finite.h"
#include "StdMath.h"

template <class T>
AutoClass(V2T,
  T, x,
  T, y)

  typedef T ReturnType;
  typedef T MetricType;

  /* Constructors. */
  V2T() {}

  V2T(T xy) :
    x(xy),
    y(xy)
    {}

  template <class U>
  V2T(V2T<U> const& v) :
    x((T)v.x),
    y((T)v.y)
    {}

  T& operator[](int index) {
    return (&x)[index];
  }

  T const& operator[](int index) const {
    return (&x)[index];
  }

  bool operator==(V2T const& v) const {
    return x == v.x && y == v.y;
  }

  bool operator!=(V2T const& v) const {
    return x != v.x || y != v.y;
  }

  bool operator<(V2T const& v) const {
    return x < v.x && y < v.y;
  }

  bool operator<=(V2T const& v) const {
    return x <= v.x && y <= v.y;
  }

  bool operator>(V2T const& v) const {
    return x > v.x && y > v.y;
  }

  bool operator>=(V2T const& v) const {
    return x >= v.x && y >= v.y;
  }

  void operator+=(V2T const& v) {
    x += v.x;
    y += v.y;
  }

  void operator-=(V2T const& v) {
    x -= v.x;
    y -= v.y;
  }

  void operator*=(V2T const& v) {
    x *= v.x;
    y *= v.y;
  }

  void operator/=(V2T const& v) {
    x /= v.x;
    y /= v.y;
  }

  void operator*=(T f) {
    x *= f;
    y *= f;
  }

  void operator/=(T f) {
    x /= f;
    y /= f;
  }

  V2T operator-() const {
    return V2T(-x, -y);
  }

  friend V2T operator+(V2T const& v1, V2T const& v2) {
    return V2T(v1.x + v2.x, v1.y + v2.y);
  }

  friend V2T operator-(V2T const& v1, V2T const& v2) {
    return V2T(v1.x - v2.x, v1.y - v2.y);
  }

  friend V2T operator*(V2T const& v1, V2T const& v2) {
    return V2T(v1.x * v2.x, v1.y * v2.y);
  }

  friend V2T operator/(V2T const& v1, V2T const& v2) {
    return V2T(v1.x / v2.x, v1.y / v2.y);
  }

  friend V2T operator*(V2T const& v1, T s) {
    return V2T(v1.x * s, v1.y * s);
  }

  friend V2T operator/(V2T const& v1, T s) {
    return V2T<T>(v1.x / s, v1.y / s);
  }

  friend V2T operator*(T s, V2T const& v1) {
    return V2T(s * v1.x, s * v1.y);
  }

  friend V2T operator/(T s, V2T const& v1) {
    return V2T(s / v1.x, s / v1.y);
  }

  /* Member functions. */
  bool IsFinite() const {
    return LTE::IsFinite(x) &&
           LTE::IsFinite(y); 
  }

  T GetAverage() const {
    return (x + y) / 3.0f;
  }

  T GetGeometricAverage() const {
    return Sqrt(x * y);
  }

  T GetLengthSquared() const {
    return x*x + y*y;
  }

  T GetLength() const {
    return Sqrt(GetLengthSquared());
  }

  T GetMax() const {
    return x >= y ? x : y;
  }

  T GetMin() const {
    return x <= y ? x : y;
  }

  T GetProduct() const {
    return x * y;
  }

  T GetSum() const {
    return x + y;
  }

  V2T GetX() const {
    return V2T(x, 0);
  }

  V2T GetY() const {
    return V2T(0, y);
  }

  /* Vectorized standard math. */
  friend V2T Abs(V2T const& v) {
    return V2T(
      Abs(v.x),
      Abs(v.y));
  }

  friend V2T Ceil(V2T const& v) {
    return V2T(
      Ceil(v.x),
      Ceil(v.y));
  }

  friend V2T Clamp(V2T const& v, V2T const& lower, V2T const& upper) {
    return V2T(
      Clamp(v.x, lower.x, upper.x),
      Clamp(v.y, lower.y, upper.y));
  }

  friend MetricType Dot(V2T const& v1, V2T const& v2) {
    return v1.x*v2.x + v1.y*v2.y;
  }

  friend V2T Exp(V2T const& v) {
    return V2T(
      Exp(v.x),
      Exp(v.y));
  }

  friend V2T Floor(V2T const& v) {
    return V2T(
      Floor(v.x),
      Floor(v.y));
  }

  friend V2T Fract(V2T const& v) {
    return V2T(
      Fract(v.x),
      Fract(v.y));
  }

  friend MetricType Length(V2T const& v) {
    return Sqrt(Dot(v, v));
  }

  friend MetricType LengthSquared(V2T const& v) {
    return Dot(v, v);
  }

  friend V2T Log(V2T const& v) {
    return V2T(
      Log(v.x),
      Log(v.y));
  }

  friend V2T Max(V2T const& v1, V2T const& v2) {
    return V2T(
      Max(v1.x, v2.x),
      Max(v1.y, v2.y));
  }

  friend V2T Min(V2T const& v1, V2T const& v2) {
    return V2T(
      Min(v1.x, v2.x),
      Min(v1.y, v2.y));
  }

  friend V2T Mix(V2T const& v1, V2T const& v2, V2T const& t) {
    return V2T(
      Mix(v1.x, v2.x, t.x),
      Mix(v1.y, v2.y, t.y));
  }

  friend V2T Normalize(V2T const& v) {
    MetricType l = Length(v);
    return l > 0 ? v / l : v;
  }

  friend MetricType PNorm(V2T const& v, T p) {
    return Pow(Pow(Abs(v.x), p) +
               Pow(Abs(v.y), p), 1 / p);
  }

  friend V2T Pow(V2T const& v1, V2T const& v2) {
    return V2T(
      Pow(v1.x, v2.x),
      Pow(v1.y, v2.y));
  }

  friend V2T Round(V2T const& v) {
    return V2T(
      Round(v.x),
      Round(v.y));
  }

  friend V2T Saturate(V2T const& v) {
    return V2T(
      Saturate(v.x),
      Saturate(v.y));
  }

  friend V2T Sign(V2T const& v) {
    return V2T(
      Sign(v.x),
      Sign(v.y));
  }
};

LT_API V2 RandV2(float min, float max);

#endif
