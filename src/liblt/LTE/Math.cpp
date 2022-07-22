#include "Math.h"

#include <cstring>

uint V3ToColor32(const V3& color, float alpha) {
  uchar r = (uchar)Floor(255.f * color.x);
  uchar g = (uchar)Floor(255.f * color.y);
  uchar b = (uchar)Floor(255.f * color.z);
  uchar a = (uchar)Floor(255.f * alpha);
  return (r << 24) | (g << 16) | (b << 8) | a;
}

String DecimalToString(uint decimal) {
  const char* encoding = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  const size_t base = strlen(encoding);
  String result;
  while (decimal > 0) {
    result += encoding[decimal % base];
    decimal /= base;
  }
  return result;
}

void RotateBasis(V3& e1, V3& e2, V3& e3, const V3& rot) {
  if (rot.x != 0.0f)
    RotateTangentSpace(e3, e2, rot.x);
  if (rot.y != 0.0f)
    RotateTangentSpace(e1, e3, rot.y);
  if (rot.z != 0.0f)
    RotateTangentSpace(e2, e1, rot.z);
}

void RotateTangentSpace(V3& e1, V3& e2, float angle) {
  float c = Cos(angle);
  float s = Sin(angle);
  V3 newE1 = c * e1 - s * e2;
  e2 = s * e1 + c * e2;
  e1 = newE1;
}

float WorleyDistance2D(const V2& v, const V2& c) {
  V2 p = c + V2(noise(c), noise(c + V2(.77f)));
  return LengthSquared(p - v);
}

float WorleyNoise2D(const V2& p) {
  float minDist = FLT_MAX;
  V2 c = Floor(p);
  for (int xo = -1; xo <= 1; ++xo)
  for (int yo = -1; yo <= 1; ++yo) {
    V2 xp = c + V2((float)xo, (float)yo);
    V2 n = xp + V2(noise(xp), noise(xp + V2(.77f)));
    minDist = Min(minDist, LengthSquared(p - n));
  }
  return Sqrt(minDist);
}

float WorleyNoise3D(const V3& p) {
  float minDist = FLT_MAX;
  V3 c = Floor(p);
  for (int xo = -1; xo <= 1; ++xo)
  for (int yo = -1; yo <= 1; ++yo)
  for (int zo = -1; zo <= 1; ++zo) {
    V3 xp = c + V3((float)xo, (float)yo, (float)zo);
    V3 n = xp + V3(noise(xp), noise(xp + V3(.77f)), noise(xp + V3(.1473f)));
    minDist = Min(minDist, LengthSquared(p - n));
  }
  return Sqrt(minDist);
}

DistanceNoise3D::DistanceNoise3D(uint pointCount, float c1, float c2, float c3,
                                 float min, float max) :
  c1(c1),
  c2(c2),
  c3(c3)
{
  for (size_t i = 0; i < pointCount; ++i)
    points.push_back(RandV3(min, max));
}

float DistanceNoise3D::operator()(const V3& v) const {
  float dist1 = FLT_MAX;
  float dist2 = FLT_MAX;
  float dist3 = FLT_MAX;
  for (size_t i = 0; i < points.size(); ++i) {
    float thisDist = LengthSquared(v - points[i]);
    if (thisDist < dist1) {
      dist3 = dist2;
      dist2 = dist1;
      dist1 = thisDist;
    }
    else if (thisDist < dist2) {
      dist3 = dist2;
      dist2 = thisDist;
    }
    else if (thisDist < dist3)
      dist3 = thisDist;
  }
  return c1*Sqrt(dist1) + c2*Sqrt(dist2) + c3*Sqrt(dist3);
}

VoronoiNoise3D::VoronoiNoise3D(uint pointCount, float min, float max) {
  for (uint i = 0; i < pointCount; ++i)
    points.push_back(RandV3(min, max));
}

float VoronoiNoise3D::operator()(const V3& v) const {
  float m = FLT_MAX;
  for (size_t i = 0; i < points.size(); ++i)
    m = Min(m, LengthSquared(v - points[i]));
  return Sqrt(m);
}
