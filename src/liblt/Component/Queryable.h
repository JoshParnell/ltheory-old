#ifndef Component_Queryable_h__
#define Component_Queryable_h__

#include "Common.h"
#include "LTE/AutoClass.h"
#include "LTE/Pointer.h"
#include "LTE/V3.h"

struct ComponentQueryableImpl;

AutoClassEmpty(ComponentQueryable)
  Pointer<ComponentQueryableImpl> impl;

  LT_API ComponentQueryable();
  LT_API ~ComponentQueryable();

  LT_API void Remove(ObjectT* object);

  LT_API void Run(ObjectT* self, UpdateState& state);

  LT_API void QueryBox(
    ObjectT* self,
    Bound3D const& box,
    Vector<ObjectT*>& objects);

  LT_API ObjectT* QueryRay(
    ObjectT* self,
    WorldRay const& ray,
    float& t,
    float tMax,
    V3* normalOut,
    bool accel,
    bool (*check)(ObjectT const*, void*),
    void* aux);
};

AutoComponent(Queryable)
  void OnUpdate(UpdateState& s) {
    Queryable.Run(this, s);
    BaseT::OnUpdate(s);
  }

  void QueryInterior(
    Bound3D const& box,
    Vector<ObjectT*>& objects)
  {
    return Queryable.QueryBox(this, box, objects);
  }

  ObjectT* QueryInterior(
    WorldRay const& ray,
    float& t,
    float tMax,
    V3* normalOut,
    bool accel,
    bool (*check)(ObjectT const*, void*),
    void* aux)
  {
    return Queryable.QueryRay(this, ray, t, tMax, normalOut, accel, check, aux);
  }
};

/* Pre-defined callbacks for use with Query. */
LT_API bool RaycastCanCollide(ObjectT const*, void*);
LT_API bool RaycastCanCollideBidirectional(ObjectT const*, void*);
LT_API bool RaycastSolids(ObjectT const*, void*);

#endif
