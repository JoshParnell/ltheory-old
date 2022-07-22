#include "Queryable.h"

#include "BoundingBox.h"
#include "Interior.h"

#include "Game/Object.h"

#include "Module/PhysicsEngine.h"

#include "LTE/AutoPtr.h"
#include "LTE/Ray.h"
#include "LTE/SpatialPartition.h"
#include "LTE/StackFrame.h"
#include "LTE/Vector.h"

const size_t kPartitionLevels = 2;
const size_t kPartitionBuckets = 4096;
const size_t kPartitionStaticMult = 2;
const size_t kPartitionCellSize = 1024;

/* Right now, should be sufficient to use two levels for SH: one for static
   objects and one for dynamic. If necessary, we can use multiple grid sizes for
   dyanmic objects, but the gain is questionable. */
const float kSpatialGridSizes[kPartitionLevels] = {
  kPartitionCellSize,
  kPartitionCellSize,
};

namespace {
  struct RayMarchData {
    WorldRay ray;
    V3 invRayD;
    V3* normalOut;
    float tMin;
    float tMax;
    bool (*checkIntersection)(ObjectT const*, void*);
    void* aux;
    Pointer<ObjectT> closest;

    RayMarchData(
        WorldRay const& ray,
        float tMax,
        V3* normalOut,
        bool (*checkIntersection)(ObjectT const*, void*), void* aux) :
      ray(ray),
      normalOut(normalOut),
      tMin(FLT_MAX),
      tMax(tMax),
      checkIntersection(checkIntersection),
      aux(aux)
    {
      invRayD = V3(1) / ray.direction;
    }
  };

  bool RaymarchCallback(void* object, void* aux) {
    RayMarchData& rd = *(RayMarchData*)aux;

    float tFar;
    float t;
    V3 normal;

    ObjectT* o = (ObjectT*)object;
    if (rd.checkIntersection
        && !rd.checkIntersection(o, rd.aux))
      return true;

    Bound3D const& bound = o->GetGlobalBound();
    if (bound.Intersects(rd.ray.origin, rd.invRayD, t, tFar)
        && t <= rd.tMax)
    {
      if (GetPhysicsEngine()->Raycast(
            rd.ray, o, rd.tMax, t, rd.normalOut ? &normal : nullptr)
          && t < rd.tMin
          && t <= rd.tMax)
      {
        rd.closest = o;
        rd.tMin = t;
        if (rd.normalOut)
          *rd.normalOut = normal;
      }
    }
    return true;
  }
}

struct ComponentQueryableImpl {
  Vector<void*> queryVector;
  AutoPtr<SpatialPartition> partition[kPartitionLevels];

  short queryVersion;
  bool dirty;

  ComponentQueryableImpl() :
    queryVersion(0),
    dirty(true)
    {}

  void Refresh(ObjectT* self) {
    if (!dirty)
      return;
    dirty = false;

    if (!partition[0])
      for (size_t i = 0; i < kPartitionLevels; ++i)
        partition[i] = SpatialPartition_Hash(V3(kSpatialGridSizes[i]),
            (i == 0 ? kPartitionStaticMult : 1) * kPartitionBuckets);

    for (size_t i = 1; i < kPartitionLevels; ++i)
      partition[i]->Clear();

    for (InteriorIterator it = Object_GetInteriorObjects(self);
         it.HasMore(); it.Advance())
      TraverseObject(it.Get());
  }

  void Remove(ObjectT* o) {
    ComponentBoundingBox* bb = o->GetBoundingBox();
    if (bb && bb->inSpatialGrid >= 0) {
      partition[bb->inSpatialGrid]->Remove(o, o->GetGlobalBound());
      dirty = true;
      bb->inSpatialGrid = -1;
    }
  }

