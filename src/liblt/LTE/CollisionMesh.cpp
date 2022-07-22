#include "CollisionMesh.h"

#include "Array.h"
#include "AutoPtr.h"
#include "Bound.h"
#include "Matrix.h"
#include "Mesh.h"
#include "SpatialPartition.h"
#include "SpatialSignature.h"
#include "Sphere.h"

const bool kUseHashGrid = false;
const bool kUseSpatialSignatures = true;
const float kGridResolution = 1;
const float kSignatureResolution = 4;

extern bool TriTriOverlap(
  V3 const& V0,
  V3 const& V1,
  V3 const& V2,
  V3 const& U0,
  V3 const& U1,
  V3 const& U2);

namespace {
  struct PartitionAddData {
    V3 v0, v1, v2;
  };

  /* TODO : Make the box - triangle test more precise! */
  bool PartitionAddCallback(Bound3 const& box, void* aux) {
    PartitionAddData& ad = *(PartitionAddData*)aux;
    return box.IntersectsTri(ad.v0, ad.v1, ad.v2);
  }

  struct MeshIntersectionData {
    Bound3* localBox;
    Sphere* localSphere;
    V3 v0, v1, v2;
    bool intersected;
    Array<V3> const& targetVerts;
    Array<Bound3> const& targetBoxes;
    size_t targetID;

    MeshIntersectionData(
        Array<V3> const& targetVerts,
        Array<Bound3> const& targetBoxes) :
      targetVerts(targetVerts),
      targetBoxes(targetBoxes)
      {}
  };

  bool MeshIntersectionCallback(void* object, void* aux) {
    MeshIntersectionData& id = *(MeshIntersectionData*)aux;
    size_t index = (size_t)object;
    V3 const& u0 = id.targetVerts[index + 0];
    V3 const& u1 = id.targetVerts[index + 1];
    V3 const& u2 = id.targetVerts[index + 2];

    if (!id.localBox->Overlaps(id.targetBoxes[index / 3]))
      return true;

    if (!TriTriOverlap(id.v0, id.v1, id.v2, u0, u1, u2))
      return true;

    id.intersected = true;
    id.targetID = index;
    return false;
  }

  struct RayIntersectionData {
    Ray ray;
    V3 invRayD;
    V3 rayDNorm;
    float tMin, tMax;
    Array<V3> const& verts;
    Array<Bound3> const& boxes;
    size_t hits;
    V3* normalOut;

    RayIntersectionData(
        Ray const& ray,
        float tMax,
        Array<V3> const& verts,
        Array<Bound3> const& boxes,
        V3* normalOut) :
      ray(ray),
      tMin(FLT_MAX),
      tMax(tMax),
      verts(verts),
      boxes(boxes),
      hits(0),
      normalOut(normalOut)
    {
      invRayD = V3(1) / ray.direction;
      rayDNorm = Normalize(ray.direction);
    }
  };

  bool RayIntersectionCallback(void* object, void* aux) {
    RayIntersectionData& rd = *(RayIntersectionData*)aux;
    // TODO : Playing with fire...
    size_t index = (size_t)object;
    V3 const& u0 = rd.verts[index + 0];
    V3 const& u1 = rd.verts[index + 1];
    V3 const& u2 = rd.verts[index + 2];
    float t, tFar;

    if (!(rd.boxes[index / 3].Intersects(rd.ray.origin, rd.invRayD, t, tFar)
          && t <= rd.tMax))
      return true;

    if (!(rd.ray.IntersectsTri(u0, u1, u2, t) && t <= rd.tMax))
      return true;

    if (t < rd.tMin) {
      rd.tMin = t;
      if (rd.normalOut)
        *rd.normalOut = Normalize(Cross(u2 - u1, u1 - u0));
    }
    rd.hits++;
    return true;
  }

  struct CollisionMeshImpl : public CollisionMeshT {
    Bound3 box;
    Array<V3> vertices;
    Array<Bound3> boxes;
    AutoPtr<SpatialPartition> partition;
    AutoPtr<SpatialSignature> signature;

    CollisionMeshImpl(Mesh const& sourceMesh) {
      Mesh cleaned = sourceMesh->RemoveDegeneracies();
      vertices.resize(cleaned->indices.size());
      for (size_t i = 0; i < vertices.size(); ++i)
        vertices[i] = cleaned->vertices[cleaned->indices[i]].p;

      box = cleaned->GetBound();
      ComputeTriBoxes(cleaned);
      CreateAccelerators();
    }

