#ifndef LTE_RNG_h__
#define LTE_RNG_h__

#include "Reference.h"
#include "StdMath.h"
#include "V3.h"
#include "V4.h"

struct RNGT : public RefCounted {
  virtual ~RNGT() {}

  /* [0, 2^32 - 1] */
  virtual unsigned int GetInt() = 0;

  float GetAngle() {
    return kTau * GetFloat();
  }

  V2 GetCircle() {
    V2 p;
    do p = GetSquare();
    while (p.GetLengthSquared() > 1.f);
    return p;
  }

  V3 GetCube() {
    float x = GetFloat();
    float y = GetFloat();
    float z = GetFloat();
    return 2.f * V3(x, y, z) - V3(1);
  }

  V3 GetCubeSurface() {
    while (true) {
      V3 c = GetCube();
      float m = Abs(c).GetMax();
      if (m >= 1.0e-6f)
        return c / m;
    }
  }

  V3 GetDirection() {
    V3 p;
    do p = Normalize(GetSphere());
    while (Abs(p) <= V3(1e-6f));
    return p;
  }

  /* [0, 1) */
  double GetDouble() {
    return (double)GetInt() / 4294967296.0;
  };

  /* [m, M] */
  int GetInt(int lower, int upper) {
    return lower + GetInt() % (upper - lower + 1);
  }

  /* [m, M] absolute uniform */
  int GetInt2(int lower, int upper) {
    return lower + (int)((double)GetFloat() * (double)(upper - lower + 1));
  }

  /* [0, 1) */
  float GetFloat() {
    return (float)((double)GetInt() / 4294967296.0);
  };

  /* [m, M) */
  float GetFloat(float lower, float upper) {
    return lower + (upper - lower) * GetFloat();
  }

  /* [0, +inf) */
  float GetExp() {
    return -(float)Log(1.0 - GetDouble());
  }

  /* [0, +inf) */
  float GetErlang(int n) {
    float sum = 0;
    for (int i = 0; i < n; ++i)
      sum += GetExp();
    return sum;
  }

  /* (-inf, +inf) */
  float GetGaussian() {
    float angle = kTau * GetFloat();
    double radius = 1.0 - GetDouble();
    return Cos(angle) * (float)Sqrt(-2.0 * Log(radius));
  }

  float GetSign() {
    return GetFloat() <= 0.5f ? -1.0f : 1.0f;
  }

  V3 GetSphere() {
    V3 p;
    do p = GetCube();
    while (p.GetLengthSquared() > 1.0f);
    return p;
  }

  V2 GetSquare() {
    float x = GetFloat();
    float y = GetFloat();
    return 2.0f * V2(x, y) - V2(1);
  }

  V2 GetV2(float m, float M) {
    float x = GetFloat();
    float y = GetFloat();
    float d = M - m;
    return V2(m + d * x,
              m + d * y);
  }

  V2 GetV2(V2 const& lower, V2 const& upper) {
    float x = GetFloat();
    float y = GetFloat();
    return Mix(lower, upper, V2(x, y));
  }

  V3 GetV3() {
    return GetV3(0, 1);
  }

  V3 GetV3(float m, float M) {
    float x = GetFloat();
    float y = GetFloat();
    float z = GetFloat();
    float d = M - m;
    return V3(m + d * x,
              m + d * y,
              m + d * z);
  }

  V3 GetV3(V3 const& lower, V3 const& upper) {
    float x = GetFloat();
    float y = GetFloat();
    float z = GetFloat();
    return Mix(lower, upper, V3(x, y, z));
  }

  V4 GetV4() {
    return GetV4(0, 1);
  }

  V4 GetV4(float m, float M) {
    float x = GetFloat();
    float y = GetFloat();
    float z = GetFloat();
    float w = GetFloat();
    float d = M - m;
    return V4(m + d * x,
              m + d * y,
              m + d * z,
              m + d * w);
  }
};

LT_API RNG RNG_Default();

LT_API RNG RNG_LCG(unsigned int seed);

LT_API RNG RNG_MTG(unsigned int seed);
LT_API RNG RNG_MTG(
  unsigned int const* arr, size_t size);

#endif
