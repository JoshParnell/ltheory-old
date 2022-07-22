#include "Zoned.h"

#include "Interior.h"

#include "Game/Object.h"
#include "LTE/Transform.h"

float ComponentZoned::GetContainment(ObjectT* self, Position const& point) {
  Transform const& transform = self->GetTransform();
  Position relative = transform.InversePoint(point);
  return Exp(-16.0f * Max(0.0f, region->Evaluate((V3)relative)));
}

DefineFunction(Object_GetZone) {
  ObjectT* container = args.object->GetContainer();
  if (!container)
    return nullptr;

  Position const& pos = args.object->GetPos();
  Object maxZone;
  float maxContainment = 0.5f;

  for (ObjectType type = 0; type < ObjectType_SIZE; ++type) {
    for (InteriorTypeIterator it = Object_GetInteriorObjects(container, type);
         it.HasMore(); it.Advance())
    {
      Object const& object = it.Get();
      if (!object->GetZoned())
        break;

      Pointer<ComponentZoned> zone = object->GetZoned();
      float containment = zone->GetContainment(object, pos);
      if (containment > maxContainment) {
        maxZone = object;
        maxContainment = containment;
      }
    }
  }

  return maxZone;
} FunctionAlias(Object_GetZone, GetZone);