    void CreateAccelerators() {
      size_t triangles = vertices.size() / 3;
      V3 span = box.GetSideLengths();
      V3 resMult = span / span.GetGeometricAverage();
      resMult *= pow((float)triangles, 1.f / 3.f);

      if (kUseHashGrid)
        partition = SpatialPartition_Hash(span / 10.f, triangles);
      else
        partition = SpatialPartition_Uniform(box,
          Max((size_t)(resMult.x * kGridResolution), (size_t)1),
          Max((size_t)(resMult.y * kGridResolution), (size_t)1),
          Max((size_t)(resMult.z * kGridResolution), (size_t)1));

      if (kUseSpatialSignatures)
        signature = SpatialSignature_Grid(box,
          (size_t)(resMult.x * kSignatureResolution),
          (size_t)(resMult.y * kSignatureResolution),
          (size_t)(resMult.z * kSignatureResolution));

      for (size_t i = 0; i < triangles; ++i) {
        PartitionAddData ad;
        ad.v0 = vertices[i*3 + 0];
        ad.v1 = vertices[i*3 + 1];
        ad.v2 = vertices[i*3 + 2];
        partition->Add((void*)(i * 3), boxes[i], PartitionAddCallback, &ad);

        if (kUseSpatialSignatures)
          signature->Add((void*)(i * 3), boxes[i], PartitionAddCallback, &ad);
      }
    }

    void ComputeTriBoxes(Mesh const& source) {
      boxes.resize(source->GetTriangles());
      for (size_t i = 0; i < source->indices.size(); i += 3)
        boxes[i / 3] = Bound3::FromPoints(
          source->vertices[source->indices[i + 0]].p,
          source->vertices[source->indices[i + 1]].p,
          source->vertices[source->indices[i + 2]].p);
    }

    size_t GetMemoryUsage() const {
      size_t memory = sizeof(*this);
      memory += vertices.memory();
      memory += boxes.size();
      memory += partition->GetMemoryUsage();
      memory += signature ? signature->GetMemoryUsage() : 0;
      return memory;
    }

    V3 ComputeContactNormal(
      V3 const& v0, V3 const& v1, V3 const& v2,
      V3 const& u0, V3 const& u1, V3 const& u2) const
    {
      return Normalize(Cross(v2 - v1, v1 - v0) - Cross(u2 - u1, u1 - u0));
    }

    bool Intersects(
      CollisionMesh const& m,
      Matrix const& myWorld,
      Matrix const& otherWorld,
      V3* contactNormal) const
    {
      CollisionMeshImpl const& other = *(CollisionMeshImpl const*)m.t;

      /* As a heuristic, we collide the object with the smaller bounding
         surface area against the object with the larger. */
      bool sizeComp = box.GetTransformed(myWorld).GetSurfaceArea() <=
                      other.box.GetTransformed(otherWorld).GetSurfaceArea();
      CollisionMeshImpl const& source = sizeComp ? *this : other;
      CollisionMeshImpl const& target = sizeComp ? other : *this;
      Matrix const& sourceTransform = sizeComp ? myWorld : otherWorld;
      Matrix const& targetTransform = sizeComp ? otherWorld : myWorld;

      Matrix toTarget = targetTransform.Inverse() * sourceTransform;
      MeshIntersectionData id(target.vertices, target.boxes);

      id.intersected = false;

      for (size_t i = 0; i < source.vertices.size(); i += 3) {
        id.v0 = toTarget.TransformPoint(source.vertices[i + 0]);
        id.v1 = toTarget.TransformPoint(source.vertices[i + 1]);
        id.v2 = toTarget.TransformPoint(source.vertices[i + 2]);
        Bound3 triBox = Bound3::FromPoints(id.v0, id.v1, id.v2);

        /* Check the local triangle box against the target's bounding box. */
        if (!triBox.Overlaps(target.box))
          continue;

        /* Check against the spatial signature of the target. */
        if (kUseSpatialSignatures && !target.signature->Query(triBox))
          continue;

        id.localBox = &triBox;
        target.partition->Query(triBox, MeshIntersectionCallback, &id);

        if (id.intersected) {
          if (contactNormal) {
            *contactNormal = ComputeContactNormal(
              sourceTransform.TransformPoint(source.vertices[i + 0]),
              sourceTransform.TransformPoint(source.vertices[i + 1]),
              sourceTransform.TransformPoint(source.vertices[i + 2]),
              targetTransform.TransformPoint(target.vertices[id.targetID + 0]),
              targetTransform.TransformPoint(target.vertices[id.targetID + 1]),
              targetTransform.TransformPoint(target.vertices[id.targetID + 2]));
            if (!sizeComp)
              *contactNormal *= -1.f;
          }
          return true;
        }
      }

      return false;
    }

    bool Intersects(
      Ray const& ray,
      Matrix const& myWorld,
      float tMax,
      float& tOut,
      V3* normalOut) const
    {
      RayIntersectionData id(myWorld.Inverse().TransformRay(ray),
                             tMax, vertices, boxes, normalOut);
      partition->Query(id.ray, tMax, RayIntersectionCallback, &id);
      tOut = id.tMin;
      return id.hits > 0;
    }
  };
}

CollisionMesh CollisionMesh_Create(Mesh const& sourceMesh) {
  return new CollisionMeshImpl(sourceMesh);
}
