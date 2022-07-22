#ifndef Math_h__
#define Math_h__

#include "StdMath.h"
#include "String.h"
#include "V2.h"
#include "V3.h"
#include "V4.h"

#include <limits>
#include <vector>

LT_API uint V3ToColor32(V3 const& color, float alpha);

LT_API String DecimalToString(uint decimal);

/* Linear Algebra. */
template <class PointT>
float ComputeImpact(
  PointT const& pSrc,
  PointT const& pDst,
  V3 const& vSrc,
  V3 const& vDst,
  float speed,
  PointT* hitPoint = nullptr)
{
  V3 pr = (V3)(pDst - pSrc);
  V3 vr = vDst - vSrc;
  float a = LengthSquared(vr) - speed * speed;
  float b = 2.0f * Dot(vr, pr);
  float c = LengthSquared(pr);
  float d = b * b - 4.0f * a * c;

  if (d <= 0.0f)
    return -1.0f;
  else {
    d = Sqrt(d);
    float t = Max((-b - d) / (2.0f * a), (-b + d) / (2.0f * a));
    if (hitPoint)
      *hitPoint = pDst + t * vr;
    return t;
  }
}

LT_API void RotateBasis(V3& e1, V3& e2, V3& e3, V3 const& rot);
LT_API void RotateTangentSpace(V3& e1, V3& e2, float angle);

struct DistanceNoise3D {
  float c1, c2, c3;
  std::vector<V3> points;
  LT_API DistanceNoise3D(uint pointCount, float c1, float c2, float c3,
                         float min, float max);
  LT_API float operator()(V3 const& v) const;
};

struct VoronoiNoise3D {
  std::vector<V3> points;
  LT_API VoronoiNoise3D(uint pointCount, float min, float max);
  LT_API float operator()(V3 const& v) const;
};

/* Inlines. */
inline V3 Cylindrical(float r, float angle, float h) {
  return V3(r * Cos(angle), h, r * Sin(angle));
}

template <class T>
T Desaturate(T const& v, float amount) {
  return Mix(v, T(Luminance(v)), amount);
}

inline float Gain(float t, float g) {
  t *= 2.0f;
  if (t < 1.0f)
    return 0.5f * Pow(t, g);
  else
    return 1.0f - 0.5f * Pow(2.0f - t, g);
}

inline float InterpCosine(float t) {
  return (1.0f - Cos(t * kPi)) * 0.5f;
}

inline float InterpQuintic(float t) {
  return t * t * t * ((6.0f * t - 15.0f) * t + 10.0f);
}

inline void OrthoBasis(V3 const& e1, V3& e2, V3& e3) {
  float absY = Abs(e1.y);
  e2.x = Mix( e1.z,  e1.y, absY);
  e2.y = Mix( 0.0f, -e1.x, absY);
  e2.z = Mix(-e1.x,  0.0f, absY);
  e2 = Normalize(e2);
  e3 = Normalize(Cross(e1, e2));
}

inline V3 OrthoVector(V3 const& e1) {
  return Mix<V3>(V3(e1.z,  0.0f, -e1.x),
                 V3(e1.y, -e1.x,  0.0f),
                 Abs(e1.y));
}

inline V3 OrthoVectorN(V3 const& v) {
  return Normalize(OrthoVector(v));
}

inline V2 Polar(float angle) {
  return V2(Cos(angle), Sin(angle));
}

inline float Quantize(float t, float levels) {
  return Floor(t * levels) / levels;
}

inline float Rand() {
  return (float)rand() / (float)RAND_MAX;
}

inline float Rand(float min, float max) {
  return min + Rand() * (max - min);
}

inline int RandI(int exclusiveMax) {
  return rand() % exclusiveMax;
}

inline int RandI(int inclusiveMin, int inclusiveMax) {
  return inclusiveMin + rand() % (inclusiveMax - inclusiveMin + 1);
}

inline int Rand32() {
  int i1 = rand();
  int i2 = rand();
  return (i1 << 16) | i2;
}

inline float RandAngle() {
  return Rand(0, kTau);
}

inline float RandExp() {
  return -Log((1.0f - Rand()) + FLT_EPSILON);
}

inline float RandErlang(int n) {
  float sum = 0.0f;
  for (int i = 0; i < n; ++i)
    sum += RandExp();
  return sum;
}

inline float RandExp(float t) {
  return -Log(t + FLT_EPSILON);
}

inline float RandGauss(float mean, float sd) {
  int sign = RandI(2);
  return mean + (sign ? 1.0f : -1.0f) * sd * RandExp();
}

inline float Ridged(float t) {
  return Abs(1.0f - 2.0f * t);
}

inline V3 SampleCube() {
  float x = Rand();
  float y = Rand();
  float z = Rand();
  return V3(2.0f * x - 1.0f,
            2.0f * y - 1.0f,
            2.0f * z - 1.0f);
}

template <class T>
T& SamplePDF(float* probability, T* elem, size_t elems, float f) {
  float cumulative = 0;
  for (size_t i = 0; i < elems; ++i)
    cumulative += probability[i];
  f *= cumulative;
  for (size_t i = 0; i < elems; ++i) {
    f -= probability[i];
    if (f < 0)
      return elem[i];
  }
  return elem[0];
}

