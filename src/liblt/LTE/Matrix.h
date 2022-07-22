#ifndef LTE_Matrix_h__
#define LTE_Matrix_h__

#include "Ray.h"
#include "StdMath.h"
#include "V3.h"
#include "V4.h"

template <class T>
struct MatrixT {
  /* Data (ROW-MAJOR!). */
  T e[16];

  /* Constructors. */
  MatrixT() {}

  MatrixT(
      T m11, T m12, T m13, T m14,
      T m21, T m22, T m23, T m24,
      T m31, T m32, T m33, T m34,
      T m41, T m42, T m43, T m44)
  {
    e[ 0] = m11; e[ 1] = m21; e[ 2] = m31; e[ 3] = m41;
    e[ 4] = m12; e[ 5] = m22; e[ 6] = m32; e[ 7] = m42;
    e[ 8] = m13; e[ 9] = m23; e[10] = m33; e[11] = m43;
    e[12] = m14; e[13] = m24; e[14] = m34; e[15] = m44;
  }

  MatrixT(T e[16]) {
    for (size_t i = 0; i < 16; ++i)
      this->e[i] = e[i];
  }

  template <class T2>
  MatrixT(MatrixT<T2> const& other) {
    for (size_t i = 0; i < 16; ++i)
      e[i] = (T)other.e[i];
  }

  /* Operators. */
  T& operator[](size_t index) {
    return e[index];
  }

  const T& operator[](size_t index) const {
    return e[index];
  }

  friend MatrixT operator*(MatrixT const& a, MatrixT const& b) {
    return MatrixT(
      a.e[ 0] * b.e[ 0] + a.e[ 4] * b.e[ 1] + a.e[ 8] * b.e[ 2] + a.e[12] * b.e[ 3],
      a.e[ 0] * b.e[ 4] + a.e[ 4] * b.e[ 5] + a.e[ 8] * b.e[ 6] + a.e[12] * b.e[ 7],
      a.e[ 0] * b.e[ 8] + a.e[ 4] * b.e[ 9] + a.e[ 8] * b.e[10] + a.e[12] * b.e[11],
      a.e[ 0] * b.e[12] + a.e[ 4] * b.e[13] + a.e[ 8] * b.e[14] + a.e[12] * b.e[15],
      a.e[ 1] * b.e[ 0] + a.e[ 5] * b.e[ 1] + a.e[ 9] * b.e[ 2] + a.e[13] * b.e[ 3],
      a.e[ 1] * b.e[ 4] + a.e[ 5] * b.e[ 5] + a.e[ 9] * b.e[ 6] + a.e[13] * b.e[ 7],
      a.e[ 1] * b.e[ 8] + a.e[ 5] * b.e[ 9] + a.e[ 9] * b.e[10] + a.e[13] * b.e[11],
      a.e[ 1] * b.e[12] + a.e[ 5] * b.e[13] + a.e[ 9] * b.e[14] + a.e[13] * b.e[15],
      a.e[ 2] * b.e[ 0] + a.e[ 6] * b.e[ 1] + a.e[10] * b.e[ 2] + a.e[14] * b.e[ 3],
      a.e[ 2] * b.e[ 4] + a.e[ 6] * b.e[ 5] + a.e[10] * b.e[ 6] + a.e[14] * b.e[ 7],
      a.e[ 2] * b.e[ 8] + a.e[ 6] * b.e[ 9] + a.e[10] * b.e[10] + a.e[14] * b.e[11],
      a.e[ 2] * b.e[12] + a.e[ 6] * b.e[13] + a.e[10] * b.e[14] + a.e[14] * b.e[15],
      a.e[ 3] * b.e[ 0] + a.e[ 7] * b.e[ 1] + a.e[11] * b.e[ 2] + a.e[15] * b.e[ 3],
      a.e[ 3] * b.e[ 4] + a.e[ 7] * b.e[ 5] + a.e[11] * b.e[ 6] + a.e[15] * b.e[ 7],
      a.e[ 3] * b.e[ 8] + a.e[ 7] * b.e[ 9] + a.e[11] * b.e[10] + a.e[15] * b.e[11],
      a.e[ 3] * b.e[12] + a.e[ 7] * b.e[13] + a.e[11] * b.e[14] + a.e[15] * b.e[15]);
  }

  V4T<T> Column(size_t index) const {
    size_t offset = 4 * index;
    return V4T<T>(e[offset + 0],
                  e[offset + 1],
                  e[offset + 2],
                  e[offset + 3]);
  }

  T FrobeniusNorm() const {
    T norm = 0;
    for (size_t i = 0; i < 12; ++i)
      norm += e[i] * e[i];
    return Sqrt(norm);
  }

