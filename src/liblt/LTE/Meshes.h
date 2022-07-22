#ifndef LTE_Meshes_h__
#define LTE_Meshes_h__

#include "DeclareFunction.h"
#include "Math.h"
#include "Mesh.h"
#include "V3.h"

LT_API Mesh Mesh_Billboard(
  float minU = -1,
  float maxU =  1,
  float minV = -1,
  float maxV =  1);

DeclareFunction(Mesh_Box, Mesh,
  uint, resolution,
  bool, closed)

DeclareFunction(Mesh_BoxRounded, Mesh,
  uint, resolution,
  float, radius)

DeclareFunction(Mesh_BoxSphere, Mesh,
  uint, resolution,
  bool, closed)

LT_API Mesh Mesh_Cone(uint slices);

LT_API Mesh Mesh_Cylinder(uint slices);

DeclareFunction(Mesh_Plane, Mesh,
  V3, origin,
  V3, x,
  V3, y,
  uint, cellsX,
  uint, cellsY)

DeclareFunctionNoParams(Mesh_Quad, Mesh)

LT_API Mesh Mesh_SkirtedPlane(int cellsX, int cellsY);

DeclareFunction(Mesh_Sphere, Mesh,
  uint, slices,
  uint, stacks)

LT_API Mesh Mesh_Tetrahedron();

/* F(height, angle) */
template <class fOfHeightAngle>
Mesh Mesh_Polar(
  fOfHeightAngle const& radiusFunction,
  uint slices,
  uint stacks,
  bool closed)
{
  Mesh m = Mesh_Create();
  float invSlices = 1.0f / (float)(slices - 1);
  float invStacks = 1.0f / (float)(stacks - 1);

  if (closed) {
    m->AddVertex(V3(0, 0, 0), V3(0, -1, 0), 0, 0);
    for (uint i = 0; i < slices; ++i)
      m->AddTriangle(0, (i + 1) % slices + 1, i + 1);
  }

  for (uint stack = 0; stack < stacks; ++stack) {
    float y = (float)stack * invStacks;
    for (uint slice = 0; slice < slices; ++slice) {
      float t = (float)slice * invSlices;
      float angle = kTau * t;
      float r = radiusFunction(y, angle);
      float x = r * cosf(angle);
      float z = r * sinf(angle);

      if (slice && stack) {
        uint v = m->GetVertices();
        m->AddQuad(v, v - 1, v - 1 - slices, v - slices);
      }
      m->AddVertex(V3(x, y, z), V3(x, 4*y - 2, z), t, y);
    }
  }

  if (closed) {
    uint v = m->GetVertices();
    uint base = v - slices;
    m->AddVertex(V3(0, 1, 0), V3(0, 1, 0), 1, 0);
    for (uint i = 0; i < slices; ++i)
      m->AddTriangle(v, base + i, base + (i + 1) % slices);
  }
  
  return m;
}

/* F(height, angle) */
template <class fOfHeightAngle>
Mesh Mesh_PolarClosed(
  fOfHeightAngle const& radiusFunction,
  uint slices,
  uint stacks,
  bool closed)
{
  Mesh m = Mesh_Create();

  if (closed) {
    m->AddVertex(V3(0, 0, 0), V3(0, -1, 0), 0, 0);
    for (uint i = 0; i < slices; ++i)
      m->AddTriangle(0, (i + 1) % slices + 1, i + 1);
  }

  for (uint stack = 0; stack < stacks; ++stack) {
    uint baseIndex = m->GetVertices();
    for (uint slice = 0; slice < slices; ++slice) {
      float y = (float)stack / (float)(stacks - 1);
      float t = (float)slice / (float)(slices);
      float angle = kTau * t;
      float r = radiusFunction(y, angle);
      float x = r * cosf(angle);
      float z = r * sinf(angle);

      if (stack) {
        m->AddQuad(
          baseIndex + slice,
          baseIndex + slice - slices,
          baseIndex + (slice + 1) % slices - slices,
          baseIndex + (slice + 1) % slices);
      }

      m->AddVertex(V3(x, y, z), V3(x, 4*y - 2, z), t, y);
    }
  }

  if (closed) {
    uint v = m->GetVertices();
    uint base = v - slices;
    m->AddVertex(V3(0, 1, 0), V3(0, 1, 0), 1, 0);
    for (uint i = 0; i < slices; ++i)
      m->AddTriangle(v, base + i, base + (i + 1) % slices);
  }

  return m;
}

