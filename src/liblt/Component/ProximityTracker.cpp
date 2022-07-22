#include "BoundingBox.h"
#include "Orientation.h"
#include "ProximityTracker.h"

#include "Game/Object.h"

#include "LTE/Math.h"

const int kSamplePeriod = 60;
const Vector<ObjectType> kTypesToTrack =
  Vector<ObjectType>()
    << ObjectType_Asteroid
    << ObjectType_Planet
    << ObjectType_Ship
    << ObjectType_Station;

ComponentProximityTracker::ComponentProximityTracker() :
  maxDistance(2000),
  frameCounter(RandI(0, kSamplePeriod))
  {}

bool ComponentProximityTracker::IsNearby(ObjectT* self) {
  return nearby.contains(self);
}

void ComponentProximityTracker::Run(ObjectT* self, UpdateState& state) {
  if (--frameCounter > 0)
    return;
  frameCounter = kSamplePeriod;

  Position myPos = self->GetCenter();
  float myRadius = Length(self->GetExtent());

  static Vector<ObjectT*> query;
  query.clear();
  query.reserve(128);

  V3 diagonal = V3(myRadius + maxDistance);
  self->GetContainer()->QueryInterior(Bound3(myPos - diagonal, myPos + diagonal), query);

  nearby.clear();
  nearby.reserve(query.size());

  for (size_t i = 0; i < query.size(); ++i) {
    ObjectT* o = query[i];
    if (self == o || !kTypesToTrack.contains(o->GetType()))
      continue;
    Distance d = Length(myPos - o->GetCenter());
    d -= (myRadius + Length(o->GetExtent()));
    if (d < maxDistance)
      nearby << query[i];
  }
}
