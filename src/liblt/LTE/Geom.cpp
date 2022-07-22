#include "Geom.h"
#include "Bound.h"
#include "Math.h"
#include "Matrix.h"
#include "Mesh.h"
#include "Meshes.h"
#include "Plane.h"

namespace {
  float ConstantRadius(float h, float angle) {
    return 1.0f;
  }
}

namespace LTE {
  Mesh Geom_Box(float sx, float sy, float sz) {
    return Mesh_Box(2, true)->Scale(V3(sx, sy, sz));
  }

  Mesh Geom_Cylinder(uint slices) {
    return Mesh_PolarClosed(ConstantRadius, slices, 2, true)->
          Rotate(V3(kPi / (float)slices, 0, 0));
  }

  Mesh Geom_DisplacePlane(Mesh const& m, Plane const& p, float amount) {
    for (uint i = 0; i < m->vertices.size(); ++i) {
      Vertex& v = m->vertices[i];
      v.p += amount * Sign(p.PointTest(v.p)) * p.normal;
    }
    return m;
  }

  Mesh Geom_Expand(Mesh const& m, V3 const& origin, float x, float y, float z) {
    Geom_DisplacePlane(m, Plane(origin, V3(1, 0, 0)), x);
    Geom_DisplacePlane(m, Plane(origin, V3(0, 1, 0)), y);
    Geom_DisplacePlane(m, Plane(origin, V3(0, 0, 1)), z);
    return m;
  }

  Mesh Geom_MakeSymmetric(Mesh const& m, const V3& plane) {
    return m->AddMesh(Geom_Mirror(m->Clone(), plane));
  }

  Mesh Geom_MakeSymmetricAngular(Mesh const& m, uint count, V3 const& axis) {
    Mesh result = Mesh_Create();
    for (uint i = 0; i < count; ++i) {
      float angle = kTau * (float)i / (float)count;
      Matrix matrix = Matrix::RotationAxisAngle(Normalize(axis), angle);
      result->AddMesh(m, matrix);
    }
    return m->Clear()->AddMesh(result);
  }

  Mesh Geom_Mirror(Mesh const& m, V3 const& mirrorPlane) {
    return m->ReverseWinding()->Scale(mirrorPlane);
  }

  void Geom_MirrorPointRing(Vector<V3>& pointRing) {
    uint points = pointRing.size();
    uint baseIndex = points - 1;
    for (uint i = 0; i < points; ++i) {
      V3 v = pointRing[baseIndex - i];
      v.x *= -1.0f;
      pointRing.push(v);
    }
  }

  Mesh Geom_PolyBox(uint sides, float sx, float sy, float sz) {
    Mesh m = Geom_Cylinder(sides)->Transform(
      Matrix::Translation(V3(0, 0, -.5f)) * Matrix::RotationX(kPi2));

    m->Scale(V3(1, 1, sz + 1));
    Geom_DisplacePlane(m, Plane(V3(0.f), V3(0, 1, 0)), sy);
    Geom_DisplacePlane(m, Plane(V3(0.f), V3(1, 0, 0)), sx);
    return m;
  }

  Mesh Geom_Repeat(
    Mesh const& m,
    uint count,
    V3 const& direction,
    float spacing)
  {
    Mesh result = Mesh_Create();
    Bound3 box = m->GetBound();
    V3 step = spacing * (box.GetSideLengths() * direction);
    V3 offset = V3(0);
    for (uint i = 0; i < count; ++i) {
      result->AddMesh(m, Matrix::Translation(offset));
      offset += step;
    }
    return m->Clear()->AddMesh(result);
  }

  Mesh Geom_Retarget(Mesh const& m, Bound3 const& targetBox) {
    Bound3 currentBox = m->GetBound();
    return m
      ->Translate(-currentBox.GetCenter())
      ->Scale(targetBox.GetSideLengths() / currentBox.GetSideLengths())
      ->Translate(targetBox.GetCenter());
  }

  Mesh Geom_RingMesh(
    uint slices,
    uint pointCount,
    float bevelSize,
    float bevelDepth,
    float ringSpacing,
    float ringMinWidth,
    float ringMaxWidth,
    float edgeBevel,
    bool symmetricRing)
  {
    Vector<V3> points;
    Vector<V3> radii;
    radii.push(V3(0, 0, bevelDepth));

    for (uint i = 0; i < slices; ++i) {
      float thisWidth = Rand(ringMinWidth, ringMaxWidth);
      radii.push(V3(thisWidth, 0, ringSpacing));
      radii.push(V3(thisWidth, 0, ringSpacing / 8));
    }

    radii.back().z = bevelDepth;
    radii.push(V3(radii.back().x - bevelSize, 0, 0));
    radii.front().x = radii[1].x - bevelSize;

    for (uint i = 0; i < pointCount; ++i) {
      float t = ((float)i + 0.5f) / (float)pointCount;
      float angle = (symmetricRing ? kPi : kTau) * t - kPi2;
      V3 randomOff = RandV3(-0.2f, 0.2f);
      randomOff.z = 0;
      points.push(V3(Polar(angle), 0) + randomOff);
      points.push(V3(Polar(angle + edgeBevel), 0) + randomOff);
    }

    if (symmetricRing)
      Geom_MirrorPointRing(points);

    return Mesh_PolarPoints(points, radii, true)
      ->Rotate(V3(0, kPi2, 0))
      ->ComputeNormals();
  }

  Mesh Geom_SmoothHull(Mesh const& source, uint quality, float radius) {
    Bound3 box = source->GetBound();
    V3 center = box.GetCenter();
    Mesh hull = Mesh_BoxSphere(quality, true)
      ->Scale(box.GetRadius() * 1.5f)
      ->Translate(center);

    for (size_t j = 0; j < hull->vertices.size(); ++j) {
      V3& pos = hull->vertices[j].p;
      V3 closest = 0;
      float minDist = FLT_MAX;
      for (size_t k = 0; k < source->vertices.size(); ++k) {
        V3& p = source->vertices[k].p;
        float dist = LengthSquared(pos - p);
        if (dist < minDist) {
          minDist = dist;
          closest = p;
        }
      }
      pos = closest + Normalize(pos - center) * radius;
    }
    return hull;
  }

  Mesh Geom_Sphere(uint stacks, uint slices) {
    Mesh m = Mesh_Create();
    for (uint i = 0; i < slices; ++i)
      m->AddTriangle(0, (i + 1) % slices + 1, i + 1);
    m->AddVertex(V3(0, -1, 0), V3(0, -1, 0), 0, 0);

    for (uint i = 0; i < stacks; ++i) {
      uint off = m->GetVertices();
      for (uint j = 0; j < slices; ++j) {
        float h = (float)(i + 1) / (float)(stacks + 1);
        float t = (float)j / (float)slices;
        float y = 2*h - 1;
        float r = Sqrt(1 - y*y);
        float x = r * Cos(kTau * t);
        float z = r * Sin(kTau * t);

        if (i)
          m->AddQuad(off + j,
                     off + j - slices,
                     off + (j + 1) % slices - slices,
                     off + (j + 1) % slices);
        m->AddVertex(V3(x, y, z), V3(x, y, z), h, t);
      }
    }

    uint off = m->GetVertices();
    m->AddVertex(V3(0, 1, 0), V3(0, 1, 0), 1, 0);
    for (uint i = 0; i < slices; ++i)
      m->AddTriangle(off, off - slices + i, off - slices + (i + 1) % slices);
    return m;
  }
}