inline Mesh Mesh_PolarPoints(
  std::vector<V3> const& pointRing,
  std::vector<V3> const& radii,
  bool closed)
{
  uint slices = (uint)pointRing.size();
  uint stacks = (uint)radii.size();
  Mesh m = Mesh_Create();

  if (closed) {
    m->AddVertex(V3(0), V3(0, -1, 0), 0, 0);
    for (uint i = 0; i < slices; ++i)
      m->AddTriangle(0, (i + 1) % slices + 1, i + 1);
  }

  float yBase = 0;
  for (uint stack = 0; stack < stacks; ++stack) {
    uint baseIndex = m->GetVertices();
    float r = radii[stack].x;
    for (uint slice = 0; slice < slices; ++slice) {
      float t = (float)slice / (float)(slices);
      const V3& p = pointRing[slice];
      float x = p.x;
      float y = yBase + p.z;
      float z = p.y;
      x *= r;
      z *= r;

      if (stack)
        m->AddQuad(
          baseIndex + slice,
          baseIndex + slice - slices,
          baseIndex + (slice + 1) % slices - slices,
          baseIndex + (slice + 1) % slices);

      m->AddVertex(V3(x, y, z), V3(0), t, y);
    }

    yBase += radii[stack].z;
  }

  if (closed) {
    uint v = m->GetVertices();
    uint base = v - slices;
    m->AddVertex(V3(0, yBase, 0), V3(0, 1, 0), 1, 0);
    for (uint i = 0; i < slices; ++i)
      m->AddTriangle(v, base + i, base + (i + 1) % slices);
  }

  return m;
}

template <class InnerRadiusFnType, class OuterRadiusFnType>
Mesh Mesh_PolarTorus(
  InnerRadiusFnType const& inner,
  OuterRadiusFnType const& outer,
  uint slices,
  uint stacks,
  bool closed)
{
  Mesh m = Mesh_Create();
  float invSlices = 1.0f / (float)(slices - 1);
  float invStacks = 1.0f / (float)(stacks - 1);

  if (closed)
    for (uint i = 0; i < slices; ++i)
      m->AddQuad(2*i, 2*((i + 1) % slices), 2*((i + 1) % slices) + 1, 2*i + 1);

  for (uint stack = 0; stack < stacks; ++stack) {
    float y = (float)stack * invStacks;
    for (uint slice = 0; slice < slices; ++slice) {
      float t = (float)slice * invSlices;
      float angle = kTau * t;
      float r1 = inner(y, angle);
      float r2 = outer(y, angle);

      float cosAngle = Cos(angle);
      float sinAngle = Sin(angle);

      if (slice && stack) {
        uint v = m->GetVertices();
        /* Inner quad. */
        m->AddQuad(v, v - slices*2, v - slices*2 - 2, v - 2);
        v++;
        /* Outer quad. */
        m->AddQuad(v, v - 2, v - 2 - slices*2, v - slices*2);
      }

      {
        float x = r1 * cosAngle;
        float z = r1 * sinAngle;
        m->AddVertex(V3(x, y, z), V3(x, 4*y - 2, z), t, y);
      }
      {
        float x = r2 * cosAngle;
        float z = r2 * sinAngle;
        m->AddVertex(V3(x, y, z), V3(x, 4*y - 2, z), t, y);
      }
    }
  }

  if (closed) {
    uint o = m->GetVertices() - 2 * slices;
    for (uint i = 0; i < slices; ++i)
      m->AddQuad(2*i + o, 2*i + 1 + o,
                 2*((i + 1) % slices) + 1 + o, 2*((i + 1) % slices) + o);
  }

  return m;
}

#endif
