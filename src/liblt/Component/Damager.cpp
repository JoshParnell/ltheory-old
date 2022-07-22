#include "Damager.h"
#include "Collidable.h"
#include "Integrity.h"
#include "Motion.h"
#include "Pilotable.h"

#include "Game/Events.h"
#include "Game/Object.h"
#include "Game/Graphics/Effects.h"

bool ComponentDamager::Hit(
  ObjectT* self,
  Object const& dest,
  Position const& position,
  float dt)
{
  ComponentCollidable* collidable = dest->GetCollidable();
  if (!collidable || !dest->CanCollide(self))
    return false;

  if (source != dest->GetRoot().t) {
    collidable->Collide(dest, self, position, position);

    float scale = 2.0f * source->GetRadius();
    Effect_SmallPlume(position, dest->GetVelocity(), type->color, scale);

    /* If the object can be damaged, damage it appropriately. */
    Damage damage = type->damage *
      (type->type == WeaponClass_Beam ? (source->GetPowerFraction() * dt) : 1.0f);

    Event_Damage(source, dest, damage);
  }

  return true;
}
