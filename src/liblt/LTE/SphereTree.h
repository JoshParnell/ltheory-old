#ifndef LTE_SphereTree_h__
#define LTE_SphereTree_h__

#include "AutoClass.h"
#include "Reference.h"
#include "Sphere.h"
#include "Vector.h"

#if 0

const float kTriangleCost = 1.0f;
typedef Reference<struct SphereTreeT> SphereTree;

AutoClass(Triangle,
  V3, p1,
  V3, p2,
  V3, p3)
  Triangle() {}

  V3& operator[](size_t index) {
    return ((V3*)this)[index];
  }

  V3 const& operator[](size_t index) const {
    return ((V3 const*)this)[index];
  }
};

struct SphereTreeT : public RefCounted {
  Sphere bound;
  Vector<SphereTree> children;

  void ComputeBound() {
    V3 center = 0;
    for (size_t i = 0; i < elements.size(); ++i) {
      Triangle const& element = elements[i];
      for (size_t j = 0; j < 3; ++j)
        center += element[j];
    }
    center /= (float)(3 * elements.size());

    float radius = 0;
    for (size_t i = 0; i < elements.size(); ++i) {
      Triangle const& element = elements[i];
      for (size_t j = 0; j < 3; ++j)
        radius = Max(radius, Length(center - element[j]));
    }

    for (size_t i = 0; i < children.size(); ++i) {
      SphereTree const& child = children[i];
      radius = Max(radius,
        Length(center - child->bound.center) + child->bound.radius);
    }

    bound = Sphere(center, radius);
  }

  float GetCost() {
    float cost = (float)elements.size() * kTriangleCost;
    float volume = GetVolume();
    for (size_t i = 0; i < children.size(); ++i) {
      float fraction = children[i]->GetVolume() / volume;
      cost += fraction * children[i]->GetCost();
    }
    return cost;
  }

  float GetVolume() const {
    return bound.GetVolume();
  }
};

LT_API SphereTree SphereTree_Mesh(Mesh const& mesh);

#endif
#endif