  void TraverseObject(ObjectT* o) {
    ComponentBoundingBox* bb = o->GetBoundingBox();
    if (bb && bb->inSpatialGrid != 0) {
      size_t level = 0;

      /* Movable objects are eligible to be put in special partitions to
       * reduce cost of adding to grid. */
      if (o->IsMovable()) {
        Bound3D const& bound = o->GetGlobalBound();
        float maxSize = bound.GetSideLengths().GetMax();
        for (level = 1; level < kPartitionLevels; ++level)
          if (maxSize < kSpatialGridSizes[level])
            break;

        if (level == kPartitionLevels)
          level = kPartitionLevels - 1;
      }

      partition[level]->Add(o, o->GetGlobalBound());
      bb->inSpatialGrid = (short)level;
    }

#if 0
    for (ObjectT* child = o->children; child; child = child->nextSibling)
      TraverseObject(child);
#endif
  }

  /* Bound query. */
  void QueryBox(
    ObjectT* self,
    Bound3D const& box,
    Vector<ObjectT*>& objects)
  {
    SFRAME("AABB Query");
    Refresh(self);

    queryVersion++;
    queryVector.clear();

    for (size_t i = 0; i < kPartitionLevels; ++i)
      partition[i]->Query(box, queryVector);

    /* Use the bounding box's query version to perform duplicate checking. */
    for (size_t i = 0; i < queryVector.size(); ++i) {
      ObjectT* o = (ObjectT*)queryVector[i];
      ComponentBoundingBox* bb = o->GetBoundingBox();
      if (bb->queryVersion != queryVersion) {
        bb->queryVersion = queryVersion;
        objects.push(o);
      }
    }
  }

  /* Ray query. */
  ObjectT* QueryRay(
    ObjectT* self,
    WorldRay const& ray,
    float& outT,
    float tMax,
    V3* normalOut,
    bool accelerate,
    bool (*checkIntersection)(ObjectT const*, void*),
    void* aux)
  {
    SFRAME("Ray Query");
    Refresh(self);
    RayMarchData rd(ray, tMax, normalOut, checkIntersection, aux);

    V3 normal;

    float rayMagnitude = Length(ray.direction);
    if (rayMagnitude < 1e-6f || !ray.direction.IsFinite())
      return nullptr;

    float marchDistance = tMax * rayMagnitude;

    LTE_ASSERT(marchDistance < 100000);

    /* TODO : High-precision querying. */
    for (size_t i = 0; i < kPartitionLevels; ++i)
      partition[i]->Query(rd.ray, rd.tMax, RaymarchCallback, &rd);

    outT = rd.tMin;
    return rd.closest;
  }
};

ComponentQueryable::ComponentQueryable() :
  impl(new ComponentQueryableImpl)
  {}

ComponentQueryable::~ComponentQueryable() {
  delete impl;
}

void ComponentQueryable::Remove(ObjectT* object) {
  impl->Remove(object);
}

void ComponentQueryable::Run(ObjectT* self, UpdateState& state) {
  impl->dirty = true;
  impl->Refresh(self);
}

void ComponentQueryable::QueryBox(
  ObjectT* self,
  Bound3D const& box,
  Vector<ObjectT*>& objects)
{
  return impl->QueryBox(self, box, objects);
}

ObjectT* ComponentQueryable::QueryRay(
  ObjectT* self,
  WorldRay const& ray,
  float& t,
  float tMax,
  V3* normalOut,
  bool accel,
  bool (*check)(ObjectT const*, void*),
  void* aux)
{
  return impl->QueryRay(self, ray, t, tMax, normalOut, accel, check, aux);
}

bool RaycastCanCollide(ObjectT const* object, void* other) {
  return object->CanCollide((ObjectT const*)other);
}

bool RaycastCanCollideBidirectional(ObjectT const* object, void* vOther) {
  ObjectT const* other = (ObjectT const*)vOther;
  return object->CanCollide(other) && other->CanCollide(object);
}

bool RaycastSolids(ObjectT const* object, void*) {
  ObjectType type = object->GetType();
  return type != ObjectType_Shield;
}