  MatrixT Inverse() const {
    MatrixT inv(e[ 5] * e[10] * e[15] -
                e[ 5] * e[11] * e[14] -
                e[ 9] * e[ 6] * e[15] +
                e[ 9] * e[ 7] * e[14] +
                e[13] * e[ 6] * e[11] -
                e[13] * e[ 7] * e[10],

               -e[ 4] * e[10] * e[15] +
                e[ 4] * e[11] * e[14] +
                e[ 8] * e[ 6] * e[15] -
                e[ 8] * e[ 7] * e[14] -
                e[12] * e[ 6] * e[11] +
                e[12] * e[ 7] * e[10],

                e[ 4] * e[ 9] * e[15] -
                e[ 4] * e[11] * e[13] -
                e[ 8] * e[ 5] * e[15] +
                e[ 8] * e[ 7] * e[13] +
                e[12] * e[ 5] * e[11] -
                e[12] * e[ 7] * e[9],

               -e[ 4] * e[ 9] * e[14] +
                e[ 4] * e[10] * e[13] +
                e[ 8] * e[ 5] * e[14] -
                e[ 8] * e[ 6] * e[13] -
                e[12] * e[ 5] * e[10] +
                e[12] * e[ 6] * e[ 9],

               -e[ 1] * e[10] * e[15] +
                e[ 1] * e[11] * e[14] +
                e[ 9] * e[ 2] * e[15] -
                e[ 9] * e[ 3] * e[14] -
                e[13] * e[ 2] * e[11] +
                e[13] * e[ 3] * e[10],

                e[ 0] * e[10] * e[15] -
                e[ 0] * e[11] * e[14] -
                e[ 8] * e[ 2] * e[15] +
                e[ 8] * e[ 3] * e[14] +
                e[12] * e[ 2] * e[11] -
                e[12] * e[ 3] * e[10],

               -e[ 0] * e[ 9] * e[15] +
                e[ 0] * e[11] * e[13] +
                e[ 8] * e[ 1] * e[15] -
                e[ 8] * e[ 3] * e[13] -
                e[12] * e[ 1] * e[11] +
                e[12] * e[ 3] * e[ 9],

                e[ 0] * e[ 9] * e[14] -
                e[ 0] * e[10] * e[13] -
                e[ 8] * e[ 1] * e[14] +
                e[ 8] * e[ 2] * e[13] +
                e[12] * e[ 1] * e[10] -
                e[12] * e[ 2] * e[ 9],

                e[ 1] * e[ 6] * e[15] -
                e[ 1] * e[ 7] * e[14] -
                e[ 5] * e[ 2] * e[15] +
                e[ 5] * e[ 3] * e[14] +
                e[13] * e[ 2] * e[ 7] -
                e[13] * e[ 3] * e[ 6],

               -e[ 0] * e[ 6] * e[15] +
                e[ 0] * e[ 7] * e[14] +
                e[ 4] * e[ 2] * e[15] -
                e[ 4] * e[ 3] * e[14] -
                e[12] * e[ 2] * e[ 7] +
                e[12] * e[ 3] * e[ 6],

                e[ 0] * e[ 5] * e[15] -
                e[ 0] * e[ 7] * e[13] -
                e[ 4] * e[ 1] * e[15] +
                e[ 4] * e[ 3] * e[13] +
                e[12] * e[ 1] * e[ 7] -
                e[12] * e[ 3] * e[ 5],

               -e[ 0] * e[ 5] * e[14] +
                e[ 0] * e[ 6] * e[13] +
                e[ 4] * e[ 1] * e[14] -
                e[ 4] * e[ 2] * e[13] -
                e[12] * e[ 1] * e[ 6] +
                e[12] * e[ 2] * e[ 5],

               -e[ 1] * e[ 6] * e[11] +
                e[ 1] * e[ 7] * e[10] +
                e[ 5] * e[ 2] * e[11] -
                e[ 5] * e[ 3] * e[10] -
                e[ 9] * e[ 2] * e[ 7] +
                e[ 9] * e[ 3] * e[ 6],

                e[ 0] * e[ 6] * e[11] -
                e[ 0] * e[ 7] * e[10] -
                e[ 4] * e[ 2] * e[11] +
                e[ 4] * e[ 3] * e[10] +
                e[ 8] * e[ 2] * e[ 7] -
                e[ 8] * e[ 3] * e[ 6],

               -e[ 0] * e[ 5] * e[11] +
                e[ 0] * e[ 7] * e[ 9] +
                e[ 4] * e[ 1] * e[11] -
                e[ 4] * e[ 3] * e[ 9] -
                e[ 8] * e[ 1] * e[ 7] +
                e[ 8] * e[ 3] * e[ 5],

                e[ 0] * e[ 5] * e[10] -
                e[ 0] * e[ 6] * e[ 9] -
                e[ 4] * e[ 1] * e[10] +
                e[ 4] * e[ 2] * e[ 9] +
                e[ 8] * e[ 1] * e[ 6] -
                e[ 8] * e[ 2] * e[ 5]);

    T det = e[ 0] * inv[ 0] + e[ 1] * inv[ 4] + e[ 2] * inv[ 8] + e[ 3] * inv[12];
    det = 1 / det;
    for (size_t i = 0; i < 16; ++i)
      inv[i] *= det;
    return inv;
  }

