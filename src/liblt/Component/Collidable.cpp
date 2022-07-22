#include "BoundingBox.h"
#include "Collidable.h"
#include "Drawable.h"
#include "Motion.h"

#include "Game/Object.h"
#include "Game/Player.h"

#include "Module/PhysicsEngine.h"

#include "LTE/CollisionMesh.h"
#include "LTE/ProgramLog.h"
#include "LTE/Renderer.h"
#include "LTE/StackFrame.h"

const double kCollisionDamageCoefficient = 1.0 / 1000.0;
const double kResolutionCoefficient = 1.5;

namespace {
  void ResolveCollision(
    ObjectT* self,
    ObjectT* othr,
    V3 const& normal)
  {
    ComponentMotion* motion = self->GetMotion();
    double selfMass = (double)self->GetMass();
    double othrMass = (double)othr->GetMass();
    V3 selfVelocity = self->GetVelocity();
    V3 othrVelocity = othr->GetVelocity();

    double totalEnergy =
      selfMass * LengthSquared(selfVelocity) * Saturate( Dot(Normalize(selfVelocity), normal)) +
      othrMass * LengthSquared(othrVelocity) * Saturate(-Dot(Normalize(othrVelocity), normal));
    
    double othrMassFraction = othrMass / (selfMass + othrMass);
    double damage = kCollisionDamageCoefficient * totalEnergy * othrMassFraction;

    /* TEMPCHANGE Don't apply collision damage to AI pilots. */
    Player const& p = self->GetOwner();

    /* CRITICAL Don't apply any collision damage...ever ;) */
    if (false && (!p || p->IsHuman()))
      self->ApplyDamage((Damage)damage);

    if (motion)
      motion->velocity -=
        (float)(kResolutionCoefficient * othrMassFraction) *
        Max(0.f, Dot(motion->velocity, normal)) * normal;
  }
}

ComponentCollidable::ComponentCollidable() :
  passive(true),
  solid(true)
  {}

void ComponentCollidable::CheckCollisions(ObjectT* self, UpdateState& state) {
  AUTO_FRAME;
  if (!state.hasFocus)
    return;

  /* CRITICAL. Never check collisions for non-player objects. */
  Player const& owner = self->GetOwner();
  if (!owner || !owner->IsHuman())
    return;

  if (!self->CanMove())
    return;

  ComponentBoundingBox& bb = *self->GetBoundingBox();
  ObjectT* context = self->GetContainer();

  ComponentMotion* myMotion = self->GetMotion();
  float mySpeed = myMotion ? myMotion->speed : 0.f;

  /* We can use the version to detect whether the BB has been initialized. */
  if (!bb.orientationVersion)
    return;

  /* Get a list of nearby objects. */
  static Vector<ObjectT*> nearbyObjects;
  nearbyObjects.clear();
  nearbyObjects.reserve(100);
  Bound3D const& myBound = self->GetGlobalBound();
  context->QueryInterior(myBound, nearbyObjects);

  for (size_t i = 0; i < nearbyObjects.size(); ++i) {
    ObjectT* other = nearbyObjects[i];
    if (self == other)
      continue;

    ComponentCollidable* otherCollidable = other->GetCollidable();
    if (!otherCollidable)
      continue;

    if (!solid && !otherCollidable->solid)
      continue;

    ComponentBoundingBox* otherBoundingBox = other->GetBoundingBox();
    if (!otherBoundingBox->orientationVersion)
      continue;

    /* Query the collision filters to determine whether we should proceed
     * with checking this collision. */
    if (!self->CanCollide(other))
      continue;
    if (!other->CanCollide(self))
      continue;

    ComponentMotion* otherMotion = other->GetMotion();
    float otherSpeed = otherMotion ? otherMotion->speed : 0.0f;

    /* Don't bother if we're hardly moving. */
    if (mySpeed < 1e-4f && otherSpeed < 1e-4f)
      continue;

    if (myBound.Overlaps(other->GetGlobalBound())) {
      V3 contactNormal;
      bool collided = GetPhysicsEngine()->CheckCollision(self, other, &contactNormal);

      if (collided) {
        /* Generic collision-resolution logic. */
        if (solid && otherCollidable->solid) {
          ResolveCollision(self, other, contactNormal);
          ResolveCollision(other, self, -contactNormal);
        }

        /* Notify collision handlers. */
        if (otherCollidable->solid)
          self->OnCollide(self, other, 0, 0);
        if (solid)
          other->OnCollide(other, self, 0, 0);
      }
    }
  }
}

void ComponentCollidable::Collide(
  ObjectT* self,
  ObjectT* other,
  Position const& pSelf,
  Position const& pOther)
{
  self->OnCollide(self, other, pSelf, pOther);
}
