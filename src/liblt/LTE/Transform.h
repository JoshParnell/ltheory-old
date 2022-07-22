#ifndef LTE_Transform_h__
#define LTE_Transform_h__

#include "AutoClass.h"
#include "DeclareFunction.h"
#include "Matrix.h"

const float kSmallFloat = 1e-6f;

AutoClass(Transform,
  V3D, pos,
  V3F, right,
  V3F, up,
  V3F, look,
  V3F, scale)

  Transform() :
    pos(0),
    right(1, 0, 0),
    up(0, 1, 0),
    look(0, 0, 1),
    scale(1)
    {}

  bool operator==(Transform const& other) const {
    return pos == other.pos
        && right == other.right
        && up == other.up
        && look == other.look
        && scale == other.scale;
  }

  MatrixD GetMatrix() const {
    return MatrixD(
      scale.x * right.x, scale.y * up.x, scale.z * look.x, pos.x,
      scale.x * right.y, scale.y * up.y, scale.z * look.y, pos.y,
      scale.x * right.z, scale.y * up.z, scale.z * look.z, pos.z,
      0, 0, 0, 1);
  }

  V3F InverseDir(V3F const& axis) const {
    return V3F(Dot(axis, right), Dot(axis, up), Dot(axis, look));
  }

  V3D InversePoint(V3D const& p) const {
    return InverseVector((V3F)(p - pos));
  }

  V3F InverseVector(V3F const& v) const {
    return InverseDir(v) / scale;
  }

  void Orthogonalize() {
    Orthogonalize1(look);
    Orthogonalize2(look, up);
    Orthogonalize3(look, up, right);
  }

  void Orthogonalize1(V3& v1) {
    float l = LengthSquared(v1);
    v1 = l >= kSmallFloat ? v1 / Sqrt(l) : V3F(0, 0, 1);
  }

  void Orthogonalize2(V3& v1, V3& v2) {
    if (Abs(Dot(v1, v2) - LengthSquared(v2)) < kSmallFloat)
      v2 = (v1.x * v1.x < 0.9f) ? V3F(1, 0, 0) : V3F(0, 1, 0);
  }

  void Orthogonalize3(V3& v1, V3& v2, V3& v3) {
    v2 = Normalize(Cross(v1, Cross(v2, v1)));
    v3 = Cross(v2, v1);
  }

  V3F TransformDir(V3F const& v) const {
    return v.x * right
         + v.y * up
         + v.z * look;
  }

  V3D TransformPoint(V3D p) const {
    p *= scale;
    return p.x * (V3D)right
         + p.y * (V3D)up
         + p.z * (V3D)look
         + pos;
  }

  V3F TransformVector(V3F const& v) const {
    return TransformDir(v * scale);
  }
};

LT_API Transform operator*(Transform const&, Transform const&);
LT_API Transform Mix(Transform const&, Transform const&, double);

DeclareFunctionNoParams(Transform_Identity, Transform)

DeclareFunction(Transform_Inverse, Transform,
  Transform, source)

DeclareFunction(Transform_Look, Transform,
  V3D, pos,
  V3F, look)

DeclareFunction(Transform_LookUp, Transform,
  V3D, pos,
  V3F, look,
  V3F, up)

DeclareFunction(Transform_Matrix, Transform,
  MatrixD, matrix)

DeclareFunction(Transform_Rotate, void,
  Transform, source,
  V3F, rotation)

DeclareFunction(Transform_Scale, Transform,
  V3F, scale)

DeclareFunction(Transform_Translation, Transform,
  V3D, pos)

DeclareFunction(Transform_ST, Transform,
  V3F, scale,
  V3D, pos)

#endif
