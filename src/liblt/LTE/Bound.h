#ifndef LTE_BoundT_h__
#define LTE_BoundT_h__

#include "AutoClass.h"
#include "Matrix.h"
#include "Plane.h"
#include "Ray.h"
#include "V3.h"

template <class PointT, class DirT>
AutoClass(BoundT,
  PointT, lower,
  PointT, upper)

  typedef MatrixT<typename PointT::MetricType> MatrixType;
  typedef typename PointT::MetricType MetricType;

  BoundT() {}

  BoundT(PointT const& singlePoint) :
    lower(singlePoint),
    upper(singlePoint)
    {}

  template <class PointT2, class DirT2>
  BoundT(BoundT<PointT2, DirT2> const& other) :
    lower((PointT)other.lower),
    upper((PointT)other.upper)
    {}

  static BoundT FromPoints(PointT const& v0, PointT const& v1) {
    return BoundT(Min(v0, v1), Max(v0, v1));
  }

  static BoundT FromPoints(
    PointT const& v0,
    PointT const& v1,
    PointT const& v2)
  {
    return BoundT(Min(v0, Min(v1, v2)), Max(v0, Max(v1, v2)));
  }

  bool operator==(BoundT const& other) const {
    return lower == other.lower &&
           upper == other.upper;
  }

  PointT operator[](size_t index) const {
    return PointT(index % 2 < 1 ? lower.x : upper.x,
                  index % 4 < 2 ? lower.y : upper.y,
                  index % 8 < 4 ? lower.z : upper.z);
  }

  BoundT& AddPoint(PointT const& point) {
    lower = Min(lower, point);
    upper = Max(upper, point);
    return *this;
  }
  
  bool Contains(PointT const& point) const {
    return lower <= point &&
           upper >= point;
  }

  PointT GetCenter() const {
    return (MetricType)0.5 * (upper + lower);
  }

  BoundT GetExpanded(PointT const& factor) const {
    PointT center = GetCenter();
    DirT sideLengths = 0.5f * (GetSideLengths() * factor);
    return BoundT(center - sideLengths, center + sideLengths);
  }
  
  Plane GetFace(size_t index) const {
    LTE_ASSERT(index < 6);
    /* TODO : Unify. */
    if (index == 0)
      return Plane(lower, V3(-1.0f, 0.0f, 0.0f));
    else if (index == 1)
      return Plane(lower, V3( 0.0f,-1.0f, 0.0f));
    else if (index == 2)
      return Plane(lower, V3( 0.0f, 0.0f,-1.0f));
    else if (index == 3)
      return Plane(upper, V3( 1.0f, 0.0f, 0.0f));
    else if (index == 4)
      return Plane(upper, V3( 0.0f, 1.0f, 0.0f));
    else
      return Plane(upper, V3( 0.0f, 0.0f, 1.0f));
  }
  
  BoundT GetIntersection(BoundT const& box) const {
    return BoundT(Max(lower, box.lower),
                 Min(upper, box.upper));
  }

  MetricType GetRadius() const {
    return (upper - lower).GetLength() / 2;
  }

  PointT GetLeftPoint() const {
    return PointT((lower.x),
                  (lower.y + upper.y) / 2,
                  (lower.z + upper.z) / 2);
  }

  PointT GetRightPoint() const {
    return PointT((upper.x),
                  (lower.y + upper.y) / 2,
                  (lower.z + upper.z) / 2);
  }

  PointT GetBottomPoint() const {
    return PointT((lower.x + upper.x) / 2,
                  (lower.y),
                  (lower.z + upper.z) / 2);
  }

  PointT GetTopPoint() const {
    return PointT((lower.x + upper.x) / 2,
                  (upper.y),
                  (lower.z + upper.z) / 2);
  }

  PointT GetBackPoint() const {
    return PointT((lower.x + upper.x) / 2,
                  (lower.y + upper.y) / 2,
                  (upper.z));
  }

  PointT GetFrontPoint() const {
    return PointT((lower.x + upper.x) / 2,
                  (lower.y + upper.y) / 2,
                  (lower.z));
  }

  PointT GetSide(uint index) const {
    switch(index) {
      case 0: return GetLeftPoint();
      case 1: return GetRightPoint();
      case 2: return GetBottomPoint();
      case 3: return GetTopPoint();
      case 4: return GetBackPoint();
      case 5: return GetFrontPoint();
      default: return 0;
    }
  }

  DirT GetSideLengths() const {
    return (DirT)(upper - lower);
  }

  MetricType GetSurfaceArea() const {
    DirT sides = GetSideLengths();
    return 2 * (sides.x * sides.y +
                sides.x * sides.z +
                sides.y * sides.z);
  }

  /* TODO : Verify the projection function, might be wrong. */
  PointT GetProjected(PointT const& point) const {
    PointT projected = point;
    for (size_t i = 0; i < 6; ++i) {
      Plane face = GetFace(i);
      float t = face.PointTest(projected);
      if (t > 0)
        projected -= face.normal * t;
    }
    return projected;
  }

  BoundT GetTransformed(MatrixType const& m) const {
    BoundT newBox(m.TransformPoint((*this)[0]));
    for (size_t i = 1; i < 8; ++i)
      newBox.AddPoint(m.TransformPoint((*this)[i]));
    return newBox;
  }
    
  BoundT GetUnion(BoundT const& box) const {
    return BoundT(Min(lower, box.lower), Max(upper, box.upper));
  }

  float GetVolume() const {
    return GetSideLengths().Product();
  }

  bool Intersects(
    PointT const& rayOrigin,
    DirT const& rcpRayDirection,
    float& near,
    float& far) const
  {
    DirT t0 = (DirT)(lower - rayOrigin) * rcpRayDirection;
    DirT t1 = (DirT)(upper - rayOrigin) * rcpRayDirection;
    near = Min(t0, t1).GetMax();
    far = Max(t0, t1).GetMin();
    return near <= far;
  }

  BoundT& Intersect(BoundT const& other) {
    lower = Max(lower, other.lower);
    upper = Min(upper, other.upper);
    upper = Max(upper, lower);
    return *this;
  }

  bool Intersects(Ray const& ray, float& t) const {
    DirT invDirection = DirT(1) / ray.direction;
    DirT t1 = (DirT)(lower - ray.origin) * invDirection;
    DirT t2 = (DirT)(upper - ray.origin) * invDirection;
    float tNear = Min(t1, t2).GetMax();
    float tFar = Max(t1, t2).GetMin();

    if (tNear <= tFar) {
      t = tNear;
      return true;
    }

    return false;
  }
  
  bool Intersects(Plane const& plane) const {
    float p0 = Sign(plane.PointTest((*this)[0]));
    for (size_t i = 1; i < 8; ++i)
      if (Sign(plane.PointTest((*this)[i])) != p0)
        return true;
    return false;
  }
    
  /* TODO : This is not complete. */
  bool IntersectsTri(
    PointT const& v0,
    PointT const& v1,
    PointT const& v2) const
  {
    DirT n = Cross(v1 - v0, v2 - v0);
    /* TODO : Why do I have to write out the full plane test instead of
              precomputing w? Doesn't work when I do that...! */
    bool side = Dot(n, (*this)[0] - v0) > 0;
    for (size_t i = 1; i < 8; ++i) {
      if ((Dot(n, (*this)[i] - v0) > 0) != side)
        return true;
    }
    return false;
  }

  bool Overlaps(BoundT const& box) const {
    return lower.x < box.upper.x
        && lower.y < box.upper.y
        && lower.z < box.upper.z
        && upper.x > box.lower.x
        && upper.y > box.lower.y
        && upper.z > box.lower.z;
  }
  
  PointT Sample(DirT const& t) const {
    return PointT(
      Mix(lower.x, upper.x, t.x),
      Mix(lower.y, upper.y, t.y),
      Mix(lower.z, upper.z, t.z));
  }
    
  BoundT& Union(BoundT const& box) {
    lower = Min(lower, box.lower);
    upper = Max(upper, box.upper);
    return *this;
  }
};

#endif
