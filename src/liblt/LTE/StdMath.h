#ifndef LTE_StdMath_h__
#define LTE_StdMath_h__

#include <math.h>
#include <stdlib.h>

const float kTau = 6.28318531f;
const float kPi  = 3.14159265f;
const float kPi2 = 1.57079633f;
const float kPi4 = 0.785398163f;
const float kPi6 = 0.523598776f;

/* Units. */
inline float Degrees(float radians) {
  return (180.0f / kPi) * radians;
}

inline double Degrees(double radians) {
  return (180.0 / kPi) * radians;
}

inline float Radians(float degrees) {
  return (kPi / 180.0f) * degrees;
}

inline double Radians(double degrees) {
  return (kPi / 180.0) * degrees;
}

/* General. */
inline int Abs(int t) {
  return abs(t);
}

inline long int Abs(long int t) {
  return labs(t);
}

inline float Abs(float t) {
  return (float)fabs((double)t);
}

inline double Abs(double t) {
  return fabs(t);
}

inline float Ceil(float t) {
  return (float)ceil((double)t);
}

inline double Ceil(double t) {
  return ceil(t);
}

template <class T>
T Clamp(T t, T lower, T upper) {
  return t < lower ? lower :
         t > upper ? upper :
         t;
}

inline float Exp(float t) {
  return (float)exp((double)t);
}

inline double Exp(double t) {
  return exp(t);
}

inline float Floor(float t) {
  return (float)floor((double)t);
}

inline double Floor(double t) {
  return floor(t);
}

inline float Fract(float t) {
  return t - Floor(t);
}

inline double Fract(double t) {
  return t - Floor(t);
}

inline float Gaussian(float t, float variance) {
  return Exp(-(t * t) / variance);
}

inline double Gaussian(double t, double variance) {
  return Exp(-(t * t) / variance);
}

inline float Log(float t) {
  return (float)log((double)t);
}

inline double Log(double t) {
  return log(t);
}

inline float Log(float t, float b) {
  return Log(t) / Log(b);
}

inline double Log(double t, double b) {
  return Log(t) / Log(b);
}

template <class T>
T Max(T const& one, T const& two) {
  return one >= two ? one : two;
}

template <class T>
T Max(T const& one, T const& two, T const& three) {
  return Max(Max(one, two), three);
}

template <class T>
T Min(T const& one, T const& two) {
  return one <= two ? one : two;
}

template <class T>
T Min(T const& one, T const& two, T const& three) {
  return Min(Min(one, two), three);
}

template <class T1, class T2>
T1 Mix(T1 const& one, T1 const& two, T2 const& t) {
  return (T1)(((T2)1 - t) * one + t * two);
}

template <class T>
int Mix(int one, int two, T const& t) {
  return (int)Mix((double)one, (double)two, t);
}

inline float Mod(float t, float m) {
  return (float)fmod((double)t, (double)m);
}

inline double Mod(double t, double m) {
  return fmod(t, m);
}

inline float Negative(float t) {
  return Min(t, 0.0f);
}

inline double Negative(double t) {
  return Min(t, 0.0);
}

inline float Positive(float t) {
  return Max(t, 0.0f);
}

inline double Positive(double t) {
  return Max(t, 0.0);
}

inline float Pow(float t, float p) {
  return (float)pow((double)t, (double)p);
}

inline double Pow(double t, double p) {
  return pow(t, p);
}

inline float Round(float t) {
  return Floor(t + 0.5f);
}

inline double Round(double t) {
  return Floor(t + 0.5);
}

inline float Saturate(float t) {
  return t < 0.0f ? 0.0f :
         t > 1.0f ? 1.0f :
         t;
}

inline double Saturate(double t) {
  return t < 0.0 ? 0.0 :
         t > 1.0 ? 1.0 :
         t;
}

inline float Sign(float t) {
  return t  > 0.0f ?  1.0f :
         t == 0.0f ?  0.0f
                   : -1.0f;
}

inline double Sign(double t) {
  return t  > 0.0 ?  1.0 :
         t == 0.0 ?  0.0
                  : -1.0;
}

inline float SignPow(float t, float p) {
  return Sign(t) * Pow(Abs(t), p);
}

inline double SignPow(double t, double p) {
  return Sign(t) * Pow(Abs(t), p);
}

inline float Sqrt(float t) {
  return (float)sqrt((double)t);
}

inline double Sqrt(double t) {
  return sqrt(t);
}

/* Trig. */
inline float Acos(float t) {
  return (float)acos((double)t);
}

inline double Acos(double t) {
  return acos(t);
}

inline float Asin(float t) {
  return (float)asin((double)t);
}

inline double Asin(double t) {
  return asin(t);
}

inline float Atan(float t) {
  return (float)atan((double)t);
}

inline double Atan(double t) {
  return atan(t);
}

inline float Atan(float y, float x) {
  return (float)atan2((double)y, (double)x);
}

inline double Atan(double y, double x) {
  return atan2(y, x);
}

inline float Cos(float t) {
  return (float)cos((double)t);
}

inline double Cos(double t) {
  return cos(t);
}

inline float Sin(float t) {
  return (float)sin((double)t);
}

inline double Sin(double t) {
  return sin(t);
}

inline float Tan(float t) {
  return (float)tan((double)t);
}

inline double Tan(double t) {
  return tan(t);
}

/* Fast powers. */
template <class T>
inline T Squared(T t) {
  return t * t;
}

template <class T>
inline T Fourth(T t) {
  return Squared(Squared(t));
}

template <class T>
inline T Eighth(T t) {
  return Squared(Fourth(t));
}

#endif