  V4T<T> Row(size_t index) const {
    return V4T<T>(e[index +  0],
                  e[index +  4],
                  e[index +  8],
                  e[index + 12]);
  }

  Ray TransformRay(Ray const& r) const {
    return Ray(TransformV3Norm(r.origin, 1),
               TransformVector(r.direction));
  }

  V3T<T> TransformPoint(V3T<T> const& v) const {
    return V3T<T>(e[ 0]*v[0] + e[ 4]*v[1] + e[ 8]*v[2] + e[12],
                  e[ 1]*v[0] + e[ 5]*v[1] + e[ 9]*v[2] + e[13],
                  e[ 2]*v[0] + e[ 6]*v[1] + e[10]*v[2] + e[14]);
  }

  V3T<T> TransformVector(V3T<T> const& v) const {
    return V3T<T>(e[ 0]*v[0] + e[ 4]*v[1] + e[ 8]*v[2],
                  e[ 1]*v[0] + e[ 5]*v[1] + e[ 9]*v[2],
                  e[ 2]*v[0] + e[ 6]*v[1] + e[10]*v[2]);
  }

  V3T<T> TransformV3(V3T<T> const& v, T w) const {
    return V3T<T>(e[ 0]*v[0] + e[ 4]*v[1] + e[ 8]*v[2] + e[12]*w,
                  e[ 1]*v[0] + e[ 5]*v[1] + e[ 9]*v[2] + e[13]*w,
                  e[ 2]*v[0] + e[ 6]*v[1] + e[10]*v[2] + e[14]*w);
  }

  V3T<T> TransformV3(V3T<T> const& v, T w, T& wOut) const {
    wOut = e[ 3]*v[0] + e[ 7]*v[1] + e[11]*v[2] + e[15]*w;
    return V3T<T>(e[ 0]*v[0] + e[ 4]*v[1] + e[ 8]*v[2] + e[12]*w,
                  e[ 1]*v[0] + e[ 5]*v[1] + e[ 9]*v[2] + e[13]*w,
                  e[ 2]*v[0] + e[ 6]*v[1] + e[10]*v[2] + e[14]*w);
  }

  V3T<T> TransformV3Norm(V3T<T> const& v, T w) const {
    T n = e[ 3]*v[0] + e[ 7]*v[1] + e[11]*v[2] + e[15]*w;
    return V3T<T>(e[ 0]*v[0] + e[ 4]*v[1] + e[ 8]*v[2] + e[12]*w,
                  e[ 1]*v[0] + e[ 5]*v[1] + e[ 9]*v[2] + e[13]*w,
                  e[ 2]*v[0] + e[ 6]*v[1] + e[10]*v[2] + e[14]*w) / n;
  }

  MatrixT Transpose() const {
    return MatrixT(e[ 0], e[ 1], e[ 2], e[ 3],
                   e[ 4], e[ 5], e[ 6], e[ 7],
                   e[ 8], e[ 9], e[10], e[11],
                   e[12], e[13], e[14], e[15]);
  }

  /* Static functions. */

  static MatrixT Frustum(T x1, T x2, T y1, T y2, T z1, T z2) {
    T a =  2 * z1;
    T b = x2 - x1;
    T c = y2 - y1;
    T d = z2 - z1;
    return MatrixT(a / b,     0,  (x2 + x1) / b,             0,
                       0, a / c,  (y2 + y1) / c,             0,
                       0,     0, (-z2 - z1) / d, (-a * z2) / d,
                       0,     0,             -1,             0);
  }

  static MatrixT Identity() {
    return MatrixT(1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   0, 0, 0, 1);
  }

  static MatrixT Look(
    V3T<T> const& eye,
    V3T<T> const& look,
    V3T<T> const& up)
  {
    V3T<T> z = look;
    V3T<T> x = Normalize(Cross(up, z));
    V3T<T> y = Cross(z, x);
    return MatrixT(x.x, x.y, x.z, -Dot(x, eye),
                   y.x, y.y, y.z, -Dot(y, eye),
                   z.x, z.y, z.z, -Dot(z, eye),
                   0, 0, 0, 1);
  }

