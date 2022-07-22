#ifndef LTE_View_h__
#define LTE_View_h__

#include "Common.h"
#include "Bound.h"
#include "Matrix.h"
#include "Plane.h"
#include "Ray.h"
#include "Transform.h"

struct View {
  typedef PlaneT<double> PlaneType;
  typedef RayT<V3D, V3F> RayType;

  Transform transform;
  float fovy;
  float aspect;
  float zNear;
  float zFar;

  MatrixD view;
  MatrixD proj;
  MatrixD viewProj;
  MatrixD viewProjInverse;
  PlaneType clipPlane[6];

  View(
      Transform const& transform,
      float fovy,
      float aspect,
      float zNear,
      float zFar) :
    transform(transform),
    fovy(fovy),
    aspect(aspect),
    zNear(zNear),
    zFar(zFar)
  {
    Compute();
  }

  bool operator==(View const& other) const {
    return transform == other.transform
        && fovy == other.fovy
        && aspect == other.aspect
        && zNear == other.zNear 
        && zFar == other.zFar;
  }

  PlaneType BuildPlane(
    V3D const& origin,
    V3 const& normal,
    MatrixD const& it)
  {
    V3D ot = viewProjInverse.TransformV3Norm(origin, 1);
    V3D nt = Normalize(it.TransformV3(normal, -1));
    return PlaneType(ot, nt);
  }

  bool CanSee(Bound3D const& box) const {
    for (size_t i = 0; i < 6; ++i) {
      bool visible = false;
      for (size_t j = 0; j < 8; ++j) {
        if (clipPlane[i].PointTest(box[j]) < 0) {
          visible = true;
          break;
        }
      }
      if (visible)
        continue;
      return false;
    }
    return true;
  }

  bool CanSee(V3D const& point) const {
    for (size_t i = 0; i < 6; ++i)
      if (clipPlane[i].PointTest(point) >= 0)
        return false;
    return true;
  }

  void Compute() {
    transform.Orthogonalize();
    view = transform.GetMatrix().Inverse();
    proj = MatrixD::Perspective(fovy, aspect, zNear, zFar);
    viewProj = proj * view;
    viewProjInverse = viewProj.Inverse();
    MatrixD vpT = viewProj.Transpose();

    clipPlane[0] = BuildPlane(V3D(.5f, .5f, 0), V3D( 0,  0, -1), vpT);
    clipPlane[1] = BuildPlane(V3D(.5f, .5f, 1), V3D( 0,  0,  1), vpT);
    clipPlane[2] = BuildPlane(V3D(  1, .5f, 0), V3D(-1,  0,  0), vpT);
    clipPlane[3] = BuildPlane(V3D( -1, .5f, 0), V3D( 1,  0,  0), vpT);
    clipPlane[4] = BuildPlane(V3D(.5f,   1, 0), V3D( 0, -1,  0), vpT);
    clipPlane[5] = BuildPlane(V3D(.5f,  -1, 0), V3D( 0,  1,  0), vpT);
  }

  MatrixD const& GetMatrix() const {
    return viewProj;
  }

  RayType GetRay(V2 const& ndc) const {
    RayType r = GetRayScaled(ndc);
    return RayType(r.origin, Normalize(r.direction));
  }

  RayType GetRayScaled(V2 const& ndc) const {
    V3 u = transform.up;
    V3 l = transform.look / Tan(fovy / 2.0f);
    V3 r = transform.right * aspect;
    return RayType(transform.pos, l + r * ndc.x + u * ndc.y);
  }

  V3D Project(V3D const& v, float vw = 1) const {
    double w;
    V3D projected = viewProj.TransformV3(v, vw, w);
    projected /= w;
    if (w < 0)
      projected.z *= -1;
    return projected;
  }

  V3D Unproject(V3D const& v, float vw = 1) const {
    return viewProjInverse.TransformV3Norm(v, vw);
  }

  template <class StreamT>
  friend void _ToStream(StreamT& s, View const& v) {
    s << "[View]";
  }
};

#endif