template <class T>
T const& SamplePDF(float* probability, T const* elem, size_t elems, float f) {
  return SamplePDF(probability, (T*)elem, elems, f);
}

inline V3 SampleSphere() {
  V3 point;
  do
    point = SampleCube();
  while (point.GetLengthSquared() > 1.0f);
  return point;
}

inline V3 RandDirection() {
  return Normalize(SampleSphere());
}

inline float Sigfigs(float f, float figures) {
  if (Abs(f) < FLT_EPSILON)
    return 0;
  float exponent = figures - 1.0f - Floor(Log(f, 10.f));
  return Quantize(f, Pow(10.0f, exponent));
}

inline float Sigmoid(float t) {
  return 1.0f / (1.0f + Exp(-t));
}

inline V3 Spherical(float r, float yaw, float pitch) {
  float cosTheta = Cos(pitch);
  return r * V3(cosTheta * Cos(yaw),
                Sin(pitch),
                cosTheta * Sin(yaw));
}

/* Noise. */
inline float noise(float x) {
  return Fract(Sin(x) * 4173.137f);
}

inline float noise(V2 const& v) {
  return noise(v.x + noise(v.y));
}

inline float noise(V3 const& v) {
  return noise(v.x + noise(v.y + noise(v.z)));
}

inline float noise(V4 const& v) {
  return noise(v.x + noise(v.y + noise(v.z + noise(v.w))));
}

inline float ValueNoise1D(float p) {
  float f = Floor(p);
  float i = Fract(p);
  i = i * i * (3.0f - 2.0f * i);
  return Mix(noise(f), noise(f + 1.0f), i);
}

inline float ValueNoise2D(V2 const& p) {
  V2 f = Floor(p);
  V2 i = Fract(p);
  i = i * i * (V2(3.0f) - 2.0f * i);
  return Mix(Mix(noise(f + V2(0, 0)), noise(f + V2(1, 0)), i.x),
             Mix(noise(f + V2(0, 1)), noise(f + V2(1, 1)), i.x), i.y);
}

inline float ValueNoise3D(V3 const& p) {
  V3 f = Floor(p);
  V3 i = Fract(p);
  i = i * i * (V3(3.0f) - 2.0f * i);
  return Mix(Mix(Mix(noise(f + V3(0, 0, 0)), noise(f + V3(1, 0, 0)), i.x),
                 Mix(noise(f + V3(0, 1, 0)), noise(f + V3(1, 1, 0)), i.x), i.y),
             Mix(Mix(noise(f + V3(0, 0, 1)), noise(f + V3(1, 0, 1)), i.x),
                 Mix(noise(f + V3(0, 1, 1)), noise(f + V3(1, 1, 1)), i.x), i.y), i.z);
}

inline float ValueNoise4D(V4 const& p) {
  V4 f = Floor(p);
  V4 i = Fract(p);
  i = i * i * (V4(3.0f) - 2.0f * i);
  return Mix(Mix(Mix(Mix(noise(f + V4(0, 0, 0, 0)), noise(f + V4(1, 0, 0, 0)), i.x),
                     Mix(noise(f + V4(0, 1, 0, 0)), noise(f + V4(1, 1, 0, 0)), i.x), i.y),
                 Mix(Mix(noise(f + V4(0, 0, 1, 0)), noise(f + V4(1, 0, 1, 0)), i.x),
                     Mix(noise(f + V4(0, 1, 1, 0)), noise(f + V4(1, 1, 1, 0)), i.x), i.y), i.z),
             Mix(Mix(Mix(noise(f + V4(0, 0, 0, 1)), noise(f + V4(1, 0, 0, 1)), i.x),
                     Mix(noise(f + V4(0, 1, 0, 1)), noise(f + V4(1, 1, 0, 1)), i.x), i.y),
                 Mix(Mix(noise(f + V4(0, 0, 1, 1)), noise(f + V4(1, 0, 1, 1)), i.x),
                     Mix(noise(f + V4(0, 1, 1, 1)), noise(f + V4(1, 1, 1, 1)), i.x), i.y), i.z), i.w);
}

inline float ValueNoise(float p) {
  return ValueNoise1D(p);
}

inline float ValueNoise(V2 const& p) {
  return ValueNoise2D(p);
}

inline float ValueNoise(V3 const& p) {
  return ValueNoise3D(p);
}

inline float ValueNoise(V4 const& p) {
  return ValueNoise4D(p);
}

LT_API float WorleyNoise2D(V2 const& p);
LT_API float WorleyNoise3D(V3 const& p);

template <class FnType, class VecType>
float Fractal(FnType& function, VecType p, int octaves, float lac) {
  lac = 1 / lac;
  float total = 0;
  float totalWeight = 0;
  float weight = 1;

  for (int i = 0; i < octaves; ++i) {
    totalWeight += weight;
    total += weight * function(p);
    p *= 2.0f;
    weight *= lac;
  }
  return total / totalWeight;
}

#endif