  static MatrixT LookAt(
    V3T<T> const& eye,
    V3T<T> const& at,
    V3T<T> const& up)
  {
    V3T<T> z = Normalize(at - eye);
    V3T<T> x = Normalize(Cross(up, z));
    V3T<T> y = Cross(z, x);
    return MatrixT(x.x, x.y, x.z, -Dot(x, eye),
                   y.x, y.y, y.z, -Dot(y, eye),
                   z.x, z.y, z.z, -Dot(z, eye),
                   0, 0, 0, 1);
  }

  static MatrixT LookUpRight(
    V3T<T> const& look,
    V3T<T> const& up,
    V3T<T> const& right)
  {
    return MatrixT(right.x, up.x, look.x, 0,
                   right.y, up.y, look.y, 0,
                   right.z, up.z, look.z, 0,
                         0,    0,      0, 1);
  }

  static MatrixT Orthographic(T w, T h, T z1, T z2) {
    T dz = z2 - z1;
    return 
      MatrixT(2 / w,     0,       0,                0,
                  0, 2 / h,       0,                0,
                  0,     0, -2 / dz,  -(z1 + z2) / dz,
                  0,     0,       0,                1) *
      Scale(V3T<T>(1, 1, -1));
  }

  static MatrixT Perspective(T fovy, T aspect, T z1, T z2) {
    T y = z1 * Tan(fovy / 2);
    T x = y * aspect;
    return Frustum(-x, x, -y, y, z1, z2) *
           Scale(V3T<T>(1, 1, -1));
  }

  static MatrixT RotationAxisAngle(V3T<T> const& v, T angle) {
    T c = Cos(angle);
    T cc = 1 - c;
    T s = Sin(angle);
    T e[] = {
      c + v.x*v.x*cc, v.y*v.x*cc + v.z*s, v.z*v.x*cc - v.y*s, 0,
      v.x*v.y*cc - v.z*s, c + v.y*v.y*cc, v.z*v.y*cc + v.x*s, 0,
      v.x*v.z*cc + v.y*s, v.y*v.z*cc - v.x*s, c + v.z*v.z*cc, 0,
      0, 0, 0, 1,
    };
    return MatrixT(e);
  }

  static MatrixT RotationX(T angle) {
    T c = Cos(angle);
    T s = Sin(angle);
    return MatrixT( 1,  0,  0,  0,
                    0,  c, -s,  0,
                    0,  s,  c,  0,
                    0,  0,  0,  1);
  }

  static MatrixT RotationY(T angle) {
    T c = Cos(angle);
    T s = Sin(angle);
    return MatrixT( c,  0,  s,  0,
                    0,  1,  0,  0,
                   -s,  0,  c,  0,
                    0,  0,  0,  1);
  }

  static MatrixT RotationZ(T angle) {
    T c = Cos(angle);
    T s = Sin(angle);
    return MatrixT( c, -s,  0,  0,
                    s,  c,  0,  0,
                    0,  0,  1,  0,
                    0,  0,  0,  1);
  }

  static MatrixT RotationYPR(V3T<T> const& ypr) {
    return MatrixT::RotationY(ypr.x) *
           MatrixT::RotationX(ypr.y) *
           MatrixT::RotationZ(ypr.z);
  }

  static MatrixT Scale(V3T<T> const& v) {
    return MatrixT(v[0],    0,    0,   0,
                      0, v[1],    0,   0,
                      0,    0, v[2],   0,
                      0,    0,    0,   1);
  }

  static MatrixT Translation(V3T<T> const& v) {
    return MatrixT(  1,   0,   0, v[0],
                     0,   1,   0, v[1],
                     0,   0,   1, v[2],
                     0,   0,   0,    1);
  }

  template <class StreamT>
  friend void _ToStream(StreamT& s, MatrixT const& m) {
    s << "Matrix(" << m.e[ 0] << ", " << m.e[ 1] << ", " << m.e[ 2] << ", " << m.e[ 3] << '\n'
      << "       " << m.e[ 4] << ", " << m.e[ 5] << ", " << m.e[ 6] << ", " << m.e[ 7] << '\n'
      << "       " << m.e[ 8] << ", " << m.e[ 9] << ", " << m.e[10] << ", " << m.e[11] << '\n'
      << "       " << m.e[12] << ", " << m.e[13] << ", " << m.e[14] << ", " << m.e[15] << ")";
  }
};

#endif
